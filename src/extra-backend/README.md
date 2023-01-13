# FlybyWire Simulations - C++ WASM framework

A lightweight framework to abstract the most common aspects when developing
C++ WASM modules using the MSFS SDK and SimConnect.

## Purpose

The purpose of this framework is to provide a lightweight abstraction layer
to the MSFS SDK and SimConnect. This allows developers to focus on the
implementation of the actual module without having to worry about the
boilerplate code required to get the module up and running.

## Goals

This framework will not cover all aspects of MSFS SDK or SimConnect, but it will
also not limit the developer to use the full SDK or SimConnect directly.
The goal is to make easy things easy and hard things possible.

It is not aimed at any specific use case or systems - it does not abstract the
aircraft or its systems. This will be done in the actual modules.

Helps new developers to get started with C++ WASM development in the FlyByWire
Code base without an overwhelming incomprehensible framework.

Continuously improve the framework to make it easier to use and more powerful
for additional use cases without making it overly complex.

## Overview and Features

## Components

### Gauge

### MsfsHandler

### DataManager

#### Variables
##### Named Variables
##### Aircraft Variables

#### Data Definition / Custom SimObjects

#### Client Data Area Definition / Custom SimObjects
[Not yet implemented]

#### Events

## Adding/Using MsfsHandler and Modules

## Using DataManager

## Building

## ToDo
- [ ] Documentation :)
- [ ] Implement receiving events including registering callbacks into Modules
- [ ] Implement additional SimConnect data types (Client Data Area)
-
- Ideas:
  - [ ] Use better build tools (CMake, Ninja, etc.) if build times become too long.
  - [ ] Add Unit Test framework
  - [ ] Add Loggin Framework]
