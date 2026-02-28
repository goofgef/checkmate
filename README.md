$$$$$$$$$$$$$$$$$
    Checkmate
$$$$$$$$$$$$$$$$$

Checkmate is a stack-based VM written in C, designed to provide a modular and customizable experience for developers.

- What does VM stand for?  
VM stands for virtual machine. In Checkmate’s context, a VM is an abstract program that interprets code—usually bytecode. It runs as a standalone, isolated program.  
A stack-based VM operates on a stack, which you can think of as a line of people: you can remove the person at the top ("pop") or add someone to the top ("push").

To get started:

You need GCC and make.

Installation:

- Debian/Ubuntu:


  sudo apt update
  sudo apt install build-essential


- Fedora:


  sudo dnf groupinstall "Development Tools"


- Arch Linux:


  sudo pacman -S base-devel


- **macOS:**


  xcode-select --install


Then, compile and run the source with **compkit.bat** or **compkit.sh**, depending on your system.

**For compkit.bat:**


compkit compile main.c

compkit run


**For compkit.sh:**


chmod +x compkit.sh

./compkit.sh compile main.c

./compkit.sh run


This opens a basic REPL where you can try out the base VM.

To use it as a library:


compkit libdll

./compkit.sh libso


This will generate a DLL or SO file you can link against.
