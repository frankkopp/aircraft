// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_PUSHBACK_H
#define FLYBYWIRE_A32NX_PUSHBACK_H

#include "Module.h"
#include "DataManager.h"
#include "InertialDampener.h"

#ifdef __cpp_lib_math_constants
#include <numbers>
constexpr double PI = std::numbers::pi;
#else
constexpr double PI = 3.14159265358979323846;
#endif

class MsfsHandler;

// TODO: To be implemented
class Pushback : public Module {
private:

  // Convenience pointer to the data manager
  DataManager* dataManager;

  InertialDampener inertialDampener{0.0, 0.15};

  // LVARs
  NamedVariablePtr pushbackSystemEnabled;
  NamedVariablePtr pushbackPaused;
  NamedVariablePtr tugCommandedHeadingFactor;
  NamedVariablePtr tugCommandedHeading;
  NamedVariablePtr tugCommandedSpeedFactor;
  NamedVariablePtr tugCommandedSpeed;
  NamedVariablePtr tugInertiaSpeed;
  NamedVariablePtr parkingBrakeEngaged;
  NamedVariablePtr updateDelta;
  NamedVariablePtr rotXInput;
  NamedVariablePtr rotXOut;

  // Sim-vars
  AircraftVariablePtr simOnGround;
  AircraftVariablePtr pushbackAttached;
  AircraftVariablePtr aircraftHeading;
  AircraftVariablePtr windVelBodyZ;

  // Data structure for PushbackDataID
  DataDefinitionVariablePtr pushbackData;

  // Data structure for PushbackDataID
  struct PushbackData {
    INT64 pushbackWait;
    FLOAT64 velBodyZ;
    FLOAT64 rotVelBodyY;
    FLOAT64 rotAccelBodyX;
  } pushbackDataStruct{};

  // Events
  EventPtr keyTugHeadingEvent;
  EventPtr keyTugSpeedEvent;

public:
  Pushback() = delete;

  /**
* Creates a new Pushback instance and takes a reference to the MsfsHandler instance.
* @param msfsHandler The MsfsHandler instance that is used to communicate with the simulator.
*/
  explicit Pushback(MsfsHandler* msfsHandler);

  bool initialize() override;
  bool preUpdate(sGaugeDrawData* pData) override;
  bool update(sGaugeDrawData* pData) override;
  bool postUpdate(sGaugeDrawData* pData) override;
  bool shutdown() override;

private:

  /**
   * Adds two angles with wrap around to result in 0-360°
   * @param a - positive or negative angle
   * @param b - positive or negative angle
   */
  static double angleAdd(double a, double b) {
    double r = a + b;
    while (r > 360.0) {
      r -= 360.0;
    }
    while (r < 0.0) {
      r += 360.0;
    }
    return r;
  };

  /**
   * Returns the signum (sign) of the given value.
   * @tparam T
   * @param val
   * @return sign of value or 0 when value==0
   */
  template<typename T>
  int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }
};

#endif //FLYBYWIRE_A32NX_PUSHBACK_H
