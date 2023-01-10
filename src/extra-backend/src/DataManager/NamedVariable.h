// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_NAMEDVARIABLE_H
#define FLYBYWIRE_A32NX_NAMEDVARIABLE_H

#include <string>

#include "CacheableVariable.h"

class NamedVariable: public CacheableVariable {

public:
  // clang-format off
  explicit NamedVariable(
      std::string varName,
      int varIndex = 0,
      ENUM unit = UNITS.Number,
      bool autoUpdate = false,
      const std::chrono::duration<int64_t, std::milli> &maxAgeTime = 0ms,
      int64_t maxAgeTicks = 0);
// clang-format on

  FLOAT64 getFromSim() override;
  void setToSim(FLOAT64 value) override;
  void writeToSim() override;
};

#endif // FLYBYWIRE_A32NX_NAMEDVARIABLE_H
