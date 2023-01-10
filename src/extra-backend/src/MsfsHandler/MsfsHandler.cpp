// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include <utility>

#include "Module.h"
#include "MsfsHandler.h"
#include "Units.h"

// PUBLIC METHODS
// ===========================

MsfsHandler::MsfsHandler(std::string name) : simConnectName(std::move(name)) {
  a32nxIsDevelopmentState = dataManager.make_named_var("A32NX_DEVELOPER_STATE", UNITS.Bool, true, false, 0, 0);
  a32nxIsReady = dataManager.make_named_var("A32NX_IS_READY", UNITS.Bool, true, false, 0, 0);
  simOnGround = dataManager.make_aircraft_var("SIM ON GROUND", 0, UNITS.Bool, true, 0, 0);
};

void MsfsHandler::registerModule(Module* pModule) { modules.push_back(pModule); }

bool MsfsHandler::initialize() {

  bool result;

  // Initialize SimConnect
  result = initializeSimConnect();
  if (!result) {
    std::cout << simConnectName << ": Failed to initialize SimConnect" << std::endl;
    return false;
  }

  // Initialize data manager
  result = dataManager.initialize();
  if (!result) {
    std::cout << simConnectName << ": Failed to initialize data manager" << std::endl;
    return false;
  }

  // Initialize modules
  result = true;
  result &= std::all_of(modules.begin(), modules.end(), [](
    Module* pModule) { return pModule->initialize(); });
  if (!result) {
    std::cout << simConnectName << ": Failed to initialize modules" << std::endl;
    return false;
  }

  isInitialized = result;
  return result;
}

bool MsfsHandler::update(sGaugeDrawData* pData) {
  if (!isInitialized) {
    std::cout << simConnectName << ": MsfsHandler::update() - not initialized" << std::endl;
    return false;
  }
  tickCounter++;

  // TODO: Pause detection

  bool result = true;
  result &= dataManager.preUpdate(pData);
  result &= std::all_of(modules.begin(), modules.end(), [&pData](
    Module* pModule) { return pModule->preUpdate(pData); });

  result &= dataManager.update(pData);
  result &= std::all_of(modules.begin(), modules.end(), [&pData](
    Module* pModule) { return pModule->update(pData); });

  result &= dataManager.postUpdate(pData);
  result &= std::all_of(modules.begin(), modules.end(), [&pData](
    Module* pModule) { return pModule->postUpdate(pData); });

  if (!result) {
    std::cout << simConnectName << ": MsfsHandler::update() - failed" << std::endl;
  }

  // TODO: Remove these test variables
  if (tickCounter % 100 == 0) {
    std::cout << *a32nxIsDevelopmentState << std::endl;
    std::cout << *a32nxIsReady << std::endl;
    std::cout << *simOnGround << std::endl;
  }

  return result;
}

bool MsfsHandler::shutdown() {
  bool result = true;
  result &= dataManager.shutdown();
  result &= std::all_of(modules.begin(), modules.end(), [](
    Module* pModule) { return pModule->shutdown(); });
  return result;
}

// PRIVATE METHODS
// ===================================

bool MsfsHandler::initializeSimConnect() {
  return SUCCEEDED(SimConnect_Open(&hSimConnect, simConnectName.c_str(), nullptr, 0, 0, 0));
}
