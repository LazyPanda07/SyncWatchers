#!/bin/bash

set -e

cd ..

python3 -m pip install .

export WORKIND_DIR=$(pwd)

cd binaries

./SyncWatchers & python3 ${WORKIND_DIR}/core_test.py
