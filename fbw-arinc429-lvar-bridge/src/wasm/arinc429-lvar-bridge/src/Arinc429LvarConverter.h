// Copyright (c) 2023-2024 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_AIRCRAFT_ARINC429LVARCONVERTER_H
#define FLYBYWIRE_AIRCRAFT_ARINC429LVARCONVERTER_H

#include <vector>

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>
#include "SimpleProfiler.hpp"

static constexpr int     MAX_INDEX_LVAR_SCAN  = 99999;
static const std::string LVAR_PREFIX          = "A32NX_";
static const std::string ARINC429_LVAR_SUFFIX = "";

/**
 * @brief TODO
 */
class Arinc429LvarConverter {
  const std::string DEFAULT_VARS_FILE = "\\modules\\arinc429_vars.txt";
  const std::string WORK_VARS_FILE    = "\\work\\aring429_vars.txt";

 private:
  bool initialized = false;
  bool varsRead    = false;

  int64_t tickCounter = 0;

  ID isReadyID;
  ID isArinc429LvarBridgeOnID;
  ID doArinc429LvarBridgeInit;
  ID isArinc429LvarBridgeVerbose;

  std::vector<std::pair<int, int>> arinc429Vars;

  SimpleProfiler profiler{"Arinc429LVarConverter", 100};

 public:
  void init();
  void update();

 private:
  void readVarFile();
  void getAllLVarsFromSim();
  void registerConvertedVars(const std::string& line);
};

#endif  // FLYBYWIRE_AIRCRAFT_ARINC429LVARCONVERTER_H
