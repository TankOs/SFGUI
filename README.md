# SFGUI — Simple and Fast Graphical User Interface

## Getting started

  1. Download and install [SFML](http://sfml-dev.org/).
  2. Download SFGUI from the
     [download page](https://github.com/TankOs/SFGUI/releases). If available,
     choose a binary release for your operating system and IDE/toolset.
  3. Unpack the archive.
  4. If source release: Build and install SFGUI (see [Building](#building) for
     instructions).
  5. To see that everything works, start a new C++ project and compile the
     [Hello World example](https://github.com/TankOs/SFGUI/blob/master/examples/HelloWorld.cpp).

## Documentation

Visit the [SFGUI wiki](https://github.com/TankOs/SFGUI/wiki) for further help.

## Building

Building SFGUI is only required in the following cases:

  * You like building yourself in general.
  * No binary release is available for your operating system, neither at the
    website, nor in your operating system's software archives.
  * You want to adjust SFGUI to your likings or help with development.

At first, download and install the dependencies:

  * [SFML](http://sfml-dev.org/)
  * [CMake](http://cmake.org/)

Make yourself familiar with CMake and how it works.

Then create a new directory `build`, start CMake (GUI), choose that directory
as your build directory and SFGUI's directory as source code directory.

Hit _Configure_ followed by _Generate_. Close CMake.

Finally either open the project files you have generated with CMake (e.g.
Visual Studio solution) or instruct the chosen Make tool to build SFGUI, for
example GNU Make:

  * `make install` (as root)

On Linux and after installing, you have to update _ld's_ library cache by
running `ldconfig` as root.

## Using SFGUI in other CMake-powered projects

Projects using CMake within the build process can make use of SFGUI's
[_FindSFGUI.txt_](https://github.com/TankOs/SFGUI/blob/master/cmake/Modules/FindSFGUI.cmake)
file. Copy that file into your CMake _Modules_ directory (located in CMake's
directory on Windows, or in _/usr/share/cmake-x.y/Modules/_).

Instructions on how to use and configure the script can be found right at the
top of the file in the comment section.

## Contributing

Prior contributing source code, please read the [guidelines
file](https://github.com/TankOs/SFGUI/blob/master/doc/GUIDELINES.md). It
contains important information about requirements for pull requests to be
accepted.

The [authors file](https://github.com/TankOs/SFGUI/blob/master/AUTHORS.md)
lists contributors together with contact information. If you make a
contribution, please add yourself to the list.

## Contact

  * Website: https://github.com/TankOs/SFGUI
  * IRC: [irc.boxbox.org:6697, #sfgui](irc://irc.sfml-dev.org:6697/#sfgui)
