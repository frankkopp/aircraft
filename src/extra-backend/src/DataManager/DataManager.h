// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_DATAMANAGER_H
#define FLYBYWIRE_A32NX_DATAMANAGER_H

#include <MSFS/Legacy/gauges.h>
#include <SimConnect.h>

class DataManager {
private:
public:
  DataManager();

  bool initialize();

  bool preUpdate(sGaugeDrawData *pData);
  bool update(sGaugeDrawData *pData);
  bool postUpdate(sGaugeDrawData *pData);

  bool processSimObjectData(SIMCONNECT_RECV_SIMOBJECT_DATA *pData);

  bool shutdown();

  // factory for cacheable variables

  // factory for data defintions
};

#endif // FLYBYWIRE_A32NX_DATAMANAGER_H
