// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
#define FLYBYWIRE_A32NX_LIGHTINGPRESETS_H

#include "MsfsHandler/Module.h"

class MsfsHandler;

class LightingPresets : public Module {

public:
  explicit LightingPresets(MsfsHandler *msfsHandler);
  bool initialize() override;
  bool preUpdate(sGaugeDrawData *pData) override;
  bool update(sGaugeDrawData *pData) override;
  bool postUpdate(sGaugeDrawData *pData) override;
  bool shutdown() override;
};

#endif // FLYBYWIRE_A32NX_LIGHTINGPRESETS_H
