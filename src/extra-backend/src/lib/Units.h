// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <map>
#include <string>

#include <MSFS/Legacy/gauges.h>

/**
 * The Units class is a helper class to make handling of MSFS SDK units easier.
 * Add any additional required units here as the MSFS SDK has many more.
 */
class Units {
public:
  const ENUM Percent = get_units_enum("Percent");
  const ENUM Number = get_units_enum("Number");
  const ENUM Bool = get_units_enum("Bool");
  const ENUM Pounds = get_units_enum("Pounds");
  const ENUM Psi = get_units_enum("Psi");
  const ENUM Pph = get_units_enum("Pounds per hour");
  const ENUM Gallons = get_units_enum("Gallons");
  const ENUM Feet = get_units_enum("Feet");
  const ENUM FootPounds = get_units_enum("Foot pounds");
  const ENUM FeetMin = get_units_enum("Feet per minute");
  const ENUM FeetSec = get_units_enum("Feet per second");
  const ENUM Mach = get_units_enum("Mach");
  const ENUM Millibars = get_units_enum("Millibars");
  const ENUM Celsius = get_units_enum("Celsius");
  const ENUM Hours = get_units_enum("Hours");
  const ENUM Seconds = get_units_enum("Seconds");
  const ENUM RadiansSec = get_units_enum("Radians per second");
  const ENUM RadiansSecSquared = get_units_enum("Radians per second squared");

  std::map<ENUM, std::string> unitStrings = {
    {Percent,    "Percent"},
    {Number,     "Number"},
    {Bool,       "Bool"},
    {Pounds,     "Pounds"},
    {Psi,        "Psi"},
    {Pph,        "Pph"},
    {Gallons,    "Gallons"},
    {Feet,       "Feet"},
    {FootPounds, "FootPounds"},
    {FeetMin,    "FeetMin"},
    {FeetSec,    "FeetSec"},
    {Mach,       "Mach"},
    {Millibars,  "Millibars"},
    {Celsius,    "Celsius"},
    {Hours,      "Hours"},
    {Seconds,    "Seconds"},
    {RadiansSec, "RadiansSec"},
    {RadiansSecSquared, "RadiansSecSquared"}
  };
};

inline Units UNITS{};

