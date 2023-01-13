// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>

#include "DataManager.h"
#include "NamedVariable.h"
#include "SimconnectExceptionStrings.h"

inline constexpr bool DEBUG_DATA_MANAGER = false;

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
      if (DEBUG_DATA_MANAGER && tickCounter % 100 == 0) {
        std::cout << "DataManager::preUpdate() - auto read named and aircraft: "
        << var.second->getVarName()  << " = " << var.second->get()  << std::endl;
      }
    }
  }

  // request all data definitions set to automatically read
  for (auto &ddv: dataDefinitionVariables) {
    if (ddv->isAutoRead()) {
      if (!ddv->requestUpdateFromSim(timeStamp, tickCounter)) {
        std::cerr << "DataManager::preUpdate(): requestUpdateFromSim() failed for "
                  << ddv->getName() << std::endl;
      }
      if (DEBUG_DATA_MANAGER && tickCounter % 100 == 0) {
        std::cout << "DataManager::preUpdate() - auto read simobjects: "
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
      if (DEBUG_DATA_MANAGER && tickCounter % 100 == 0) {
        std::cout << "DataManager::postUpdate() - auto write named and aircraft: "
                  << var.second->getVarName()  << " = " << var.second->get()  << std::endl;
      }
    }
  }

  // write all data definitions set to automatically write
  for (auto &ddv: dataDefinitionVariables) {
    if (ddv->isAutoWrite()) {
      if (!ddv->updateToSim(timeStamp, tickCounter)) {
        std::cerr << "DataManager::postUpdate(): updateToSim() failed for "
                  << ddv->getName() << std::endl;
      }
      if (DEBUG_DATA_MANAGER && tickCounter % 100 == 0) {
        std::cout << "DataManager::postUpdate() - auto write simobjects"
                  << ddv->getName()  << std::endl;
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

  // Check if variable already exists
  // TODO: change the variable so that the shortest required update and write
  //  times are used
  if (variables.find(varName) != variables.end()) {
    return std::dynamic_pointer_cast<NamedVariable>(variables[varName]);
  }

  // Create new var and store it in the map
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

  // Check if variable already exists
  // TODO: change the variable so that the shortest required update and write
  //  times are used
  const std::string fullName = varName + ":" + std::to_string(0);
  if (variables.find(fullName) != variables.end()) {
    return std::dynamic_pointer_cast<AircraftVariable>(variables[fullName]);
  }

  // Create new var and store it in the map
  std::shared_ptr<AircraftVariable> var =
    std::make_shared<AircraftVariable>(
      varName, index, std::move(setterEventName), std::move(setterEvent),
      unit, autoReading, autoWriting, maxAgeTime, maxAgeTicks);
  variables[fullName] = var;

  return var;
}

AircraftVariablePtr DataManager::make_simple_aircraft_var(
  const std::string &varName,
  Unit unit,
  bool autoReading,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks) {

  // Check if variable already exists
  // TODO: change the variable so that the shortest required update and write
  //  times are used
  const std::string fullName = varName + ":" + std::to_string(0);
  if (variables.find(fullName) != variables.end()) {
    return std::dynamic_pointer_cast<AircraftVariable>(variables[fullName]);
  }

  // Create new var and store it in the map
  AircraftVariablePtr var =
    std::make_shared<AircraftVariable>(
      varName, 0, "", nullptr, unit, autoReading, false, maxAgeTime, maxAgeTicks);
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
  DataDefinitionVariablePtr var =
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


EventPtr DataManager::make_event(const std::string &eventName) {
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


