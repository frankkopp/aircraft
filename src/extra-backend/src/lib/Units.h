// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <map>
#include <string>

#include <MSFS/Legacy/gauges.h>

class Units {
public:
  ENUM Percent = get_units_enum("Percent");
  ENUM Number = get_units_enum("Number");
  ENUM Bool = get_units_enum("Bool");
  ENUM Pounds = get_units_enum("Pounds");
  ENUM Psi = get_units_enum("Psi");
  ENUM Pph = get_units_enum("Pounds per hour");
  ENUM Gallons = get_units_enum("Gallons");
  ENUM Feet = get_units_enum("Feet");
  ENUM FootPounds = get_units_enum("Foot pounds");
  ENUM FeetMin = get_units_enum("Feet per minute");
  ENUM FeetSec = get_units_enum("Feet per second");
  ENUM Mach = get_units_enum("Mach");
  ENUM Millibars = get_units_enum("Millibars");
  ENUM Celsius = get_units_enum("Celsius");
  ENUM Hours = get_units_enum("Hours");
  ENUM Seconds = get_units_enum("Seconds");

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
  };
};

inline Units UNITS{};

