#!/usr/bin/env bash

set -eu -o pipefail

ARCH=$(uname -m)
REPO="YuriD/xkb-layout"
TAG="${TAG:-}"

case "$ARCH" in
    x86_64)
        FILE_ARCH="x86_64"
        ;;
    aarch64|arm64)
        FILE_ARCH="arm64"
        ;;
    *)
        echo "Not supported architecture: $ARCH"
        exit 1
        ;;
esac

if [ "$TAG" = "" ]; then
    TAG=$(curl -s "https://codeberg.org/api/v1/repos/$REPO/releases/latest" | awk -F'"tag_name": *"' '{print $2}' | awk -F'"' '{print $1}')
    if [ "$TAG" = "" ]; then
        echo "Error: Unable to retrieve the tag. Please check your connection or enter the tag manually."
        exit 1
    fi
fi

FILENAME="xkb-layout-$FILE_ARCH"
URL="https://codeberg.org/$REPO/releases/download/$TAG/$FILENAME"
INSTALL_DIR="$HOME/.local/bin"

mkdir -p "$INSTALL_DIR"

if ! curl -L --fail "$URL" -o "$INSTALL_DIR/xkb-layout"; then
    echo "Error: The file could not be downloaded. The tag '$TAG' or the architecture '$FILE_ARCH' may not exist."
    exit 1
fi

chmod +x "$INSTALL_DIR/xkb-layout"
echo "Successfully installed: $INSTALL_DIR/xkb-layout (version $TAG)"
