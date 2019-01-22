#!/bin/bash

set -ex

cmake=("cmake")
need_stop=false
if [ "${TRAVIS_OS_NAME}" = "osx" ];
then
    generator="-GXcode"
    CORES=4 # Apple toolchain can't figure out the number of cores...
elif [ "${TRAVIS_OS_NAME}" = "windows" ];
then
    generator="-GVisual Studio 15 2017 Win64"
else
    generator="-GNinja"
    cmake=(docker exec ${RUNNER} cmake)
    need_stop=true
fi

build_dir="$(pwd)/build"

# Configure
${cmake[@]} -H"$(pwd)" -B"${build_dir}" "${generator}" -DCMAKE_BUILD_TYPE=Release

# Compile
${cmake[@]} --build "${build_dir}" --config Release -j ${CORES:-}

# Stop docker image for linux machine
if ${need_stop};
then
     docker stop "${RUNNER}"
fi