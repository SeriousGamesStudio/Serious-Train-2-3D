GUIDE TO BUILDING OGRE
========================================================================

Ogre uses [CMake](https://cmake.org/) as its build system on all supported platforms.
This guide will explain to you how to use CMake to build Ogre from source.
CMake is available from https://cmake.org/download/. You need a CMake
version >= 2.8.6.

What is CMake?
-------------------

CMake is a cross-platform build system - or perhaps more accurately a
build configurator. It is a program which, from a set of CMake scripts,
creates a native build system for your platform that allows you to
build Ogre.
The build process is configurable via CMake. Ogre provides several
options which you can use to customise your build.

Preparing the build environment
------------------------------------

You should now create a build directory for Ogre somewhere outside
Ogre's sources. This is the directory where CMake will create the
build system for your chosen platform and compiler, and this is also
where the Ogre libraries will be compiled. This way, the Ogre source
dir stays clean, and you can have multiple build directories all
working from the same Ogre source.

Getting dependencies
-------------------------

By default ogre will build the essential dependencies automatically when you run cmake the first time. If you would rather use system wide libraries set `OGRE_BUILD_DEPENDENCIES=OFF`.

Ogre will install the dependencies into the subfolder `Dependencies` in the build dir by default. You can configure it by setting `OGRE_DEPENDENCIES_DIR` in cmake. For instance to point to a common dependencies folder for all of your projects. Inside
this directory you must have the subdirectories bin, lib and include
where you place .dll, .lib and header files of the dependencies, respectively

On linux you additionally need the following system headers to build the GL RenderSystems (command for Ubuntu):

    sudo apt-get install libgles2-mesa-dev libxt-dev libxaw7-dev

furthermore we recommend installing the following optional packages

    sudo apt-get install nvidia-cg-toolkit libsdl2-dev doxygen

these will enable input handling in the SampleBrowser and building the documentation.

If you cannot obtain prebuilt binaries of a dependency for your platform,
please refer to the list below and get a source package from the website,
then build it according to its documentation.

### Essential dependencies:

* freetype: http://www.freetype.org

### Recommended dependencies:

* zlib: http://www.zlib.net
* zziplib: https://github.com/paroj/ZZIPlib
* SDL: https://www.libsdl.org/

### Optional dependencies:

* DirectX SDK: http://msdn.microsoft.com/en-us/directx/
* FreeImage: http://freeimage.sourceforge.net
* Doxygen: http://doxygen.org
* Cg: http://developer.nvidia.com/object/cg_toolkit.html
* Boost: http://www.boost.org (+)
* POCO: http://pocoproject.org (+)
* TBB: http://www.threadingbuildingblocks.org (+)

(+) used to build threaded versions of Ogre. 
You can use either C++11, POCO or TBB instead of Boost. When using Boost, only the boost-thread, boost-system and boost-date-time libraries are required.

Running CMake
------------------

Now start the program cmake-gui by either typing the name in a console
or selecting it from the start menu. In the field *Where is the source
code* enter the path to the Ogre source directory (the directory which
contains this file). In the field *Where to build the binaries* enter
the path to the build directory you created.
Hit *Configure*. A dialogue will appear asking you to select a generator.
Choose the appropriate one for your platform and compiler. On Unix, you
most likely want to use *Unix Makefiles*; for Visual Studio select the
appropriate version and platform (Win32 | Win64); on Apple use Xcode.
Click *Finish*. CMake will now gather some information about your
build environment and try to locate the dependencies. It will then show
a list of build options. You can adjust the settings to your liking; for
example unchecking any of the `OGRE_BUILD_XXX` options will disable that
particular component from being built. Once you are satisfied, hit
*Configure* again and then click on *Generate*. CMake will then create
the build system for you.

Building Ogre
------------------

Go to your chosen build directory. CMake has generated a build system for
you which you will now use to build Ogre. If you are using Visual Studio,
you should find the file OGRE.sln. Open it and compile the target
*BUILD_ALL*. Similarly you will find an Xcode project to build Ogre
on MacOS. If you rather want to trigger the build form a console, then
 cd to your build directory and call the appropriate make
program as

    cmake --build . --config release

to start the build process.
If you have doxygen installed and CMake picked it up, then there will
be an additional build target called *OgreDoc* which you can optionally build.
This will freshly generate the API documentation for Ogre's classes from
the header files. In Visual Studio, just select and build the target
*OgreDoc*, on Linux type:

     make OgreDoc


Installing Ogre
--------------------

Once the build is complete, you can optionally have the build system
copy the built libraries and headers to a clean location. We recommend
you do this step as it will make it easier to use Ogre in your projects.
In Visual Studio, just select and build the target *INSTALL*. This will
create the folder `sdk` inside your build directory and copy all the
required libraries there. For Makefile based generators, type:

```sh
make install  # (or sudo make install, if root privileges are required)
```

On Linux Ogre will by default be installed to `/usr/local`. You can change
the install location by changing the variable `CMAKE_INSTALL_PREFIX` in
CMake.


Building Ogre on Mac OS X for iOS OS
--------------------------------------------

To build Ogre for iOS, you need to specify the ios cross toolchain to cmake as

`cmake -DCMAKE_TOOLCHAIN_FILE=CMake/toolchain/ios.toolchain.xcode.cmake -G Xcode .`

Unfortunately, you will now have to do a few manual steps to
make the generated build system work properly.

A Xcode project has now been generated in the build directory, so
to start the Ogre build, open OGRE.xcodeproj and build as usual.
To run samples on your device you will need to have a valid iOS Developer
certificate installed.  For each sample, double click on target in the Groups &
Files list.  Ensure that a valid identity is selected in the Code Signing Identity
drop menu.
Also, because we can't tell CMake what Xcode project format you want, you will
have to change it yourself.  Open the Project Menu, choose Edit Project Settings.
Click on the General tab in the settings window.  Change Project Format to
Xcode 3.1-compatible.
And another thing.  You will need to manually set the Bundle Identifier property of
the Info.plist file to match the App ID of the chosen code signing identity.
This can be done from the Target Properties panel.  It must match the bundle
identifier of a valid developer certificate if you are building for devices.

Building Ogre as Windows Store or Windows Phone application
-------------------------------------------------------------------

You need Windows 8.0 or later, Windows 10 is recommended.

You need Visual Studio 2012 or later, Visual Studio 2015 is recommended
as it is bundled with Universal 10.0.240.0, WinStore 8.0/8.1 and WinPhone 8.0/8.1 SDKs.

Download and install CMake 3.4 or later.

Patched dependencies must be used, compiled with appropriate WINAPI_FAMILY.
Cg is not supported.
You can use https://bitbucket.org/eugene_gff/ogre-dependencies-winrt -
has VS2012 and VS2013 projects for Win32, WinRT (can be reused for WinPhone)
Compile dependencies for all configurations that you plan to use before
running CMake. Dependencies for Win32 and for WinRT must be located in
separate folders, Win32 version can be built from OgreDependencies.VS201x.sln, 
WinRT from OgreDependencies.VS201x.WinRT.sln

Run CMake, specify source and binaries folders, than "Configure", select
"Visual Studio 14 2015" generator and "Specify options for cross-compiling"
option, specify Operating System = "WindowsStore" or "WindowsPhone",
Version = "8.0", "8.1" or for UAP Operating System = "WindowsStore", Version = "10.0.10240.0", "10.0.10586.0" then "Finish", specify WinRT dependencies folder
for OGRE_DEPENDENCIES_DIR, "Configure", should be no more errors, then press
"Generate". If you want build time to be shorter - set OGRE_UNITY_BUILD to on.

Select SampleBrowser as the start up project and run.

### Notes

1. The code and generated CMake solution should be on local NTFS drive,
and can't be on a network drive, including VMWare shared folders - or
you will get a errors when you will try to compile/link/run resulting exe.

2. Ogre uses d3dcompiler_xx.dll to compile shaders, and WinStore and
WinPhone 8.1 projects can use it without restriction as it is part of OS.
But WinStore and WinPhone 8.0 applications should load already compiled
shaders from cache, as d3dcompiler_xx.dll is available to them only during
development. Therefore to avoid necessity to deploy d3dcompiler_xx.dll
during development (additional configurations with d3dcompiler_xx.dll)
and generation of such cache - use version 8.1 of these platforms.

3. Running WinPhone emulator in OS running under VMware:
We were able to run the emulation and debug without an issue by using two
steps: (a) Under the settings of the VM > CPU, make sure you have the
option to pass-through the Intel VT-x/EPT feature, (b) Edit the config
file .vmx and add the parameter: hypervisor.cpuid.v0 = "FALSE"
All versions of Visual Studio 2012 have a window refresh issue when running
in VMware and the window is maximized, the solution is just to change the
size of the Visual Studio window to be less the the screen width and height.

Building Ogre on Ubuntu for Android
--------------------------------------------

To build Ogre for Android, you need to specify the ios cross toolchain to cmake as

`cmake -DCMAKE_TOOLCHAIN_FILE=CMake/toolchain/android.toolchain.cmake -DANDROID_NDK=path/to/android-ndk .`

this will build the core Ogre libraries. Also if your `PATH` contains the `ndk-build` and `android` executables it will generate the SampleBrowser APK.

To manually build the SampleBrowser, navigate your command prompt to the SampleBrowserNDK subfolder in your OGRE build folder and run:

`ndk-build .`

this will build the native library. Then run

`ant debug install`

to generate the APK and install it on your device.

Building Ogre for HTML5 (Emscripten)
-----------------------------------------
Install the Emscripten SDK and make sure that the environment variables are correctly set.

Run cmake in cross compile mode using emscripten as following:

```
cmake -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake .
```

in cmake GUI change `CMAKE_AR` to `emcc`. Then run `make`.

this will not build the full SampleBrowser, but just a minimal Sample. The resulting `EmscriptenSample.html` will be placed in `${CMAKE_BINARY_DIR}/bin/`.

To prevent any cross-origin issues, start a local webserver as `python3 -m http.server 8000` and visit `localhost:8000`.
