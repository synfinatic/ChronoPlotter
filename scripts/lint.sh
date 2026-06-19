#!/usr/bin/env bash
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

SOURCES=(
    ChronoPlotter.cpp
    PowderTest.cpp
    SeatingDepthTest.cpp
    TunerTest.cpp
    About.cpp
    untar.cpp
)

exec cppcheck \
    --enable=warning,style,performance,portability \
    --suppress=missingInclude \
    "--suppress=syntaxError:qcustomplot/qcustomplot.h" \
    --error-exitcode=1 \
    --std=c++17 \
    -I. \
    -IQXlsx/header \
    "${SOURCES[@]}"
