#!/bin/bash
# This clones the umappp repository and obtains its dependencies.
# We vendor everything to avoid problems with managing compile-
# time dependencies in parallel with the R package ecosystem.

# Usage: UMAPPP_VERSION=v3.2.0 vendor.sh

set -e
set -u

UMAPPP_VERSION=${UMAPPP_VERSION:-master}

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMP_DIR="${SCRIPT_DIR}/temp"
VENDOR_DIR="$(dirname "${SCRIPT_DIR}")/vendor"

# Pulling down the dependencies.
if [ ! -e "${TEMP_DIR}" ]
then
    git clone https://github.com/libscran/umappp "${TEMP_DIR}"
    cd "${TEMP_DIR}"
    git checkout ${UMAPPP_VERSION}
else
    cd "${TEMP_DIR}"
    git fetch
    git checkout ${UMAPPP_VERSION}
    if [ "${UMAPPP_VERSION}" = "master" ]; then
        git pull
    fi
fi
cmake -S . -B build -DBUILD_TESTING=OFF

# Cleaing out what was there before.
rm -rf ../include
mkdir ../include

# Copying over the headers.
cp -r include/umappp ../include/umappp

for x in aarand knncolle irlba subpar
do
    cp -r build/_deps/${x}-src/include/${x} ../include/${x}
done

cp -r build/_deps/eigen-src/Eigen ../include/Eigen

rm -rf "${VENDOR_DIR}"
cp -r ../include "${VENDOR_DIR}"