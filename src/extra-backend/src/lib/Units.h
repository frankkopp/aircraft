// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_UNITS_H
#define FLYBYWIRE_A32NX_UNITS_H

#include <map>
#include <string>
#include <utility>

#include <MSFS/Legacy/gauges.h>

class Unit {
public:
  const char* name;
  const ENUM id;
  [[maybe_unused]]
  explicit Unit(const char* nameInSim) : name(nameInSim), id(get_units_enum(name)) {}
};

/**
 * The Units class is a helper class to make handling of MSFS SDK units easier.
 * Add any additional required units here as the MSFS SDK has many more.
 */
class Units {
public:
  const Unit Bool{"Bool"};
  const Unit Celsius{"Celsius"};
  const Unit Feet{"Feet"};
  const Unit FeetMin{"feet/minute"};
  const Unit FeetSec{"feet/second"};
  const Unit FeetSecSquared{"feet per second squared"};
  const Unit FootPounds{"Foot pounds"};
  const Unit Gallons{"Gallons"};
  const Unit Hours{"Hours"};
  const Unit Mach{"Mach"};
  const Unit Millibars{"Millibars"};
  const Unit Number{"Number"};
  const Unit Percent{"Percent"};
  const Unit PercentOver100{"Percent over 100"};
  const Unit Pounds{"Pounds"};
  const Unit Pph{"Pounds per hour"};
  const Unit Psi{"Psi"};
  const Unit Rad{"radians"};
  const Unit RadSec{"radians per second"};
  const Unit RadSecSquared{"radians per second squared"};
  const Unit Seconds{"Seconds"};
};

inline Units UNITS{};

#endif //FLYBYWIRE_A32NX_UNITS_H
