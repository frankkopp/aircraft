// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include "Event.h"

// define/instantiate the state id generator
IDGenerator Event::idGenerator{};

Event::Event(HANDLE hdlSimConnect, const std::string &eventName)
  : hSimConnect(hdlSimConnect), eventName(eventName) {

  eventClientID = Event::idGenerator.getNextId();
  std::cout << "Event::Event(): " << eventName << " has client ID " << eventClientID << std::endl;
  if (!SUCCEEDED(SimConnect_MapClientEventToSimEvent(hSimConnect, eventClientID, eventName.c_str()))) {
    std::cerr << "Failed to map event " << eventName << " to client event " << eventClientID
              << std::endl;
  }
}

bool Event::trigger(DWORD data0, DWORD data1, DWORD data2, DWORD data3, DWORD data4) const {
  return SUCCEEDED(SimConnect_TransmitClientEvent_EX1(
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
}
