# Windows Setup

- [Windows Setup](#windows-setup)
  - [Setup WSL](#setup-wsl)
    - [For Windows 10 version 2004 (Build 19041 or higher) or Windows 11](#for-windows-10-version-2004-build-19041-or-higher-or-windows-11)
    - [For earlier version of Windows](#for-earlier-version-of-windows)
  - [Setup the distribution](#setup-the-distribution)
  - [Setup Visual Studio Code](#setup-visual-studio-code)
  - [Setup workspace](#setup-workspace)
  - [Setup XServer](#setup-xserver)

## Setup WSL

To enable the most recent version of Windows Subsystem for Linux, Virtualization
must be enabled on your device. You can check if Virtualization is enable by
opening the Task Manager (`Ctrl+Shift+Esc`) in the Performance tab.

If Virtualization is disabled, you should be able to enable it from the BIOS.
The procedure varies based on you PC manufacturer; check [this
article](https://www.bleepingcomputer.com/tutorials/how-to-enable-cpu-virtualization-in-your-computer-bios/)
for an example.

The setup of WSL depends on the version of the OS installed:

### For Windows 10 version 2004 (Build 19041 or higher) or Windows 11

Open PowerShell in administrator mode and enter the following command:

```powershell
wsl --install
```

then restart your machine.

The above command only works if WSL is not installed at all, if you run `WSL
--install` and see the WSL help text, run

```powershell
wsl --install -d Ubuntu
```

to install the latest Ubuntu distribution.

### For earlier version of Windows

The manual installation requires a few more steps. You can follow the guide [here](https://learn.microsoft.com/en-us/windows/WSL/install-manual)

## Setup the distribution

Once you have installed WSL, you need to create a user account and password for
your newly installed Linux distribution.

You can start WSL by running the `WSL` command from any terminal, or look for Ubuntu in the start menu.

When launching `WSL` from terminal make sure to navigate back to the home
directory of Linux (run the command `cd` with no arguments), to avoid working in
a mounted folder on Windows.

## Setup Visual Studio Code

For the course we suggest using VSCode as IDE and text editor.

Download and install it **on Windows** from the [VSCode website](https://code.visualstudio.com/), making sure to select the "Add to PATH" option during the installation.

Download and install the [Remote Development Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack).

We also suggest to install the [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

With the Remote Development Extension you can now run the `code` command from WSL to open VSCode (you may need to restart WSL the first time).

## Setup workspace

From the bash command line in WSL, run the following commands to setup the workspace:

```bash
sudo apt update && sudo apt upgrade
sudo apt install g++ git clang-format cmake libsfml-dev
```

## Setup XServer

To use graphical user interfaces in Linux, you need to setup an X Server. In
Windows build version 21364 or higher, WSL supports a GUI natively.

In WSL run the following commands:

```bash
sudo apt install x11-apps
xclock
```

If no error occurs, no further action is needed.

Otherwise, try first updating your WSL version, by running from PowerShell:

```powershell
wsl --update
```

If this doesn't help, you have to install an external X Server, following this
[guide](https://aalonso.dev/blog/how-to-use-gui-apps-in-wsl2-forwarding-x-server-cdj).

