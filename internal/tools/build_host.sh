#!/bin/bash
set -euo pipefail
PROJECT_ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$PROJECT_ROOT"
if [[ -x ../.touchable/venv/bin/python ]]; then
    exec ../.touchable/venv/bin/python tools/build_host.py "$@"
fi
exec python3 tools/build_host.py "$@"
