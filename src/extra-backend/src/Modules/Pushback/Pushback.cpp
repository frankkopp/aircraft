// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "MsfsHandler.h"
#include "Pushback.h"
#include "Units.h"

static constexpr double SPEED_RATIO = 18.0;
static constexpr double TURN_SPEED_RATIO = 0.16;

Pushback::Pushback(MsfsHandler* msfsHandler) : Module(msfsHandler) {}

///
// DataManager Howto Note:
// =======================

// The Pushback module uses the DataManager to get and set variables.
// Looking at the make_xxx_var functions, you can see that they are updated
// with different update cycles.
//
// Some variables are read from the sim at every tick:
// - A32NX_PUSHBACK_SYSTEM_ENABLED
// - Pushback Attached
// - SIM ON GROUND
//
// The rest are read on demand after the state of the above variables have been checked.
//
// No variable is written automatically.
///


bool Pushback::initialize() {
  std::cout << "Pushback::initialize()" << std::endl;

  dataManager = &msfsHandler->getDataManager();

  // LVARs
  pushbackSystemEnabled = dataManager->make_named_var("A32NX_PUSHBACK_SYSTEM_ENABLED", UNITS.Bool, true);
  updateDelta = dataManager->make_named_var("A32NX_PUSHBACK_UPDT_DELTA");
  parkingBrakeEngaged = dataManager->make_named_var("A32NX_PARK_BRAKE_LEVER_POS");
  tugCommandedSpeedFactor = dataManager->make_named_var("A32NX_PUSHBACK_SPD_FACTOR");
  tugCommandedHeadingFactor = dataManager->make_named_var("A32NX_PUSHBACK_HDG_FACTOR");
  tugCommandedSpeed = dataManager->make_named_var("A32NX_PUSHBACK_SPD");
  tugCommandedHeading = dataManager->make_named_var("A32NX_PUSHBACK_HDG");
  tugInertiaSpeed = dataManager->make_named_var("A32NX_PUSHBACK_INERTIA_SPD");
  rotXOut = dataManager->make_named_var("A32NX_PUSHBACK_R_X_OUT");

  // Simvars
  pushbackAttached = dataManager->make_aircraft_var("Pushback Attached", 0, "", nullptr, UNITS.Bool, true);
  simOnGround = dataManager->make_aircraft_var("SIM ON GROUND", 0, "", nullptr, UNITS.Bool, true);
  aircraftHeading = dataManager->make_aircraft_var("PLANE HEADING DEGREES TRUE", 0, "", nullptr, UNITS.Rad);
  windVelBodyZ = dataManager->make_aircraft_var("RELATIVE WIND VELOCITY BODY Z");

  // Data definitions for PushbackDataID
  std::vector<DataDefinitionVariable::DataDefinition> pushBackDataDef = {
    {"Pushback Wait",                0, UNITS.Bool},
    {"VELOCITY BODY Z",              0, UNITS.FeetSec},
    {"ROTATION VELOCITY BODY Y",     0, UNITS.FeetSec},
    {"ROTATION ACCELERATION BODY X", 0, UNITS.RadSecSquared}
  };
  pushbackData = dataManager->make_datadefinition_var(
    "PUSHBACK DATA", pushBackDataDef, &pushbackDataStruct, sizeof(PushbackData));

  // Events
  tugHeadingEvent = dataManager->make_event("KEY_TUG_HEADING");
  tugSpeedEvent = dataManager->make_event("KEY_TUG_SPEED");

  std::cout << "Pushback::initialized()" << std::endl;
  isInitialized = true;
  return true;
}

bool Pushback::preUpdate(sGaugeDrawData* pData) {
  // empty
  return true;
}

bool Pushback::update(sGaugeDrawData* pData) {
  if (!isInitialized || !pushbackSystemEnabled->getAsBool()
      || !pushbackAttached->getAsBool() || !simOnGround->getAsBool()) {
    return true;
  }

  updateDelta->setAndWriteToSim(pData->dt); // debug value

  // read all data from sim - could be done inline but better readability this way
  parkingBrakeEngaged->readFromSim();
  aircraftHeading->readFromSim();
  tugCommandedSpeedFactor->readFromSim();
  tugCommandedHeadingFactor->readFromSim();

  const double parkBrakeSpdFactor = parkingBrakeEngaged->getAsBool() ? (SPEED_RATIO / 10) : SPEED_RATIO;
  const FLOAT64 tugCmdSpd = tugCommandedSpeedFactor->get() * parkBrakeSpdFactor;
  tugCommandedSpeed->setAndWriteToSim(tugCmdSpd); // debug value

  const FLOAT64 inertiaSpeed = inertialDampener.updateSpeed(tugCmdSpd);
  tugInertiaSpeed->setAndWriteToSim(inertiaSpeed); // debug value

  const double parkingBrakeHdgFactor =
    parkingBrakeEngaged->getAsBool() ? (TURN_SPEED_RATIO / 10) : TURN_SPEED_RATIO;
  const FLOAT64 computedRotationVelocity = sgn<FLOAT64>(tugCmdSpd)
                                           * tugCommandedHeadingFactor->get()
                                           * parkingBrakeHdgFactor;

  // As we might use the elevator for taxiing we compensate for wind to avoid
  // the aircraft lifting any gears.
  const FLOAT64 windCounterRotAccel = windVelBodyZ->readFromSim() / 2000.0;
  FLOAT64 movementCounterRotAccel = windCounterRotAccel;
  if (inertiaSpeed > 0) movementCounterRotAccel -= 0.5;
  else if (inertiaSpeed < 0) movementCounterRotAccel += 1.0;
  else movementCounterRotAccel = 0.0;
  rotXOut->setAndWriteToSim(movementCounterRotAccel); // debug value

  // K:KEY_TUG_HEADING expects an unsigned integer scaling 360° to 0 to 2^32-1 (0xffffffff / 360)
  FLOAT64 aircraftHeadingDeg = aircraftHeading->get() * (180.0 / PI);
  const FLOAT64 computedHdg = angleAdd(aircraftHeadingDeg,
                                       -90 * tugCommandedHeadingFactor->get());
  tugCommandedHeading->setAndWriteToSim(computedHdg); // debug value

  // TUG_HEADING units are a 32-bit integer (0 to 4294967295) which represent 0 to 360 degrees.
  // To set a 45-degree angle, for example, set the value to 4294967295 / 8.
  // https://docs.flightsimulator.com/html/Programming_Tools/Event_IDs/Aircraft_Misc_Events.htm#TUG_HEADING
  constexpr DWORD headingToInt32 = 0xffffffff / 360;
  const DWORD convertedComputedHeading = static_cast<DWORD>(computedHdg) * headingToInt32;

  // send K:KEY_TUG_HEADING event
  tugHeadingEvent->trigger_ex1(convertedComputedHeading);

  // K:KEY_TUG_SPEED - seems to actually do nothing
  //  tugSpeedEvent->trigger_ex1(static_cast<DWORD>(inertiaSpeed));

  // Update sim data
  pushbackDataStruct.pushbackWait = inertiaSpeed == 0 ? 1 : 0;
  pushbackDataStruct.velBodyZ = inertiaSpeed;
  pushbackDataStruct.rotVelBodyY = computedRotationVelocity;
  pushbackDataStruct.rotAccelBodyX = movementCounterRotAccel;
  pushbackData->writeToSim();

  return true;
}

bool Pushback::postUpdate(sGaugeDrawData* pData) {
  //  empty
  return true;
}

bool Pushback::shutdown() {
  isInitialized = false;
  std::cout << "Pushback::shutdown()" << std::endl;
  return true;
}
