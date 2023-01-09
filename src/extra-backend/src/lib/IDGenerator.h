// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

//
// Created by frank on 09.01.2023.
//

#ifndef FLYBYWIRE_A32NX_IDGENERATOR_H
#define FLYBYWIRE_A32NX_IDGENERATOR_H

class IDGenerator {
private:
  int nextId = 1;

public:
  inline int getNextId() { return nextId++; };
};

#endif // FLYBYWIRE_A32NX_IDGENERATOR_H
