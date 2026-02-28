#!/bin/bash

# -----------------------------
# Setup
# -----------------------------
PROJ_NAME="checkmate"
MODE="$1"
ARG="$2"

# -----------------------------
# Check mode
# -----------------------------
if [ "$MODE" == "check" ]; then
    if [ -z "$ARG" ]; then
        echo "You must provide a file to check!"
        exit 1
    fi
    echo "Linting $ARG..."
    gcc -Wall -Wextra -Wpedantic -fsyntax-only "$ARG"
    exit 0
fi

# -----------------------------
# Compile mode
# -----------------------------
if [ "$MODE" == "compile" ]; then
    echo "Compiling project..."
    make
    exit 0
fi

# -----------------------------
# Run mode
# -----------------------------
if [ "$MODE" == "run" ]; then
    if [ ! -f "$PROJ_NAME" ]; then
        echo "Error: $PROJ_NAME not found!"
        exit 1
    fi
    echo "Running $PROJ_NAME..."
    ./"$PROJ_NAME" "$ARG"
    exit 0
fi

# -----------------------------
# Build static library (.a)
# -----------------------------
if [ "$MODE" == "liba" ]; then
    echo "Building static library lib$PROJ_NAME.a..."
    make slib
    exit 0
fi

# -----------------------------
# Build Linux shared library (.so)
# -----------------------------
if [ "$MODE" == "libso" ]; then
    echo "Building Linux shared library lib$PROJ_NAME.so..."
    make linuxdlib
    exit 0
fi

# -----------------------------
# Invalid mode
# -----------------------------
echo "Invalid mode! Use: check, compile, run, liba, or libso"
exit 1