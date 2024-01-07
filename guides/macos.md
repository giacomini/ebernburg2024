# macOS

- [macOS](#macos)
  - [Xcode Command Line Tools](#xcode-command-line-tools)
  - [Homebrew](#homebrew)
  - [Install the main tools](#install-the-main-tools)
  - [Install Visual Studio Code](#install-visual-studio-code)
  - [Software updates](#software-updates)

This guide documents the steps to perform in order to install, on _macOS-based_
laptops and desktops, all the software needed to fruitfully attend the hands-on
session of this **C++ Basic Workshop**.

## Xcode Command Line Tools

The _Xcode Command Line Tools (CLT)_ are required in order to successfully
install the _Homebrew_ package manager. Moreover they ensure that a second
compiler is available as a fallback solution in case of problem with the
preferred one (_GCC_).

To install the _CLT_, open a _Terminal_ and issue:

```shell
% xcode-select --install
```

A pop-up window will open, asking to confirm the installation.

Proceed by following the instructions (which will likely include the request to
type your account's password or authenticate using the fingerprint reader).

Depending on the network performance, the installation may take some time (e.g.
up to half an hour or more). Once completed, you can verify that the procedure
was successful by issuing the following command, and checking that its output is
consistent with:

```shell
% xcode-select -p
/Library/Developer/CommandLineTools
```

Alternatively, if you have already installed the full _Xcode_
suite, the following output is equivalently valid:

```shell
% xcode-select -p
/Applications/Xcode.app/Contents/Developer
```

## Homebrew

The [`Homebrew`](https://brew.sh/) _package manager_ (hereafter referred to as
_brew_) is the main tool we will use to install and keep up-to-date all the
programs and libraries used during the workshop.

To proceed with the installation, the minimal requirements are:

- A 64-bit Intel CPU or Apple Silicon CPU 1
- macOS Monterey (12) (or higher)
- the Command Line Tools (CLT) for Xcode (or Xcode)
- the Bourne-again shell (available by default on macOS)

If all of them are satisfied, open a _Terminal_ and issue the following command:

```shell
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

If, during the installation, errors appear complaining about missing
certificates when the `curl` command is ran, try to open the offending URL
within the Safari browser, and _entrust_ all certificates. Then, issue the installation command again.

Please note that, to complete the installation, **instructions may appear at the
bottom of the terminal where the installation command was ran**.
If needed, please take care of following them as well.

To verify the installation was correctly achieved, issue:

```shell
% brew
```

If all went as expected, the command output will start with the following lines:

```shell
Example usage:
  brew search TEXT|/REGEX/
  brew info [FORMULA|CASK...]
  brew install FORMULA|CASK...
  brew update
  brew upgrade [FORMULA|CASK...]
  brew uninstall FORMULA|CASK...
  brew list [FORMULA|CASK...]
  ...
```

## Install the main tools

Once _Homebrew_ is correctly functioning, run the following command:

```shell
% brew install git gcc clang-format cmake
```

In order, these programs will be installed:

- The [git](https://git-scm.com/) _version control system_
- The [gcc](https://gcc.gnu.org/) compiler
- The [clang-format](https://www.kernel.org/doc/html/latest/translations/it_IT/process/clang-format.html) code formatter
- [CMake](https://cmake.org/) a software that eases the compilation of non-trivial projects

You can verify the installation of all above programs by checking their version:

```shell
% git --version
git version 2.43.0
```

```shell
% g++-13 --version
g++-13 (Homebrew GCC 13.2.0) 13.2.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

```shell
% clang-format --version
clang-format version 17.0.6
```

```shell
% cmake --version
cmake version 3.28.0

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

Updates may have occurred between the moment this guide was prepared and the
workshop date. Therefore, discrepancies in the version numbering might be
expected. Please consider this step successful if all programs are available and
theversion numbers are not too far off from the ones listed.

## Install Visual Studio Code

_Visual Studio Code_ [_Visual Studio Code_](https://code.visualstudio.com/)
(VSCode) can either be installed using brew:

```shell
% brew install visual-studio-code
```

or downloading the macOS installer [here](https://code.visualstudio.com/).

Once done, please follow
[these instructions](https://code.visualstudio.com/docs/setup/mac#_launching-from-the-command-line)
to be able to start VScode from the command line (you might be requested to type
your password or scan your fingerprint).

Finally check that they worked:

```shell
% code
```

## Software updates

Please, regularly update the packages installed with brew:

```shell
% brew update
% brew upgrade
```

More details are available in the [brew
FAQ](https://docs.brew.sh/FAQ#how-do-i-update-my-local-packages).
