#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

BUILD_DIR="$REPO_ROOT/tests/build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

qmake CONFIG+=debug "$REPO_ROOT/tests/tests.pro"
make -j"$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)"

QT_QPA_PLATFORM=offscreen ./tests "$@"
