# Compile Guide for RadixEngine
## Quick note if you are compiling for the first time
Well done. You made the right decision. It's not even that hard.

## Dependencies
**NOTE:** Dependencies will change all the time so watch the cmake output carefuly for missing libraries.
Make sure to check CMakeLists.txt if it is something missing.

## Upgrade your OS
You will need very recent versions of all the dependencies. So please upgrade your OS before proceeding.
- [Upgrade Ubuntu](http://www.ubuntu.com/download/desktop/upgrade)
- [Upgrade Arch](https://wiki.archlinux.org/index.php/Pacman#Upgrading_packages)

## List of dependencies
- assimp
- epoxy
- sdl2
- sdl2-mixer
- bullet *>=2.82+*
- tinyxml2
- libgl
- freeimg
- cmake *>=2.8 (make)*
- make *(make)*
- gcc *(make)*
- pkg-config *(make)*
- git *(download source code)*
- libunittest++ *(optional, for tests)*

## Installing dependencies
Remember, you have to be root, so you might want to prefix `sudo`.

### Windows
[Install msys2](http://msys2.github.io/), then launch the *Mingw-w64 Win64 shell* from the start menu. In it, type:
```bash
pacman -S mingw-w64-x86_64-{cmake,make,gcc,pkg-config,assimp,libepoxy,SDL2,SDL2_mixer,bullet,tinyxml2,freeimage}
```

If you do not already have `git` installed, please also install it with:

```bash
pacman -S git
```

### Arch Linux
**Note:** Package `tinyxml2` can be found in AUR.
```bash
pacman -S cmake make gcc pkg-config assimp libepoxy sdl2 sdl2_mixer bullet mesa unittestpp freeimage
```

### Debian 8 / Ubuntu 14.04 / Linux Mint 17.x
```bash
apt-get install cmake make gcc pkg-config lib{assimp,epoxy,sdl2,sdl2-mixer,bullet,tinyxml2,gl1-mesa,unittest++,freeimage}-dev
```

### Others
Now how will you satisfy these dependencies? No problem. You might notice that when you
are looking for them in your package manager that you get overwhelmed by a list of possible
installation candidates, how do you know which one is the right one?

An easy rule that applies to many libraries is that a library has the lib prefix and then, 
since you want to install the files for development there is a dev in the name. 
Pretty much like libfoo-dev for most libraries.

# Downloading source code

In your favorite console, run these commands:

```bash
git clone https://github.com/GlPortal/RadixEngine.git
cd RadixEngine
git submodule init
git submodule update
```

# Building the binary

Make sure to check the README.org and COMPILE.org from the release you downloaded in case
there are differences in the compile workflow for it.

In the root directory of the downloaded source type:
```bash
mkdir build && cd build
cmake ..
```

**NOTE**: On Windows, please use `cmake .. -G "MSYS Makefiles"` instead.

If you want to profile the source code pass *-DCMAKE_CXX_FLAGS=-pg* to cmake.
Now if this throws an error you have to fix something first. If you don't manage to fix the error, use the
bugtracker linked below.

If the command did not produce an error, you can build the binary by typing in:
```bash
make
```

If this produces no error you have built the binary and should be able to link it with a project, e.g. GlPortal.

If you get errors, try to build Radix again. If you don't manage to fix the error, use the
bugtracker https://github.com/GlPortal/RadixEngine/issues or http://bugs.glportal.de to report what you did, and what error you got.

# Keep us posted
Not working? No worries! Help us help you figure out what we missed to make this work on 
your system by opening an issue.
