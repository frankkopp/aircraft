// Copyright (c) 2023 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_PUSHBACK_H
#define FLYBYWIRE_PUSHBACK_H

#include "DataManager.h"
#include "Module.h"
#include "Pushback/InertialDampener.hpp"

#ifdef __cpp_lib_math_constants
#include <numbers>
constexpr double PI = std::numbers::pi;
#else
constexpr double PI = 3.14159265358979323846;
#endif

class MsfsHandler;

/**
 * This module is responsible for the pushback process.
 *
 * It is controlled by two LVARs:
 * - A32NX_PUSHBACK_SYSTEM_ENABLED
 * - A32NX_PUSHBACK_SPD_FACTOR
 * - A32NX_PUSHBACK_HDG_FACTOR
 *
 * - Pushback Attached (simvar)
 * - SIM ON GROUND (simvar)
 */
class Pushback : public Module {
 private:
  static constexpr int TEST = 11;

  static const SIMCONNECT_NOTIFICATION_GROUP_ID NOTIFICATION_GROUP_1 = 1;

  // Convenience pointer to the data manager
  DataManager* dataManager = nullptr;

  // Used to smoothen acceleration and deceleration
  InertialDampener inertialDampener{0.0, 0.15, 0.1};

  // LVARs
  NamedVariablePtr pushbackSystemEnabled;
  NamedVariablePtr parkingBrakeEngaged;
  NamedVariablePtr tugCommandedHeadingFactor;
  NamedVariablePtr tugCommandedSpeedFactor;
  // debug purposes - send as LVARs for debugging to the flyPad
  NamedVariablePtr pushbackDebug;
  NamedVariablePtr tugCommandedHeading;
  NamedVariablePtr tugCommandedSpeed;
  NamedVariablePtr tugInertiaSpeed;
  NamedVariablePtr updateDelta;
  NamedVariablePtr rotXOut;

  // Sim-vars
  AircraftVariablePtr simOnGround;
  AircraftVariablePtr pushbackAttached;
  AircraftVariablePtr aircraftHeading;
  AircraftVariablePtr windVelBodyZ;

  // Data structure for PushbackDataID
  struct PushbackData {
    FLOAT64 pushbackWait;
    FLOAT64 velBodyZ;
    FLOAT64 rotVelBodyY;
    FLOAT64 rotAccelBodyX;
  };
  std::shared_ptr<DataDefinitionVariable<PushbackData>> pushbackData;

  // Events
  ClientEventPtr tugHeadingEvent;
  ClientEventPtr tugSpeedEvent;

  // Profiler for measuring the update time
  SimpleProfiler profiler{"Pushback::update", 120};

 public:
  Pushback() = delete;

  /**
   * Creates a new Pushback instance and takes a reference to the MsfsHandler instance.
   * @param msfsHandler The MsfsHandler instance that is used to communicate with the simulator.
   */
  explicit Pushback(MsfsHandler& msfsHandler) : Module(msfsHandler) {
    std::cout << "Pushback constructor: " << TEST << std::endl;
  }

  bool initialize() override;
  bool preUpdate(sGaugeDrawData* pData) override;
  bool update(sGaugeDrawData* pData) override;
  bool postUpdate(sGaugeDrawData* pData) override;
  bool shutdown() override;

 protected:
  /**
   * Calculates the counter rotation acceleration. This is required as the elevator creates a lift force
   * especially if a user uses the stick for taxiing as forward and backward taxiing are controlled
   * by the same axis as the elevator.
   * Especially in strong winds this can lead to the aircraft lifting its nose or gears.
   * @param inertiaSpeed The current inertia speed.
   * @param windVelBodyZ The current wind velocity in body Z direction.
   * @return The counter rotation acceleration.
   */
  virtual FLOAT64 calculateCounterRotAccel(const FLOAT64 inertiaSpeed, AircraftVariablePtr& windVelBodyZ) const = 0;

  /**
   * @brief Returns the park brake factor for slowing down when the parking brake is engaged.
   * @return The park brake factor.
   */
  virtual constexpr int getParkBrakeFactor() const = 0;  // slow down when parking brake is engaged by this factor

  /**
   * @brief Returns the speed factor for the pushback for the aircraft
   * @return the speed factor
   */
  virtual constexpr FLOAT64 getSpeedFactor() const = 0;  // ft/sec for "VELOCITY BODY Z"

  /**
   * @brief Returns the turn speed factor for the pushback for the aircraft
   * @return the turn speed factor
   */
  virtual constexpr FLOAT64 getTurnSpeedFactor() const = 0;  // ft/sec for "ROTATION VELOCITY BODY Y"
};

#endif  // FLYBYWIRE_PUSHBACK_H
