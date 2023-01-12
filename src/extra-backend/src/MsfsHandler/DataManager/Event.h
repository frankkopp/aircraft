// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 12.01.2023.
//

#ifndef FLYBYWIRE_A32NX_EVENT_H
#define FLYBYWIRE_A32NX_EVENT_H

#include <iostream>
#include <string>

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>
#include "IDGenerator.h"

/**
 * TODO: docs comments
 */
class Event {
private:

  HANDLE hSimConnect;

  const std::string eventName;
  const DWORD eventClientID;

public:

  Event() = delete; // no default constructor
  Event(const Event&) = delete; // no copy constructor
  Event& operator=(const Event&) = delete; // no copy assignment

  Event(HANDLE hdlSimConnect, const std::string &eventName, DWORD eventClientID);

  [[nodiscard]]
  void trigger_ex1(DWORD data0 = 0, DWORD data1 = 0, DWORD data2 = 0, DWORD data3 = 0,
                   DWORD data4 = 0) const;

  [[maybe_unused]] [[nodiscard]]
  void trigger(DWORD data0 = 0) const;

  // Getter and setter
public:
  [[maybe_unused]] [[nodiscard]]
  const std::string &getEventName() const { return eventName; }

  [[maybe_unused]] [[nodiscard]]
  DWORD getEventClientId() const { return eventClientID; }

};

#endif //FLYBYWIRE_A32NX_EVENT_H
