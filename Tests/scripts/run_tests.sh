#!/bin/bash

set -e

cd ..

python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt

export WORKIND_DIR=$(pwd)

cd binaries/bin

export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

./SyncWatchers & python3 ${WORKIND_DIR}/core_tests.py
