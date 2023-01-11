// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include "AircraftVariable.h"

AircraftVariable::AircraftVariable(
  const std::string& varName,
  int varIndex,
  ENUM unit,
  bool autoReading,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks)
  : CacheableVariable(varName, varIndex, unit, autoReading, false, maxAgeTime, maxAgeTicks)
{
  // check if variable is indexed and register the correct index
  dataID = get_aircraft_var_enum(varName.c_str());
  if (dataID == -1) { // cannot throw an exception in MSFS
    std::cerr << ("Aircraft variable " + varName + " not found") << std::endl;
  }
}

FLOAT64 AircraftVariable::getFromSim() {
  if (dataID == -1) {
    std::cerr << ("Aircraft variable " + varName + " not found") << std::endl;
    dirty = false;
    return FLOAT64{};
  }
  const FLOAT64 value = aircraft_varget(dataID, unit, index);
  cachedValue = value;
  dirty = false;
  return value;
}


