# Linux setup

- [Linux setup](#linux-setup)
  - [Install the main tools](#install-the-main-tools)
  - [Install VSCode](#install-vscode)

This guide documents the steps to install, on Ubuntu 22.04 and in general on _Debian_-based
laptops and desktops, the software needed to fruitfully attend the hands-on
sessions of this **C++ Basic Workshop**.

## Install the main tools

First, make sure to update your workspace, by running the following commands from the terminal:

```bash
sudo apt update
sudo apt upgrade
```

Then, install the following packages:

```bash
sudo apt install g++ git clang-format cmake libsfml-dev
```

If you use a different Linux distribution, the commands and the names of the software
packages may be different.

## Install VSCode

Download the Visual Studio Code .deb package from the [website](https://code.visualstudio.com/download).

Install it through the graphical interface or through the command line with:

```bash
sudo apt install ./<file>.deb
```

In case you use another Linux distribution, the package is available also in other formats (e.g. RPM and tarball).
