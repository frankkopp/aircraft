/* eslint-disable no-underscore-dangle */
// Copyright (c) 2022 FlyByWire Simulations
// SPDX-License-Identifier: GPL-3.0

export class MSFSFlightPlanLoader {
    static fpListenerInitialized = false;

    static rawData: Record<string, any> = {};

    public static init() {
        if (!this.fpListenerInitialized) {
            RegisterViewListener('JS_LISTENER_FLIGHTPLAN');
            this.fpListenerInitialized = true;
        }
    }

    public static async LoadFromGame(): Promise<void> {
        return new Promise((resolve) => {
            this.init();
            Coherent.call('LOAD_CURRENT_GAME_FLIGHT').catch(console.error);
            Coherent.call('LOAD_CURRENT_ATC_FLIGHTPLAN').catch(console.error);
            setTimeout(() => {
                Coherent.call('GET_FLIGHTPLAN').then(async (data: Record<string, any>) => {
                    this.rawData = data;
                    resolve();
                }).catch(console.error);
            }, 100);
        });
    }
}
