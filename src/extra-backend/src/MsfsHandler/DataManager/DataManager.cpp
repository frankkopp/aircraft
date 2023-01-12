// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "DataManager.h"
#include "NamedVariable.h"
#include "SimconnectExceptionStrings.h"

DataManager::DataManager() = default;

bool DataManager::initialize(HANDLE hdl) {
  hSimConnect = hdl;
  isInitialized = true;
  return true;
}

bool DataManager::preUpdate(sGaugeDrawData* pData) {
  if (!isInitialized) {
    std::cerr << "DataManager::preUpdate() called but DataManager is not initialized" << std::endl;
    return false;
  }

  tickCounter++;
  timeStamp = pData->t;

  // get all variables set to automatically read
  for (auto &var: variables) {
    if (var.second->isAutoRead()) {
      var.second->updateFromSim(timeStamp, tickCounter);
    }
  }

  // request all data definitions set to automatically read
  for (auto &ddv: dataDefinitionVariables) {
    if (ddv->isAutoRead()) {
      if (!ddv->requestUpdateFromSim(timeStamp, tickCounter)) {
        std::cerr << "DataManager::preUpdate(): requestUpdateFromSim() failed for "
                  << ddv->getName() << std::endl;
      }
    }
  }

  // get requested sim object data
  requestData();

  return true;
}

bool DataManager::update([[maybe_unused]] sGaugeDrawData* pData) const {
  if (!isInitialized) {
    std::cerr << "DataManager::update() called but DataManager is not initialized" << std::endl;
    return false;
  }
  // empty
  return true;
}

bool DataManager::postUpdate([[maybe_unused]] sGaugeDrawData* pData) {
  if (!isInitialized) {
    std::cerr << "DataManager::postUpdate() called but DataManager is not initialized" << std::endl;
    return false;
  }

  // write all variables set to automatically write
  // aircraft variables are not writeable and will return false for isAutoWrite()
  // so this will not be called
  for (auto &var: variables) {
    if (var.second->isAutoWrite()) {
      var.second->updateToSim();
    }
  }

  // write all data definitions set to automatically write
  for (auto &ddv: dataDefinitionVariables) {
    if (ddv->isAutoWrite()) {
      if (!ddv->updateToSim(timeStamp, tickCounter)) {
        std::cerr << "DataManager::postUpdate(): updateToSim() failed for "
                  << ddv->getName() << std::endl;
      }
    }
  }

  return true;
}

bool DataManager::processSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* data) {
  for (auto &ddv: dataDefinitionVariables) {
    if (ddv->getRequestId() == data->dwRequestID) {
      ddv->updateFromSimObjectData(data);
      return true;
    }
  }
  std::cout << "DataManager::processSimObjectData(): no matching request id found" << std::endl;
  return false;
}

bool DataManager::shutdown() {
  isInitialized = false;
  std::cout << "DataManager::shutdown()" << std::endl;
  return true;
}

void DataManager::requestData() {
  DWORD cbData;
  SIMCONNECT_RECV* ptrData;
  while (SUCCEEDED(SimConnect_GetNextDispatch(hSimConnect, &ptrData, &cbData))) {
    processDispatchMessage(ptrData, &cbData);
  }
}

NamedVariablePtr DataManager::make_named_var(
  const std::string &varName,
  Unit unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {
  // TODO - check if variable already exists
  std::shared_ptr<NamedVariable> var =
    std::make_shared<NamedVariable>(varName, unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks);
  variables[var->getVarName()] = var;
  return var;
}

AircraftVariablePtr DataManager::make_aircraft_var(
  const std::string &varName,
  int index,
  std::string setterEventName,
  EventPtr setterEvent,
  Unit unit,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {

  // TODO - check if variable already exists and if use the faster updating one
  std::shared_ptr<AircraftVariable> var =
    std::make_shared<AircraftVariable>(
      varName, index, std::move(setterEventName), std::move(setterEvent),
      unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks);
  const std::string &fullName = var->getVarName() + ":" + std::to_string(index);
  variables[fullName] = var;
  return var;
}

std::shared_ptr<AircraftVariable>
DataManager::make_simple_aircraft_var(
  const std::string &varName,
  Unit unit,
  bool autoReading,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {

  // TODO - check if variable already exists and if use the faster updating one
  std::shared_ptr<AircraftVariable> var =
    std::make_shared<AircraftVariable>(
      varName, 0, "", nullptr, unit, autoReading, false, maxAgeTime, maxAgeTicks);
  const std::string &fullName = var->getVarName() + ":" + std::to_string(0);
  variables[fullName] = var;
  return var;
}

DataDefinitionVariablePtr DataManager::make_datadefinition_var(
  const std::string &name,
  std::vector<DataDefinitionVariable::DataDefinition> &dataDefinitions,
  void* dataStruct,
  size_t dataStructSize,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {
  std::shared_ptr<DataDefinitionVariable> var =
    std::make_shared<DataDefinitionVariable>(
      hSimConnect,
      name,
      dataDefinitions,
      dataDefIDGen.getNextId(),
      dataReqIDGen.getNextId(),
      dataStruct,
      dataStructSize,
      autoReading,
      autoWriting,
      maxAgeTime,
      maxAgeTicks);

  dataDefinitionVariables.push_back(var);
  return var;
}


std::shared_ptr<Event> DataManager::make_event(const std::string &eventName) {
  if (events.find(eventName) != events.end()) {
    return events[eventName];
  }
  std::shared_ptr<Event> event = std::make_shared<Event>(hSimConnect, eventName, eventIDGen.getNextId());
  events[eventName] = event;
  return event;
}

// =================================================================================================
// Private methods
// =================================================================================================

void DataManager::processDispatchMessage(SIMCONNECT_RECV* pRecv, [[maybe_unused]] DWORD* cbData) {
  switch (pRecv->dwID) {

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
      processSimObjectData(static_cast<SIMCONNECT_RECV_SIMOBJECT_DATA*>(pRecv));
      break;

    case SIMCONNECT_RECV_ID_OPEN:
      std::cout << "DataManager: SimConnect connection established" << std::endl;
      break;

    case SIMCONNECT_RECV_ID_QUIT:
      std::cout << "DataManager: Received SimConnect connection quit message" << std::endl;
      break;

    case SIMCONNECT_RECV_ID_EXCEPTION:
      std::cerr << "DataManager: Exception in SimConnect connection: ";
      std::cerr << SimconnectExceptionStrings::getSimConnectExceptionString(static_cast<SIMCONNECT_EXCEPTION>(static_cast<SIMCONNECT_RECV_EXCEPTION*>(pRecv)->dwException));
      break;

    default:
      break;
  }
}


