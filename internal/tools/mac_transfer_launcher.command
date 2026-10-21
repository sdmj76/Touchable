#!/bin/bash
# The packager copies this file to the transfer package root.
set -euo pipefail
export PYTHONDONTWRITEBYTECODE=1
TRANSFER_ROOT=$(cd "$(dirname "$0")" && pwd)
cd "$TRANSFER_ROOT"
TRANSFER_GAME=__TRANSFER_GAME__
TRANSFER_PRODUCT=__TRANSFER_PRODUCT__
TRANSFER_MODE=${1:-}

finish() {
    local status=$?
    if [[ $status -ne 0 ]]; then
        echo
        echo 'Preparation failed. Read the errors above and README-FIRST.md.'
        if [[ "$TRANSFER_MODE" != '--check-only' && -t 0 ]]; then
            read -r -p 'Press Return to close this window.' _
        fi
    fi
}
trap finish EXIT

if [[ -n "$TRANSFER_MODE" && "$TRANSFER_MODE" != '--check-only' ]]; then
    echo 'Usage: Open-Game.command [--check-only]' >&2
    exit 1
fi
if [[ $(uname -m) != arm64 ]]; then
    echo 'An Apple Silicon Mac is necessary. Do not run this script through Rosetta.' >&2
    exit 1
fi
if ! xcrun --sdk iphoneos --show-sdk-path >/dev/null 2>&1; then
    echo 'The iPhone SDK was not found. Install and open the full Xcode first.' >&2
    echo 'Select the Command Line Tools in Xcode Settings > Locations.' >&2
    exit 1
fi
if [[ ! -x .touchable/venv/bin/python ]]; then
    echo 'Creating the local Python environment. No Python packages are downloaded.'
    xcrun python3 -m venv --without-pip .touchable/venv
fi
.touchable/venv/bin/python -c 'import sys; assert sys.version_info >= (3, 9), "Python 3.9 or later is required."'
echo 'Checking the transfer files.'
.touchable/venv/bin/python internal/tools/verify_mac_transfer.py --game "$TRANSFER_GAME"
.touchable/venv/bin/python internal/tools/games.py check --game "$TRANSFER_GAME"
echo 'Generating the Xcode project for the paths on this Mac.'
.touchable/venv/bin/python internal/tools/prepare_ios.py --game "$TRANSFER_GAME"
echo
echo 'In Xcode, select the original development team.'
echo 'Connect the iPhone and select it as the run destination.'
echo 'Keep the existing Bundle ID.'
if [[ "$TRANSFER_MODE" != '--check-only' ]]; then
    open -a Xcode ".touchable/build/games/$TRANSFER_GAME/ios-project/$TRANSFER_PRODUCT.xcodeproj"
fi
