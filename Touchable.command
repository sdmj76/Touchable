#!/bin/bash
# Touchable launcher. Keep all implementation in internal/.
set -euo pipefail
TOUCHABLE_ROOT=$(cd "$(dirname "$0")" && pwd)
cd "$TOUCHABLE_ROOT"
export PYTHONDONTWRITEBYTECODE=1
export PYTHONUNBUFFERED=1
finish() {
    local status=$?
    if [[ -t 0 ]]; then
        if [[ $status -ne 0 ]]; then echo 'The operation failed. Read the messages above and the logs.'; fi
        read -r -p 'Press Return to close this window.' _ || true
    fi
}
trap finish EXIT
if [[ $(uname -m) != arm64 ]]; then
    echo 'An Apple Silicon Mac is necessary. Use a native Terminal, not Rosetta.' >&2
    exit 1
fi
if ! xcrun --sdk iphoneos --show-sdk-path >/dev/null 2>&1; then
    echo 'Install and open the full Xcode first. Select the Xcode Command Line Tools.' >&2
    exit 1
fi
TOUCHABLE_REQUIREMENTS=$(shasum -a 256 internal/requirements.txt | awk '{print $1}')
if [[ ! -x .touchable/venv/bin/python || ! -f .touchable/venv/.touchable-ready ]] || \
   [[ $(cat .touchable/venv/.touchable-ready 2>/dev/null) != "$TOUCHABLE_REQUIREMENTS" ]]; then
    echo 'First run: preparing local tools. A network connection is necessary.'
    mkdir -p .touchable/logs
    if ! bash internal/tools/bootstrap.sh > .touchable/logs/bootstrap.log 2>&1; then
        echo 'Tool preparation failed. Read .touchable/logs/bootstrap.log.' >&2
        exit 1
    fi
fi
.touchable/venv/bin/python internal/tools/launcher.py "$@"
