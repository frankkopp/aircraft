// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "NamedVariable.h"

// clang-format off
NamedVariable::NamedVariable(
    std::string varName,
    int varIndex,
    ENUM unit,
    bool autoUpdate,
    const std::chrono::duration<int64_t, std::milli> &maxAgeTime,
    int64_t maxAgeTicks)
    : CacheableVariable(varName, varIndex, unit, autoUpdate, maxAgeTime, maxAgeTicks)
{ // clang-format on
  if (varIndex != 0) {
    dataID = register_named_variable((varName + ":" + std::to_string(varIndex)).c_str());
  } else {
    dataID = register_named_variable(varName.c_str());
  }
}

FLOAT64 NamedVariable::getFromSim() {
  const FLOAT64 value = get_named_variable_value(dataID);
  cachedValue = value;
  dirty = false;
  return value;
}

void NamedVariable::setToSim(FLOAT64 value) {}

void NamedVariable::writeToSim() {}

