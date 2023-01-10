// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_DATAMANAGER_H
#define FLYBYWIRE_A32NX_DATAMANAGER_H

#include <vector>
#include <memory>
#include <map>

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>

#include "Units.h"
#include "NamedVariable.h"

class DataManager {
private:
  std::map<std::string, std::shared_ptr<CacheableVariable>> variables{};

  HANDLE hSimConnect{};

  bool isInitialized = false;

public:
  DataManager();

  bool initialize();

  bool preUpdate(sGaugeDrawData *pData);
  bool update(sGaugeDrawData *pData);
  bool postUpdate(sGaugeDrawData *pData);

  bool processSimObjectData(SIMCONNECT_RECV_SIMOBJECT_DATA *pData);

  bool shutdown();

  // factory for cacheable variables
  // clang-format off
  template <typename T>
  std::shared_ptr<NamedVariable>  make_var(
      std::string nameInSim,
      int index = 0,
      ENUM unit = UNITS.Number,
      bool autoUpdate = false,
      const std::chrono::duration<int64_t, std::milli> &maxAgeTime = 0ms,
      int64_t maxAgeTicks = 0);
  // clang-format on

  // factory for data defintions
};

#endif // FLYBYWIRE_A32NX_DATAMANAGER_H
