#!/bin/bash

set -ex

command_prefix=()

# Install pre-requisites
if [ "${TRAVIS_OS_NAME}" = "osx" ];
then
    brew update || brew update
    brew install cmake || :;
elif [ ! "${TRAVIS_OS_NAME}" = "windows" ];
then
    # Start a pre-configured docker image
    # Images are picked from bincrafters sfml recipe
    docker pull ${DOCKER_IMAGE}
    docker run -v ${PWD}:${PWD} -w ${PWD}  \
               -u root \
               --name "${RUNNER}" \
               --rm -t -d ${DOCKER_IMAGE}
    command_prefix=(docker exec ${RUNNER})
fi

# Install Conan
if [ "${TRAVIS_OS_NAME}" = "windows" ];
then
    choco install conan || :;
    export PATH="${PATH}:/c/Program Files/Conan/conan";
else
    ${command_prefix[@]} pip install conan --upgrade;
fi

# Install dependencies using Conan
conan=(${command_prefix[@]} conan)
${conan[@]} remote add -f bincrafters https://api.bintray.com/conan/bincrafters/public-conan
${conan[@]} profile new default --detect  # Generates default profile detecting GCC and sets old ABI

if [ "${TRAVIS_OS_NAME}" = "osx" ];
then
    # AppleClang is a bit old, and most recent library is libc++
    # https://forums.developer.apple.com/thread/73004
    ${conan[@]} profile update settings.compiler.libcxx=libc++ default
elif [ "${TRAVIS_OS_NAME}" = "windows" ];
then
    # The "--detect" option finds gcc 8, so we set Visual Studio manually
    ${conan[@]} profile update settings.compiler="Visual Studio" default
    ${conan[@]} profile update settings.compiler.version=15 default

    # libcxx setting not used on windows
    ${conan[@]} profile remove settings.compiler.libcxx default
else
    # Sets libcxx to C++11 ABI
    ${conan[@]} profile update settings.compiler.libcxx=libstdc++11 default
fi

${conan[@]} install . --build missing