// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

extern crate systems;

mod aircraft_presets;
mod aircraft_procedures;
mod extra_backend;

use extra_backend::ExtraBackend;
use std::error::Error;
use systems_wasm::{MsfsSimulationBuilder, Variable};

#[msfs::gauge(name=extra_backend)]
async fn extra_backend(mut gauge: msfs::Gauge) -> Result<(), Box<dyn Error>> {
    let mut sim_connect = gauge.open_simconnect("extra_backend")?;

    let key_prefix = "A32NX_";
    let (mut simulation, mut handler) = MsfsSimulationBuilder::new(
        key_prefix,
        Variable::named(&format!("{}START_STATE", key_prefix)),
        sim_connect.as_mut().get_mut(),
    )
    .provides_aircraft_variable("SIM ON GROUND", "Bool", 0)?
    .build(ExtraBackend::new)?;

    while let Some(event) = gauge.next_event().await {
        handler.handle(event, &mut simulation, sim_connect.as_mut().get_mut())?;
    }

    Ok(())
}
