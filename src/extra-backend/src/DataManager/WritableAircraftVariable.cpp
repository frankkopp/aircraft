// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include "WritableAircraftVariable.h"

#include <utility>

WritableAircraftVariable::WritableAircraftVariable(
  const std::string &varName,
  int varIndex,
  std::string setterEventName,
  ENUM unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks)
  : CacheableVariable(varName, varIndex, unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks),
    setterEventName(std::move(setterEventName)) {

  // check if variable is indexed and register the correct index
  dataID = get_aircraft_var_enum(varName.c_str());
  if (dataID == -1) { // cannot throw an exception in MSFS
    std::cerr << ("Aircraft variable " + varName + " not found") << std::endl;
  }
}

FLOAT64 WritableAircraftVariable::getFromSim() {
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

void WritableAircraftVariable::setToSim() {
  if (cachedValue.has_value()) {
    dirty = false;

    std::string calculator_code;
    calculator_code += std::to_string(cachedValue.value());
    calculator_code += " ";
    if (index != 0) {
      calculator_code += std::to_string(index);
      calculator_code += " ";
      calculator_code += " (>K:2:" + setterEventName + ")";
    }
    else {
      calculator_code += " (>K:" + setterEventName + ")";
    }

    PCSTRINGZ pCompiled{};
    UINT32 pCompiledSize{};
    gauge_calculator_code_precompile(&pCompiled, &pCompiledSize, calculator_code.c_str());
    if (!execute_calculator_code(pCompiled, nullptr, nullptr, nullptr)) {
      std::cerr << "WritableAircraftVariable::setAndWriteToSim() failed to execute calculator code: ["
                << calculator_code << "]" << std::endl;
    }

    return;
  }
  std::cerr << "WritableAircraftVariable::setAndWriteToSim() called on [" << varName
            << "] but no value is cached"
            << std::endl;
}

