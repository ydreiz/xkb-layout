# xkb-layout

Small C utility that prints the current XKB keyboard layout on Linux/X11.

This repository used as a personal C practice project.

## Table of contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Build](#build)
- [Run](#run)
- [Project structure](#project-structure)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

## About

`xkb-layout` connects to the X server, reads keyboard state and XKB properties,
then prints the active layout for the current keyboard group.

The main goal of this repository is learning and practicing C in a real,
system-level context.

## Features

- Reads active keyboard group via XKB (`XkbGetState`)
- Parses `_XKB_RULES_NAMES` from the root X11 window
- Prints the active layout as plain text (for scripts and terminal use)

## Requirements

- Linux with X11
- GCC
- X11 development headers and libraries (`libX11`)
- `make` (optional for the alternative build method)

### Debian/Ubuntu

```bash
sudo apt update
sudo apt install build-essential libx11-dev
```

### Fedora

```bash
sudo dnf install gcc make libX11-devel
```

### Arch Linux

```bash
sudo pacman -S base-devel libx11
```

## Build

### Recommended way
```bash
make
```

### Alternative way
```bash
chmod +x ./build.sh
./build.sh
```

This produces the executable `xkb-layout` in the project root.

## Run

```bash
./xkb-layout
```

Example output:

```text
us
```

## Project structure

```text
.
├── main.c      # program source code
├── Makefile    # build instructions
├── build.sh    # alternative build script
└── xkb-layout  # compiled binary (created after build)
```

## Roadmap

- Improve error handling and return codes
- Add installation target (`make install`)

## Contributing

This is primarily a learning repository, but suggestions and improvements are
welcome.

If you want to contribute:

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Open a merge request

## License

This project is licensed under the MIT License.

See `LICENSE` for full text.
