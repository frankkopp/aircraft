// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include "AircraftVariable.h"

#include <utility>

AircraftVariable::AircraftVariable(
  const std::string &varName,
  int varIndex,
  std::string setterEventName,
  EventPtr setterEvent,
  ENUM unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks)
  : CacheableVariable(varName, varIndex, unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks),
    setterEventName(std::move(setterEventName)), setterEvent(std::move(setterEvent)) {

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

void AircraftVariable::setToSim() {
  if (setterEventName.empty() && setterEvent == nullptr) {
    std::cerr << "AircraftVariable::setAndWriteToSim() called on \"" << varName
              << "\" but no setter event name is set" << std::endl;
    return;
  }
  if (cachedValue.has_value()) {
    dirty = false;
    // use the given event if one is set
    if (setterEvent) {
      useEventSetter();
      return;
    }
    // Alternative use calculator code if no event is set
    useCalculatorCodeSetter();
    return;
  }
  std::cerr << "AircraftVariable::setAndWriteToSim() called on [" << varName
            << "] but no value is cached"
            << std::endl;
}

void AircraftVariable::useEventSetter() {
  const auto data = static_cast<DWORD>(cachedValue.value());
  bool result;
  if (index) {
    result = setterEvent->trigger(index, data);
  } else {
    result = setterEvent->trigger(data);
  }
  if (!result) {
    std::cerr << "AircraftVariable::setAndWriteToSim() called on \"" << varName
              << "\" but the event could not be triggered" << std::endl;
  }
}

void AircraftVariable::useCalculatorCodeSetter() {
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
  if (gauge_calculator_code_precompile(&pCompiled, &pCompiledSize, calculator_code.c_str())) {
    if (!execute_calculator_code(pCompiled, nullptr, nullptr, nullptr)) {
      std::cerr << "AircraftVariable::setAndWriteToSim() failed to execute calculator code: ["
                << calculator_code << "]" << std::endl;
    }
  } else {
    std::cerr << "Failed to precompile calculator code for " << varName << std::endl;
  }
}


