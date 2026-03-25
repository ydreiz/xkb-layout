# xkb-layout

Small C utility that prints the current XKB keyboard layout on Linux/X11.

## Table of contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Quick install (release binary)](#quick-install-release-binary)
- [Build from source](#build-from-source)
- [Run](#run)
- [Project structure](#project-structure)
- [Release process](#release-process)
- [Contributing](#contributing)
- [License](#license)

## About

`xkb-layout` connects to the X server, reads keyboard state and XKB properties,
and prints the active layout for the current keyboard group.

It is designed for scripts, status bars, and terminal usage where a short plain
text output is needed.

## Features

- Reads active keyboard group via XKB (`XkbGetState`)
- Parses `_XKB_RULES_NAMES` from the root X11 window
- Prints only the active layout (for example `us`, `ru`)
- Supports local build/install via `make`
- Supports release-binary install via `scripts/install.sh`

## Requirements

### Runtime

- Linux with X11

### Build from source

- GCC and `make`
- X11 development headers and libraries (`libX11`)

### Dependencies by distro

#### Debian/Ubuntu

```bash
sudo apt update
sudo apt install build-essential libx11-dev
```

#### Fedora

```bash
sudo dnf install gcc make libX11-devel
```

#### Arch Linux

```bash
sudo pacman -S base-devel libx11
```

## Quick install (release binary)

Install latest GitHub release to `~/.local/bin/xkb-layout`:

```bash
curl -fsSL https://raw.githubusercontent.com/ydreiz/xkb-layout/refs/heads/main/scripts/install.sh | bash
```

Install a specific version:

```bash
TAG=v1.0.0 curl -fsSL https://raw.githubusercontent.com/ydreiz/xkb-layout/refs/heads/main/scripts/install.sh | bash
```

Uninstall:

```bash
curl -fsSL https://raw.githubusercontent.com/ydreiz/xkb-layout/refs/heads/main/scripts/uninstall.sh | bash
```

Supported release architectures: `x86_64`, `arm64`.

## Build from source

### Debug build (default)

```bash
make
```

Builds `xkb-layout` with debug symbols.

### Release build

```bash
make release
```

Builds with optimizations and strips symbols.

### Local install/uninstall

By default, install target is `~/.local/bin`.

```bash
make install
make uninstall
```

### Clean

```bash
make clean
```

## Run

From repository root:

```bash
./xkb-layout
```

If installed to `~/.local/bin` and PATH is configured:

```bash
xkb-layout
```

Example output:

```text
us
```

## Project structure

```text
.
├── main.c
├── Makefile
├── scripts/
│   ├── install.sh
│   └── uninstall.sh
└── .github/workflows/
    └── build-and-release.yml
```

## Release process

Pushing a tag matching `v*` triggers GitHub Actions to:

1. Build release binaries for `x86_64` and `arm64`
2. Pack each binary into a tar.gz artifact
3. Publish artifacts in a GitHub Release

## Contributing

This repository is primarily a C learning project, but improvements are welcome.

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Open a pull request

## License

This project is licensed under the MIT License. See `LICENSE` for details.
