// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

pub struct ProcedureStep {
    // human readable name for the step
    description: String,
    // unique id for each step (will be assigned automatically in constructor)
    id: i64,
    // true if the procedure step is a pure condition check to wait for a certain state
    is_conditional: bool,
    // time to delay next step of execution of action - will be skipped if
    // expected state is already set
    delay_after: f64,
    // check if desired state is already set so the action can be skipped
    expected_state_code: String,
    // calculator code to achieve the desired state
    // if it is a conditional this calculator code needs to eval to true or false
    action_code: String,
}

pub struct AircraftProcedures {}
