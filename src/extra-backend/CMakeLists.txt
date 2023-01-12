cmake_minimum_required(VERSION 3.19)
project(flybywire-a32nx-flypad-backend)

set(CMAKE_CXX_STANDARD 17)
set(MSFS_SDK "C:\\MSFS SDK")

set(CMAKE_CXX_FLAGS "-c -g -DDEBUG -Wno-unused-command-line-argument -Wno-ignored-attributes -Wno-macro-redefined --sysroot \"${MSFS_SDK}/WASM/wasi-sysroot\" -target wasm32-unknown-wasi -flto -D_MSFS_WASM=1 -D__wasi__ -D_LIBCPP_HAS_NO_THREADS -D_WINDLL -D_MBCS -mthread-model single -fno-exceptions -fms-extensions")

include_directories("${MSFS_SDK}/WASM/include")
include_directories("${MSFS_SDK}/WASM/wasi-sysroot/include")
include_directories("${MSFS_SDK}/SimConnect SDK/include")

include_directories(src)
include_directories(src/lib)
include_directories(src/MsfsHandler)
include_directories(src/MsfsHandler/DataManager)
include_directories(src/Modules)
include_directories(src/Modules/LightingPresets)
include_directories(src/Modules/Pushback)
include_directories(src/Modules/AircraftPresets)

add_executable(flybywire-a32nx-extra-backend
        src/lib/Units.h
        src/lib/simple_assert.h
        src/lib/IDGenerator.h
        src/MsfsHandler/DataManager/SimconnectExceptionStrings.h
        src/Gauge_Extra_Backend.cpp
        src/MsfsHandler/MsfsHandler.cpp src/MsfsHandler/MsfsHandler.h
        src/MsfsHandler/DataManager/DataManager.cpp src/MsfsHandler/DataManager/DataManager.h
        src/MsfsHandler/DataManager/CacheableVariable.cpp src/MsfsHandler/DataManager/CacheableVariable.h
        src/MsfsHandler/DataManager/NamedVariable.cpp src/MsfsHandler/DataManager/NamedVariable.h
        src/MsfsHandler/DataManager/AircraftVariable.cpp src/MsfsHandler/DataManager/AircraftVariable.h
        src/MsfsHandler/DataManager/DataDefinitionVariable.cpp src/MsfsHandler/DataManager/DataDefinitionVariable.h
        src/MsfsHandler/DataManager/ClientDataAreaVariable.cpp src/MsfsHandler/DataManager/ClientDataAreaVariable.h
        src/MsfsHandler/DataManager/Event.cpp src/MsfsHandler/DataManager/Event.h
        src/MsfsHandler/Module.cpp src/MsfsHandler/Module.h
        src/Modules/LightingPresets/LightingPresets.cpp src/Modules/LightingPresets/LightingPresets.h
        src/Modules/Pushback/Pushback.cpp src/Modules/Pushback/Pushback.h
        src/Modules/Pushback/InertialDampener.cpp src/Modules/Pushback/InertialDampener.h
        src/Modules/AircraftPresets/AircraftPresets.cpp src/Modules/AircraftPresets/AircraftPresets.h
        src/Modules/AircraftPresets/AircraftProcedures.h)
