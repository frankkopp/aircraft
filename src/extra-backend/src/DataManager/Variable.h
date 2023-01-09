// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_VARIABLE_H
#define FLYBYWIRE_A32NX_VARIABLE_H


#include <string>
#include <optional>

#include "Units.h"
#include <MSFS/Legacy/gauges.h>

class Variable {

protected:
  const std::string nameInSim;
  const int index;
  ENUM unit;

public:
  Variable() = delete;
  Variable(std::string nameInSim, int index=0, ENUM unit=UNITS.Number);

  virtual FLOAT64 getFromSim() = 0;
  virtual void setToSim(FLOAT64 value) = 0;

};

#endif // FLYBYWIRE_A32NX_VARIABLE_H
