#!/bin/bash

set -e

cd ..

python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt

export WORKIND_DIR=$(pwd)

cd binaries

./SyncWatchers & python3 ${WORKIND_DIR}/core_tests.py
