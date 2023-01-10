#!/bin/bash

# get directory of this script relative to root
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

OUTPUT="${DIR}/../../flybywire-aircraft-a320-neo/SimObjects/AirPlanes/FlyByWire_A320_NEO/panel/extra-backend.wasm"

if [ "$1" == "--debug" ]; then
  CLANG_ARGS="-g -DDEBUG"
else
  WASMLD_ARGS="--strip-debug"
fi

set -ex

# create temporary folder for o files
mkdir -p "${DIR}/obj"
pushd "${DIR}/obj"

# compile c++ code
clang++ \
  -c \
  "${CLANG_ARGS}" \
  -std=c++20 \
  -Wno-unused-command-line-argument \
  -Wno-ignored-attributes \
  -Wno-macro-redefined \
  --sysroot "${MSFS_SDK}/WASM/wasi-sysroot" \
  -target wasm32-unknown-wasi \
  -flto \
  -D_MSFS_WASM=1 \
  -D__wasi__ \
  -D_LIBCPP_HAS_NO_THREADS \
  -D_WINDLL \
  -D_MBCS \
  -mthread-model single \
  -fno-exceptions \
  -fms-extensions \
  -fvisibility=hidden \
  -I "${MSFS_SDK}/WASM/include" \
  -I "${MSFS_SDK}/SimConnect SDK/include" \
  -I "${DIR}/src" \
  -I "${DIR}/src/lib" \
  -I "${DIR}/src/MsfsHandler" \
  -I "${DIR}/src/DataManager" \
  -I "${DIR}/src/LightingPresets" \
  "${DIR}/src/Gauge_Extra_Backend.cpp" \
  "${DIR}/src/MsfsHandler/MsfsHandler.cpp" \
  "${DIR}/src/MsfsHandler/Module.cpp" \
  "${DIR}/src/DataManager/DataManager.cpp" \
  "${DIR}/src/DataManager/CacheableVariable.cpp" \
  "${DIR}/src/DataManager/NamedVariable.cpp" \
  "${DIR}/src/DataManager/AircraftVariable.cpp" \
  "${DIR}/src/LightingPresets/LightingPresets.cpp"

# restore directory
popd

wasm-ld \
  --no-entry \
  --allow-undefined \
  -L "${MSFS_SDK}/WASM/wasi-sysroot/lib/wasm32-wasi" \
  -lc "${MSFS_SDK}/WASM/wasi-sysroot/lib/wasm32-wasi/libclang_rt.builtins-wasm32.a" \
  --export __wasm_call_ctors \
  ${WASMLD_ARGS} \
  --export-dynamic \
  --export malloc \
  --export free \
  --export __wasm_call_ctors \
  --export-table \
  --gc-sections \
  -O3 --lto-O3 \
  -lc++ -lc++abi \
  ${DIR}/obj/*.o \
  -o $OUTPUT
