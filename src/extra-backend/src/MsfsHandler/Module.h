// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_MODULE_H
#define FLYBYWIRE_A32NX_MODULE_H

#include <MSFS/Legacy/gauges.h>
class MsfsHandler;

class Module {
protected:
  MsfsHandler *msfsHandler;

  bool isInitialized = false;

public:
  explicit Module(MsfsHandler *msfsHandler);
  virtual bool initialize() = 0;
  virtual bool preUpdate(sGaugeDrawData *pData) = 0;
  virtual bool update(sGaugeDrawData *pData) = 0;
  virtual bool postUpdate(sGaugeDrawData *pData) = 0;
  virtual bool shutdown() = 0;
};

#endif // FLYBYWIRE_A32NX_MODULE_H
