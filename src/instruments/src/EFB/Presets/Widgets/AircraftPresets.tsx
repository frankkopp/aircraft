// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

/* eslint-disable max-len */
import React from 'react';
import { useSimVar } from '@instruments/common/simVars';
import { toast } from 'react-toastify';
import { ScrollableContainer } from '../../UtilComponents/ScrollableContainer';

export const AircraftPresets = () => {
    // Aircraft presets are handled by a backend WASM module. This frontend will
    // use the LVAR A32NX_LOAD_AIRCRAFT_PRESET to signal the backend that the user
    // requests a preset to be loaded.
    // The backend will reset the LVAR to 0 when done.
    // As long as the LVAR is 1 the backend is still applying the preset.
    // If the LVAR is set to 0 before the backend is finished applying the preset
    // will be stopped by the backend.

    const [, setLoadPresetVar] = useSimVar('L:A32NX_LOAD_AIRCRAFT_PRESET', 'number', 200);

    // Sets the LVAR to tell the wasm to load the preset into the aircraft
    function loadPreset(presetID: number, presetName: string) {
        setLoadPresetVar(presetID);
        toast.success(`Loading Preset: ${presetID}: ${presetName}`, { autoClose: 250, hideProgressBar: true, closeButton: false });
    }

    return (
        <div className="w-full">
            <div className="flex flex-row items-end space-x-4">
                <h1 className="font-bold">Aircraft</h1>
            </div>
            <div className="p-4 mt-4 rounded-lg border-2 border-theme-accent">
                <ScrollableContainer height={52}>
                    <div className="grid grid-cols-1 grid-rows-3 grid-flow-row gap-4">
                        <div
                            className="flex justify-center items-center my-1 mx-1 h-24 text-theme-text hover:text-theme-body bg-theme-accent hover:bg-theme-highlight rounded-md border-2 border-theme-accent transition duration-100"
                            onClick={() => loadPreset(1, 'Cold & Dark')}
                        >
                            Cold & Dark
                        </div>

                        <div
                            className="flex justify-center items-center my-1 mx-1 h-24 text-theme-text hover:text-theme-body bg-theme-accent hover:bg-theme-highlight rounded-md border-2 border-theme-accent transition duration-100"
                            onClick={() => loadPreset(2, 'Ready for Pushback')}
                        >
                            Ready for Pushback
                        </div>
                        <div
                            className="flex justify-center items-center my-1 mx-1 h-24 text-theme-text hover:text-theme-body bg-theme-accent hover:bg-theme-highlight rounded-md border-2 border-theme-accent transition duration-100"
                            onClick={() => loadPreset(3, 'Ready for Taxi')}
                        >
                            Ready for Taxi
                        </div>
                        <div
                            className="flex justify-center items-center my-1 mx-1 h-24 text-theme-text hover:text-theme-body bg-theme-accent hover:bg-theme-highlight rounded-md border-2 border-theme-accent transition duration-100"
                            onClick={() => loadPreset(4, 'Ready for Takeoff')}
                        >
                            Ready for Takeoff
                        </div>
                        <div
                            className="flex justify-center items-center my-1 mx-1 h-24 text-theme-text hover:text-theme-body bg-theme-accent hover:bg-theme-highlight rounded-md border-2 border-theme-accent transition duration-100"
                            onClick={() => loadPreset(5, 'Turnaround')}
                        >
                            Turnaround
                        </div>

                    </div>
                </ScrollableContainer>
            </div>
        </div>
    );
};
