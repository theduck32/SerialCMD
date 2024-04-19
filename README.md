# SerialCMD

A small tool written in C++ to allow the use of the Windows Command Prompt (cmd.exe) via the Serial Ports

This project is meant for debugging purposes only, and if you can use network instead then i recommend you either use SSHD or TelnetD.

> **NOTE :** This project is dependent on Windows Serial Drivers, such as Serenum.sys and Serial.sys , therefore if you want to use this project on Windows you will have to make sure these files are present in **C:\Windows\System32\drivers** , Some architectures such as ARM are known to not have these drivers, however you can still attempt to compile the drivers via either the [ReactOS Project](https://github.com/reactos/reactos) or the [Microsoft Driver Samples](https://github.com/microsoft/Windows-driver-samples/tree/main).


# Compiling
> **NOTE :** main-full.cpp Utilizes the registry in order to get the OS Version and report it to serial. It should not make a big difference if you compile the "Full" version or the "Lite" one.

SerialCMD can be compiled with either GCC or Clang
If you want to cross-compile to other architectures i would personally recommend using Clang as it was the most successful.

## Windows

### Compiling with Clang

 1. First of all, download [llvm-mingw](https://github.com/mstorsjo/llvm-mingw/releases) and extract all of the files to the path you want.
 2. Once all the files have been extracted, open **System Environment Variables** and add the "**bin**" folder to your **PATH** variable.
 3. Open a new Command Prompt and cd into the repository.
 ```
cd SerialCMD
@REM Replace <arch> with one of these : [x86_64,i686,aarch64,armv7]
<arch>-w64-mingw32-g++ -Os -static -o SerialCMD-full.exe main-full.cpp
<arch>-w64-mingw32-g++ -Os -static -o SerialCMD-lite.exe main-lite.cpp
strip SerialCMD-full.exe
strip SerialCMD-lite.exe
```

### Compiling with GCC MSYS2
> **NOTE :** Compiling with GCC on MSYS2 may cause errors when trying to compile main-full.cpp , to fix that you will need to follow the Clang guide. 

 1. First of all, if you haven't already then install
    [MSYS2](https://www.msys2.org/) on your device.
 2. Once MSYS2 has been installed open up MSYS2 and run the following commands
 
```
pacman -Sy base-devel gcc git
git clone https://github.com/theduck32/SerialCMD
cd SerialCMD
g++ -Os -static -o SerialCMD-lite.exe main-lite.cpp
g++ -Os -static -o SerialCMD-full.exe main-full.cpp
strip SerialCMD-lite.exe
strip SerialCMD-full.exe
```
 3. If it compiled successfully then you should have the binaries ready.

## Linux

### Compiling with MinGW GCC

 1. First install MinGW and Git using the package manager provided by your Linux distro. I will be using Arch Linux's package manager for this
 ```
 sudo pacman -Sy mingw-w64-gcc mingw-w64-headers git
 git clone https://github.com/theduck32/SerialCMD
 cd SerialCMD
 x86_64-w64-mingw32-g++ -static -Os -o SerialCMD-full.exe main-full.cpp 
 x86_64-w64-mingw32-g++ -static -Os -o SerialCMD-lite.exe main-full.cpp 
 x86_64-w64-mingw32-strip SerialCMD-full.exe 
 x86_64-w64-mingw32-strip SerialCMD-lite.exe 
 ```

