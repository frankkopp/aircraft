// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include "Event.h"

// define/instantiate the state id generator
IDGenerator Event::idGenerator{};

Event::Event(HANDLE hdlSimConnect, const std::string &eventName)
  : hSimConnect(hdlSimConnect), eventName(eventName) {

  eventClientID = Event::idGenerator.getNextId();
  if (!SUCCEEDED(SimConnect_MapClientEventToSimEvent(hSimConnect, eventClientID, eventName.c_str()))) {
    std::cerr << "Failed to map event " << eventName << " to client event " << eventClientID
              << std::endl;
  }
}

void Event::trigger_ex1(DWORD data0, DWORD data1, DWORD data2, DWORD data3, DWORD data4) const {
  const bool result = SUCCEEDED(SimConnect_TransmitClientEvent_EX1(
    hSimConnect,
    SIMCONNECT_OBJECT_ID_USER,
    eventClientID,
    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY,
    data0,
    data1,
    data2,
    data3,
    data4));
  if (!result) {
    std::cerr << "Failed to trigger_ex1 event " << eventName << " with client event " << eventClientID
              << std::endl;
  }
}

void Event::trigger(DWORD data0) const {
  const bool result = SUCCEEDED(SimConnect_TransmitClientEvent(
    hSimConnect,
    SIMCONNECT_OBJECT_ID_USER,
    eventClientID,
    data0,
    SIMCONNECT_GROUP_PRIORITY_HIGHEST,
    SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY));
  if (!result) {
    std::cerr << "Failed to trigger event " << eventName << " with client event " << eventClientID
              << std::endl;
  }
}
