// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "DataManager.h"

DataManager::DataManager() = default;

bool DataManager::initialize() {
  std::cout << "DataManager::initialize()" << std::endl;
  return true;
}

bool DataManager::preUpdate(sGaugeDrawData *pData) {
  std::cout << "DataManager::preUpdate()" << std::endl;
  return true;
}

bool DataManager::update(sGaugeDrawData *pData) {
  std::cout << "DataManager::update()" << std::endl;
  return true;
}

bool DataManager::postUpdate(sGaugeDrawData *pData) {
  std::cout << "DataManager::postUpdate()" << std::endl;
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
