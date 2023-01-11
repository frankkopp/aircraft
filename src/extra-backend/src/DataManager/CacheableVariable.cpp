// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "CacheableVariable.h"

FLOAT64 CacheableVariable::get() const {
  if (cachedValue.has_value()) {
    if (dirty) {
      std::cerr << "CacheableVariable::requestUpdateFromSim() called on '" << varName
                << "' but the value is dirty" << std::endl;
    }
    return cachedValue.value();
  }
  std::cerr << "CacheableVariable::get() called on '" << varName << "' but no value is cached"
            << std::endl;
  return FLOAT64{};
}

FLOAT64 CacheableVariable::updateFromSim(FLOAT64 timeStamp, UINT64 tickCounter) {
  if (!cachedValue.has_value()) {
    return getFromSim();
  }
  // only update if the value is equal or older than the max age for sim time or ticks
  if (timeStampSimTime + maxAgeTime >= timeStamp || tickStamp + maxAgeTicks >= tickCounter) {
    return cachedValue.value();
  }
  // update the value from the sim
  const FLOAT64 simValue = getFromSim();
  timeStampSimTime = timeStamp;
  tickStamp = tickCounter;
  return simValue;
}

void CacheableVariable::updateToSim() {
  if (cachedValue.has_value() && dirty) {
    setToSim();
  }
}

void CacheableVariable::set(FLOAT64 value) {
  if (cachedValue.has_value() && cachedValue.value() == value) {
    return;
  }
  cachedValue = value;
  dirty = true;
}

void CacheableVariable::setToSim(FLOAT64 value) {
  set(value);
  setToSim();
}
