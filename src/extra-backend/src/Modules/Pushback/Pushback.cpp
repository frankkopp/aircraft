// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include <chrono>

#include "MsfsHandler.h"
#include "MsfsHandler/DataManager/DataManager.h"
#include "InertialDampener.h"
#include "Pushback.h"
#include "Units.h"

using namespace std::chrono;

static constexpr double SPEED_RATIO = 18.0;
static constexpr double TURN_SPEED_RATIO = 0.16;

Pushback::Pushback(MsfsHandler* msfsHandler) : Module(msfsHandler) {
  dataManager = &msfsHandler->getDataManager();
}

bool Pushback::initialize() {
  std::cout << "Pushback::initialize()" << std::endl;

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
  pushbackAttached = dataManager->make_aircraft_var("Pushback Attached", 0, "", UNITS.Bool, true);
  simOnGround = dataManager->make_aircraft_var("SIM ON GROUND", 0, "", UNITS.Bool, true);
  aircraftHeading = dataManager->make_aircraft_var("PLANE HEADING DEGREES TRUE");
  windVelBodyZ = dataManager->make_aircraft_var("RELATIVE WIND VELOCITY BODY Z");

  // base sim data mainly for pause detection
  std::vector<DataDefinitionVariable::DataDefinition> pushBackDataDef = {
    {"Pushback Wait",                0, UNITS.Bool},
    {"VELOCITY BODY Z",              0, UNITS.FeetSec},
    {"ROTATION VELOCITY BODY Y",     0, UNITS.FeetSec},
    {"ROTATION ACCELERATION BODY X", 0, UNITS.RadiansSecSquared}
  };
  pushbackData = dataManager->make_datadefinition_var(
    "PUSHBACK DATA", pushBackDataDef, &pushbackDataStruct, sizeof(pushbackDataStruct));

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
      || pushbackAttached->getAsBool() || !simOnGround->getAsBool()) {
    return true;
  }

  // TODO IMPLEMENT
  std::cout << "Pushback::update() after check" << std::endl;
}

bool Pushback::postUpdate(sGaugeDrawData* pData) {
  //  empty
  return true;
}

bool Pushback::shutdown() {
  //  empty
  return true;
}
