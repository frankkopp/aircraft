// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_MSFSHANDLER_H
#define FLYBYWIRE_A32NX_MSFSHANDLER_H

#include <MSFS/Legacy/gauges.h>
#include <MSFS/MSFS.h>
#include <MSFS/MSFS_Render.h>
#include <SimConnect.h>

#include <vector>

#include "DataManager/DataManager.h"

class Module;

class MsfsHandler {
  std::vector<Module *> modules{};
  DataManager dataManager{};

  std::string simConnectName;

  HANDLE hSimConnect{};

  bool isInitialized = false;

public:
  explicit MsfsHandler(std::string name);

  bool initialize();
  bool update(sGaugeDrawData *pData);
  bool shutdown();

  void registerModule(Module *pModule);

private:
  bool initializeSimConnect();
};

#endif // FLYBYWIRE_A32NX_MSFSHANDLER_H
