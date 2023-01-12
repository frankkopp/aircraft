// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include <utility>

#include "Module.h"
#include "MsfsHandler.h"
#include "Units.h"

// =================================================================================================
// PUBLIC METHODS
// =================================================================================================

MsfsHandler::MsfsHandler(std::string name) : simConnectName(std::move(name)) {};

void MsfsHandler::registerModule(Module* pModule) {
  modules.push_back(pModule);
}

bool MsfsHandler::initialize() {
  // Initialize SimConnect
  bool result;
  result = initializeSimConnect();
  if (!result) {
    std::cout << simConnectName << ": Failed to initialize SimConnect" << std::endl;
    return false;
  }

  // Initialize data manager
  result = dataManager.initialize(hSimConnect);
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

  // initialize all data variables needed for the handler itself
  a32nxIsDevelopmentState = dataManager.make_named_var("A32NX_DEVELOPER_STATE", UNITS.Bool, true);
  a32nxIsReady = dataManager.make_named_var("A32NX_IS_READY", UNITS.Bool, true);
  // base sim data mainly for pause detection
  std::vector<DataDefinitionVariable::DataDefinition> baseDataDef = {{"SIMULATION TIME", 0, UNITS.Number},};
  baseSimData = dataManager.make_datadefinition_var("BASE DATA", baseDataDef, &simData, sizeof(simData));

  isInitialized = result;
  return result;
}

bool MsfsHandler::update(sGaugeDrawData* pData) {
  if (!isInitialized) {
    std::cout << simConnectName << ": MsfsHandler::update() - not initialized" << std::endl;
    return false;
  }

  // detect pause - uses the base sim data definition to retrieve the SIMULATION TIME
  // and run a separate pair of requestFromSim() and requestData() for it
  if (baseSimData->requestFromSim()) dataManager.requestData();
  if (simData.simulationTime == previousSimulationTime) return true;
  previousSimulationTime = simData.simulationTime;
  tickCounter++;

  // Call preUpdate(), update() and postUpdate() for all modules
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
    //    std::cout << *a32nxIsDevelopmentState << std::endl;
    //    std::cout << *a32nxIsReady << std::endl;
    //    std::cout << *baseSimData << std::endl;
    //    std::cout << "time=" << simData.simulationTime << std::endl;
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

// =================================================================================================
// PRIVATE METHODS
// =================================================================================================

bool MsfsHandler::initializeSimConnect() {
  return SUCCEEDED(SimConnect_Open(&hSimConnect, simConnectName.c_str(), nullptr, 0, 0, 0));
}
