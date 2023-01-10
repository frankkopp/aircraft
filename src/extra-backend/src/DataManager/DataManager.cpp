// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "DataManager.h"
#include "NamedVariable.h"

DataManager::DataManager() = default;

bool DataManager::initialize() {
  // empty
  return true;
}

bool DataManager::preUpdate(sGaugeDrawData* pData) {
  tickCounter++;
  timeStamp = pData->t;
  //  std::cout << "DataManager::preUpdate(): " << tickCounter << " " << timeStampSimTime << std::endl;
  for (auto &var: variables) {
    if (var.second->isAutoRead()) {
      var.second->updateFromSim(timeStamp, tickCounter);
    }
  }
  return true;
}

bool DataManager::update(sGaugeDrawData* pData) {
  // empty
  return true;
}

bool DataManager::postUpdate(sGaugeDrawData* pData) {
  for (auto &var: variables) {
    if (var.second->isAutoWrite()) {
      // aircraft variables will return false for isAutoWrite() so this will not be called
      var.second->updateToSim(timeStamp, tickCounter);
    }
  }
  return true;
}

bool DataManager::processSimObjectData(SIMCONNECT_RECV_SIMOBJECT_DATA* pData) {
  std::cout << "DataManager::processSimObjectData()" << std::endl;
  return false;
}

bool DataManager::shutdown() {
  // empty
  return true;
}

// clang-format off
std::shared_ptr<NamedVariable>
DataManager::make_named_var(
  std::string varName,
  ENUM unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) { // clang-format on
  // TODO - check if variable already exists
  std::shared_ptr<NamedVariable> var =
    std::make_shared<NamedVariable>(varName, unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks);
  variables[var->getVarName()] = var;
  return var;
}

std::shared_ptr<AircraftVariable>
DataManager::make_aircraft_var(
  std::string varName,
  int index,
  ENUM unit,
  bool autoReading,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {
  // TODO - check if variable already exists
  std::shared_ptr<AircraftVariable> var =
    std::make_shared<AircraftVariable>(varName, index, unit, autoReading, maxAgeTime, maxAgeTicks);
  variables[var->getVarName()] = var;
  return var;
}
