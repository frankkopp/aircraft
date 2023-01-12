// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#include "Module.h"
#include "MsfsHandler.h"
#include "DataManager.h"

Module::Module(MsfsHandler *backRef) : msfsHandler(backRef){
  msfsHandler->registerModule(this);
}
