// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include <MSFS/Legacy/gauges.h>

#include "NamedVariable.h"

NamedVariable::NamedVariable(
  const std::string& varName,
  ENUM unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks)
  : CacheableVariable(varName, 0, unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks)
{
  // check if variable is indexed and register the correct index
  dataID = register_named_variable(varName.c_str());
}

FLOAT64 NamedVariable::getFromSim() {
  const FLOAT64 value = get_named_variable_value(dataID);
  cachedValue = value;
  dirty = false;
  return value;
}

void NamedVariable::setToSim() {
  if (cachedValue.has_value()) {
    dirty = false;
    set_named_variable_value(dataID, cachedValue.value());
    return;
  }
  std::cerr << "NamedVariable::setAndWriteToSim() called on \"" << varName << "\" but no value is cached"
            << std::endl;
}



