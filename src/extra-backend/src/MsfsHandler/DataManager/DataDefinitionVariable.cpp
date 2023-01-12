// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include <iostream>
#include <utility>

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>

#include "Units.h"
#include "DataDefinitionVariable.h"
#include "simple_assert.h"

DataDefinitionVariable::DataDefinitionVariable(
  HANDLE hdlSimConnect,
  std::string varName,
  std::vector<DataDefinition> &dataDefinitions,
  ID dataDefinitionId,
  ID requestId,
  void* pDataStruct,
  size_t structSize,
  bool autoReading,
  bool autoWriting,
  FLOAT64 maxAgeTime,
  UINT64 maxAgeTicks)
  :
  hSimConnect(hdlSimConnect),
  name(std::move(varName)),
  dataDefinitions(dataDefinitions),
  dataDefId(dataDefinitionId),
  requestId(requestId),
  pDataStruct(pDataStruct),
  structSize(structSize),
  autoRead(autoReading),
  autoWrite(autoWriting),
  maxAgeTime(maxAgeTime),
  maxAgeTicks(maxAgeTicks) {

  // TODO: what happens if definition is wrong - will this cause a sim crash?
  //  Might need to move this out of the constructor and into a separate method

  SIMPLE_ASSERT(structSize == dataDefinitions.size() * sizeof(FLOAT64),
                "DataDefinitionVariable::updateFromSimObjectData: Struct size mismatch")

  for (auto &ddef: dataDefinitions) {
    std::string fullVarName = ddef.name;
    if (ddef.index != 0) {
      fullVarName += ":" + std::to_string(ddef.index);
    }

    if (!SUCCEEDED(SimConnect_AddToDataDefinition(
      hSimConnect,
      dataDefinitionId,
      fullVarName.c_str(),
      ddef.unit.name,
      SIMCONNECT_DATATYPE_FLOAT64))) {

      std::cerr << "Failed to add " << ddef.name << " to data definition." << std::endl;
    }
  }

}

bool DataDefinitionVariable::requestFromSim() const {
  if (!SUCCEEDED(SimConnect_RequestDataOnSimObject(
    hSimConnect,
    requestId,
    dataDefId,
    SIMCONNECT_OBJECT_ID_USER,
    SIMCONNECT_PERIOD_ONCE))) { // TODO: support PERIOD directly??

    std::cerr << "Failed to request data from sim." << std::endl;
    return false;
  }
  return true;
}

bool DataDefinitionVariable::requestUpdateFromSim(FLOAT64 timeStamp, UINT64 tickCounter) {
  //  std::cout << "Requesting update from sim for " << name << " with requestID=" << requestId << std::endl;
  //  std::cout << "Time stamp: " << timeStamp << " Tick counter: " << tickCounter << std::endl;
  //  std::cout << timeStampSimTime + maxAgeTime << " " << tickStamp + maxAgeTicks << std::endl;
  // only update if the value is equal or older than the max age for sim time ot ticks
  if (timeStampSimTime + maxAgeTime >= timeStamp || tickStamp + maxAgeTicks >= tickCounter) {
    return true;
  }
  timeStampSimTime = timeStamp;
  tickStamp = tickCounter;
  //  std::cout << "Requesting update from sim for " << name << " executing" << std::endl;
  if (!SUCCEEDED(SimConnect_RequestDataOnSimObject(
    hSimConnect,
    requestId,
    dataDefId,
    SIMCONNECT_OBJECT_ID_USER,
    SIMCONNECT_PERIOD_ONCE))) { // TODO: support PERIOD directly??

    std::cerr << "Failed to request data from sim." << std::endl;
    return false;
  }
  return true;
}

void DataDefinitionVariable::updateFromSimObjectData(const SIMCONNECT_RECV_SIMOBJECT_DATA* pData) {
  SIMPLE_ASSERT(structSize == pData->dwDefineCount * sizeof(FLOAT64),
                "DataDefinitionVariable::updateFromSimObjectData: Struct size mismatch")
  SIMPLE_ASSERT(pData->dwRequestID == requestId,
                "DataDefinitionVariable::updateFromSimObjectData: Request ID mismatch")
  std::memcpy(pDataStruct, &pData->dwData, structSize);
}

bool DataDefinitionVariable::writeToSim() {
  const bool result = SUCCEEDED(SimConnect_SetDataOnSimObject(
    hSimConnect, dataDefId, SIMCONNECT_OBJECT_ID_USER, 0, 0, structSize, pDataStruct));
  if (!result) {
    std::cerr << "Setting data to sim for " << name << " with dataDefId=" << dataDefId << " failed!" << std::endl;
  }
  return result;
}

bool DataDefinitionVariable::updateToSim(FLOAT64 timeStamp, UINT64 tickCounter) {
  if (writeToSim()) {
    timeStampSimTime = timeStamp;
    tickStamp = tickCounter;
    return true;
  }
  return false;
}


