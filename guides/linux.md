# Linux setup

- [Linux setup](#linux-setup)
  - [Install the main tools](#install-the-main-tools)
  - [Install VSCode](#install-vscode)

This guide documents the steps to perform in order to install, on _linux-based_
laptops and desktops, all the software needed to fruitfully attend the hands-on
session of this **C++ Basic Workshop**.

## Install the main tools

Make sure to update your workspace, by running the following commands from the terminal:

```bash
sudo apt update
sudo apt upgrade
```

Once the package repository is updated, install the following packages:

```bash
sudp apt install  g++ git clang-format cmake libsfml-dev
```
## Install VSCode

Download the Visual Studio Code .deb package from the [website](https://code.visualstudio.com/download).

Install it through the graphical interface, or through the command line with:

```bash
sudo apt install ./<file>.deb
```
