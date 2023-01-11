// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

#ifndef FLYBYWIRE_A32NX_SIMPLE_ASSERT_H
#define FLYBYWIRE_A32NX_SIMPLE_ASSERT_H

#include <iostream>

/**
 * This macro is used to assert a condition. As Microsoft Flight Simulator does
 * not support exceptions, this macro will only print an error message to the console.
 */
#define SIMPLE_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "Assertion failed: " << message << std::endl; \
    }

#endif //FLYBYWIRE_A32NX_SIMPLE_ASSERT_H
