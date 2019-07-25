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

### Tools needed

  * [CMake](http://cmake.org/), it is recommended to make yourself familiar with
    CMake and how it works.
  * [Conan](https://conan.io/) (**Optional**, use it to get all the project dependencies)
  * **C++11 compatible compiler**.
    > When using **conan**, make sure that your compiler is compatible with the
    packages downloaded. For example, when using Visual Studio 15 2017 on a modern
    machine, conan will download 64 bits libraries. If you want to use the 32 bits
    ABI (Win32), you will need to change your conan profile accordingly.

### Dependencies

Only [SFML](http://sfml-dev.org/) is required. You can install it with most operating
system package managers, and rely on cmake mechanisms to find and configure your project.
Here is how to use conan to get it:

  1. Add [bincrafters](https://bincrafters.github.io/) repository to your conan
     configuration
     ```
     conan remote add -f bincrafters https://api.bintray.com/conan/bincrafters/public-conan
     ```
     > Bincrafters is a repository of packages pending addition to conan official server.
  2. From the SFGUI repository, use conan to install dependencies
     ```
     conan install .
     ```

### Build!
  1. Configure using CMake:
     ```
     cmake -H. -Bbuild
     ```
     > `-H` specifies the path to your sources. On windows, I had better luck with `".\"`.
     
     > `-B` is the path to the build directory, name it as you see fit, place it
     where your fancy strickes you, it doesn't need to be among the sources.
     CMake will fix all the paths for you.

     You can use `cmake-gui` to have a convenient configuration interface.
     You need to _Configure_ and _Generate_ the build information by clicking on the buttons.
     It will not help with the compilation step.
  2. Build
      ```
      cmake --build build
      ```
      > This magic cross-compilation-fits-all command will use the selected generator
        to build your project! No need to fire up any IDE, you can work from your favorite
        text editor in peace.
  3. (Optional) Install
     ```
     cmake --build build --target install
     ```
     > CMake registers this project locally so that other CMake projects can find it.

     Depending on how you configured `CMAKE_INSTALL_PREFIX` (`/usr/local` on linux
     by default, `C:/Program Files/SFGUI` on windows), you man need administrator access
     to run the command (`sudo` on linux).

     > On Linux and after installing, you may want to update _ld's_ library cache
       by running `ldconfig` as root.

## Using SFGUI in other CMake-powered projects

### In your `CMakeList.txt`

```cmake
find_package(SFGUI 0.4.0 REQUIRED)
# ...
add_executable(SuperGameProject ${CPPS})
# ...
target_link_libraries(SuperGameProject PRIVATE SFGUI::SFGUI)
```

### A word on how `find_package` works:

This project auto-magically register itself locally, so you don't even need to
install for `find_package` to find it! If you installed at a standard path,
`find_package` will also be able to find the configuration.

However, if you did not install at a standard path or have several versions of SFGUI,
you will need to provide configure the CMake variable `SFGUI_DIR` to `cmake`.
The value is _the path to the directory containing `SFGUIConfig.cmake`_.

On unix systems, it is usually under `<CMAKE_PREFIX_PATH>/share/cmake/SFGUI`,
eg: `/usr/local/share/cmake/SFGUI` (this path should be looked up by default.)

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
  * Discord: [SFML server](https://discord.gg/nr4X7Fh)
