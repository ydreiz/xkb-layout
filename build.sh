#!/bin/bash

BUILD_DIR="build"
BINARY="xkb-layout"
SOURCE="main.c"
CC="gcc"
FLAGS="-Wall -Wextra -pedantic -ggdb"
LIBS="-lX11"

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    echo "Cannot determine OS type."
    exit 1
fi

case "$OS" in
    ubuntu|debian)
        PKG_MANAGER="sudo apt install -y"
        DEPS=("build-essential" "libx11-dev")
        CHECK_CMD="dpkg -s"
        ;;
    fedora)
        PKG_MANAGER="sudo dnf install -y"
        DEPS=("gcc" "make" "libX11-devel")
        CHECK_CMD="rpm -q"
        ;;
    arch)
        PKG_MANAGER="sudo pacman -S --noconfirm"
        DEPS=("base-devel" "libx11")
        CHECK_CMD="pacman -Qi"
        ;;
    *)
        echo "Unsupported OS: $OS"
        exit 1
        ;;
esac

MISSING=()
for pkg in "${DEPS[@]}"; do
    if ! $CHECK_CMD "$pkg" >/dev/null 2>&1; then
        MISSING+=("$pkg")
    fi
done

if [ ${#MISSING[@]} -gt 0 ]; then
    echo "Missing packages: ${MISSING[*]}"
    echo "Installing missing dependencies..."
    $PKG_MANAGER "${MISSING[@]}"
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
fi

if $CC $FLAGS "$SOURCE" -o "$BINARY" $LIBS; then
    chmod +x "$BINARY"
    echo "Build successful: ./$BINARY"
else
    echo "Build failed."
    exit 1
fi
