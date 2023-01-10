// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "DataManager.h"
#include "NamedVariable.h"

DataManager::DataManager() = default;

bool DataManager::initialize() {
  return true;
}

bool DataManager::preUpdate(sGaugeDrawData *pData) {
  return true;
}

bool DataManager::update(sGaugeDrawData *pData) {
  return true;
}

bool DataManager::postUpdate(sGaugeDrawData *pData) {
  return true;
}

bool DataManager::processSimObjectData(SIMCONNECT_RECV_SIMOBJECT_DATA *pData) {
  std::cout << "DataManager::processSimObjectData()" << std::endl;
  return false;
}

bool DataManager::shutdown() {
  std::cout << "DataManager::shutdown()" << std::endl;
  return true;
}

// clang-format off
template <>
std::shared_ptr<NamedVariable> DataManager::make_var<NamedVariable>(
    std::string nameInSim,
    int index,
    ENUM unit,
    bool autoUpdate,
    const std::chrono::duration<int64_t, std::milli> &maxAgeTime,
    int64_t maxAgeTicks)
{ // clang-format on
  std::shared_ptr<NamedVariable> var = std::make_shared<NamedVariable>(nameInSim, index, unit, autoUpdate, maxAgeTime, maxAgeTicks);
  variables[var->getNameInSim()] = var;
  return var;
}
