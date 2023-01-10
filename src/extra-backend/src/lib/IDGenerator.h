// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_IDGENERATOR_H
#define FLYBYWIRE_A32NX_IDGENERATOR_H

/**
 * This class is used to generate unique IDs for the modules.
 * Uniqueness is only guaranteed within the same instance of this class.
 * It is used to identify the modules in the MSFS gauges system.
 */
class IDGenerator {
private:
  int nextId = 1;

public:
  inline int getNextId() { return nextId++; };
};

#endif // FLYBYWIRE_A32NX_IDGENERATOR_H
