#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#######################################################################
# Find all necessary and optional OGRE dependencies
#######################################################################

# OGRE_DEPENDENCIES_DIR can be used to specify a single base
# folder where the required dependencies may be found.
set(OGRE_DEPENDENCIES_DIR "" CACHE PATH "Path to prebuilt OGRE dependencies")
option(OGRE_BUILD_DEPENDENCIES "automatically build Ogre Dependencies (freetype, zzip)" TRUE)

include(FindPkgMacros)
getenv_path(OGRE_DEPENDENCIES_DIR)
if(OGRE_BUILD_PLATFORM_EMSCRIPTEN)
  set(OGRE_DEP_SEARCH_PATH
    ${OGRE_DEPENDENCIES_DIR}
    ${EMSCRIPTEN_ROOT_PATH}/system
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/EmscriptenDependencies"
    "${OGRE_SOURCE_DIR}/EmscriptenDependencies"
    "${OGRE_BINARY_DIR}/../EmscriptenDependencies"
    "${OGRE_SOURCE_DIR}/../EmscriptenDependencies"
  )
  set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} ${OGRE_DEP_SEARCH_PATH})
elseif(APPLE_IOS)
  set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/iOSDependencies"
    "${OGRE_SOURCE_DIR}/iOSDependencies"
    "${OGRE_BINARY_DIR}/../iOSDependencies"
    "${OGRE_SOURCE_DIR}/../iOSDependencies"
  )
elseif(OGRE_BUILD_PLATFORM_ANDROID)
  set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/AndroidDependencies"
    "${OGRE_SOURCE_DIR}/AndroidDependencies"
    "${OGRE_BINARY_DIR}/../AndroidDependencies"
    "${OGRE_SOURCE_DIR}/../AndroidDependencies"
  )
else()
  set(OGRE_DEP_SEARCH_PATH 
    ${OGRE_DEPENDENCIES_DIR}
    ${ENV_OGRE_DEPENDENCIES_DIR}
    "${OGRE_BINARY_DIR}/Dependencies"
    "${OGRE_SOURCE_DIR}/Dependencies"
    "${OGRE_BINARY_DIR}/../Dependencies"
    "${OGRE_SOURCE_DIR}/../Dependencies"
  )
endif()

message(STATUS "Search path: ${OGRE_DEP_SEARCH_PATH}")
list(GET OGRE_DEP_SEARCH_PATH 0 OGREDEPS_PATH)

if(CMAKE_CROSSCOMPILING)
    set(CROSS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
    
    if(ANDROID)
        set(CROSS ${CROSS}
            -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
            -DANDROID_ABI=${ANDROID_ABI}
            -DANDROID_NDK=${ANDROID_NDK})
    endif()
    
    if(APPLE_IOS)
        set(CROSS ${CROSS}
            -DIOS_PLATFORM=${IOS_PLATFORM})
    endif()
endif()

# Set hardcoded path guesses for various platforms
if (UNIX AND NOT EMSCRIPTEN)
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/local)
  # Ubuntu 11.10 has an inconvenient path to OpenGL libraries
  set(OGRE_DEP_SEARCH_PATH ${OGRE_DEP_SEARCH_PATH} /usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu)
endif ()

# give guesses as hints to the find_package calls
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${OGRE_DEP_SEARCH_PATH})
set(CMAKE_FRAMEWORK_PATH ${CMAKE_FRAMEWORK_PATH} ${OGRE_DEP_SEARCH_PATH})

if(OGRE_BUILD_DEPENDENCIES AND NOT EXISTS ${OGREDEPS_PATH})
    set(OGREDEPS_SHARED TRUE)
    if(OGRE_STATIC OR MSVC)
        # freetype does not like shared build on MSVC and it generally eases distribution there
        set(OGREDEPS_SHARED FALSE)
    endif()

    set(BUILD_COMMAND_OPTS --target install --config ${CMAKE_BUILD_TYPE})

    if(MSVC OR EMSCRIPTEN) # other platforms ship zlib
        message(STATUS "Building zlib")
        file(DOWNLOAD 
            http://zlib.net/zlib-1.2.11.tar.gz
            ${OGRE_BINARY_DIR}/zlib-1.2.11.tar.gz 
            EXPECTED_MD5 1c9f62f0778697a09d36121ead88e08e)
        execute_process(COMMAND ${CMAKE_COMMAND} 
            -E tar xf zlib-1.2.11.tar.gz WORKING_DIRECTORY ${OGRE_BINARY_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND}
            -DCMAKE_INSTALL_PREFIX=${OGREDEPS_PATH}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_SHARED_LIBS=${OGREDEPS_SHARED}
            -G ${CMAKE_GENERATOR}
            ${CROSS}
            ${CMAKE_BINARY_DIR}/zlib-1.2.11
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/zlib-1.2.11)
        execute_process(COMMAND ${CMAKE_COMMAND} 
            --build ${CMAKE_BINARY_DIR}/zlib-1.2.11 ${BUILD_COMMAND_OPTS})
    endif()

    message(STATUS "Building ZZIPlib")
    file(DOWNLOAD
        https://github.com/paroj/ZZIPlib/archive/master.tar.gz
        ${OGRE_BINARY_DIR}/ZZIPlib-master.tar.gz)
    execute_process(COMMAND ${CMAKE_COMMAND}
        -E tar xf ZZIPlib-master.tar.gz WORKING_DIRECTORY ${OGRE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX=${OGREDEPS_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DZLIB_ROOT=${OGREDEPS_PATH}
        -DBUILD_SHARED_LIBS=${OGREDEPS_SHARED}
        -G ${CMAKE_GENERATOR}
        ${CROSS}
        ${CMAKE_BINARY_DIR}/ZZIPlib-master
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ZZIPlib-master)
    execute_process(COMMAND ${CMAKE_COMMAND} 
        --build ${CMAKE_BINARY_DIR}/ZZIPlib-master ${BUILD_COMMAND_OPTS})
    
    message(STATUS "Building freetype")
    file(DOWNLOAD
        http://download.savannah.gnu.org/releases/freetype/freetype-2.6.5.tar.gz
        ${OGRE_BINARY_DIR}/freetype-2.6.5.tar.gz)
    execute_process(COMMAND ${CMAKE_COMMAND}
        -E tar xf freetype-2.6.5.tar.gz WORKING_DIRECTORY ${OGRE_BINARY_DIR})
    # patch toolchain for iOS
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/CMake/toolchain/ios.toolchain.xcode.cmake
        freetype-2.6.5/builds/cmake/iOS.cmake
		WORKING_DIRECTORY ${OGRE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX=${OGREDEPS_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_SHARED_LIBS=${OGREDEPS_SHARED}
        -DWITH_BZip2=OFF # tries to use it on iOS otherwise
        # workaround for broken iOS toolchain in freetype
        -DPROJECT_SOURCE_DIR=${CMAKE_BINARY_DIR}/freetype-2.6.5
        ${CROSS}
        -G ${CMAKE_GENERATOR}
        ${CMAKE_BINARY_DIR}/freetype-2.6.5
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/freetype-2.6.5/objs)
    execute_process(COMMAND ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR}/freetype-2.6.5/objs ${BUILD_COMMAND_OPTS})

    if(MSVC) # other platforms dont need this
        message(STATUS "Building SDL2")
        file(DOWNLOAD
            https://libsdl.org/release/SDL2-2.0.5.tar.gz
            ${OGRE_BINARY_DIR}/SDL2-2.0.5.tar.gz)
        execute_process(COMMAND ${CMAKE_COMMAND} 
            -E tar xf SDL2-2.0.5.tar.gz WORKING_DIRECTORY ${OGRE_BINARY_DIR})
        execute_process(COMMAND ${CMAKE_COMMAND}
            -E make_directory ${OGRE_BINARY_DIR}/SDL2-build)
        execute_process(COMMAND ${CMAKE_COMMAND}
            -DCMAKE_INSTALL_PREFIX=${OGREDEPS_PATH}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DSDL_STATIC=FALSE
            -G ${CMAKE_GENERATOR}
            ${CROSS}
            ${OGRE_BINARY_DIR}/SDL2-2.0.5
            WORKING_DIRECTORY ${OGRE_BINARY_DIR}/SDL2-build)
        execute_process(COMMAND ${CMAKE_COMMAND}
            --build ${OGRE_BINARY_DIR}/SDL2-build ${BUILD_COMMAND_OPTS})
    endif()
endif()

#######################################################################
# Core dependencies
#######################################################################

# Find zlib
find_package(ZLIB)
macro_log_feature(ZLIB_FOUND "zlib" "Simple data compression library" "http://www.zlib.net" FALSE "" "")

if (ZLIB_FOUND)
  # Find zziplib
  find_package(ZZip)
  macro_log_feature(ZZip_FOUND "zziplib" "Extract data from zip archives" "http://zziplib.sourceforge.net" FALSE "" "")
endif ()

# Find FreeImage
find_package(FreeImage)
macro_log_feature(FreeImage_FOUND "freeimage" "Support for commonly used graphics image formats" "http://freeimage.sourceforge.net" FALSE "" "")

# Find FreeType
find_package(Freetype)
macro_log_feature(FREETYPE_FOUND "freetype" "Portable font engine" "http://www.freetype.org" FALSE "" "")

# Find X11
if (UNIX AND NOT APPLE AND NOT ANDROID AND NOT EMSCRIPTEN)
  find_package(X11)
  macro_log_feature(X11_FOUND "X11" "X Window system" "http://www.x.org" TRUE "" "")
  find_library(XAW_LIBRARY NAMES Xaw Xaw7 PATHS ${OGRE_DEP_SEARCH_PATH} ${DEP_LIB_SEARCH_DIR} ${X11_LIB_SEARCH_PATH})
  macro_log_feature(XAW_LIBRARY "Xaw" "X11 Athena widget set" "http://www.x.org" TRUE "" "")
  mark_as_advanced(XAW_LIBRARY)
endif ()


#######################################################################
# RenderSystem dependencies
#######################################################################

# Find OpenGL
if(NOT ANDROID AND NOT EMSCRIPTEN)
  find_package(OpenGL)
  macro_log_feature(OPENGL_FOUND "OpenGL" "Support for the OpenGL render system" "http://www.opengl.org/" FALSE "" "")
endif()

# Find OpenGL 3+
find_package(OpenGL)
macro_log_feature(OPENGL_FOUND "OpenGL 3+" "Support for the OpenGL 3+ render system" "http://www.opengl.org/" FALSE "" "")

# Find OpenGL ES 1.x
find_package(OpenGLES)
macro_log_feature(OPENGLES_FOUND "OpenGL ES 1.x" "Support for the OpenGL ES 1.x render system (DEPRECATED)" "http://www.khronos.org/opengles/" FALSE "" "")

# Find OpenGL ES 2.x
find_package(OpenGLES2)
macro_log_feature(OPENGLES2_FOUND "OpenGL ES 2.x" "Support for the OpenGL ES 2.x render system" "http://www.khronos.org/opengles/" FALSE "" "")

# Find OpenGL ES 3.x
find_package(OpenGLES3)
macro_log_feature(OPENGLES3_FOUND "OpenGL ES 3.x" "Support for the OpenGL ES 2.x render system with OpenGL ES 3 support" "http://www.khronos.org/opengles/" FALSE "" "")

# Find DirectX
if(WIN32)
	find_package(DirectX)
	macro_log_feature(DirectX9_FOUND "DirectX9" "Support for the DirectX render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")
	
	find_package(DirectX11)
	macro_log_feature(DirectX11_FOUND "DirectX11" "Support for the DirectX11 render system" "http://msdn.microsoft.com/en-us/directx/" FALSE "" "")

	if(OGRE_CONFIG_ENABLE_QUAD_BUFFER_STEREO)
		# Find DirectX Stereo Driver Libraries
		find_package(NVAPI)
		macro_log_feature(NVAPI_FOUND "NVAPI" "Support NVIDIA stereo with the DirectX render system" "https://developer.nvidia.com/nvapi" FALSE "" "")

		find_package(AMDQBS)
		macro_log_feature(AMDQBS_FOUND "AMDQBS" "Support AMD stereo with the DirectX render system" "http://developer.amd.com/tools-and-sdks/graphics-development/amd-quad-buffer-sdk/" FALSE "" "")
	endif()
endif()

#######################################################################
# Additional features
#######################################################################

# Find Cg
if (NOT (APPLE_IOS OR WINDOWS_STORE OR WINDOWS_PHONE OR ANDROID OR EMSCRIPTEN))
  find_package(Cg)
  macro_log_feature(Cg_FOUND "cg" "C for graphics shader language" "http://developer.nvidia.com/object/cg_toolkit.html" FALSE "" "")
endif ()

# Find Boost
# Prefer static linking in all cases
if (WIN32 OR APPLE)
	set(Boost_USE_STATIC_LIBS TRUE)
else ()
	# Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
	set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
endif ()
if (APPLE AND APPLE_IOS)
    set(Boost_USE_MULTITHREADED OFF)
endif()

if(ANDROID)
    # FindBoost needs extra hint on android 
    set(Boost_COMPILER -gcc)
endif()

if (NOT Boost_FOUND)
  set(Boost_ADDITIONAL_VERSIONS "1.57" "1.57.0" "1.56" "1.56.0" "1.55" "1.55.0" "1.54" "1.54.0" "1.53" "1.53.0" "1.52" "1.52.0" "1.51" "1.51.0" "1.50" "1.50.0" "1.49" "1.49.0" "1.48" "1.48.0" "1.47" "1.47.0" "1.46" "1.46.0" "1.45" "1.45.0" "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40")
  # Components that need linking (NB does not include header-only components like bind)
  set(OGRE_BOOST_COMPONENTS thread date_time)
  find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()
if (NOT Boost_FOUND)
	# Try again with the other type of libs
	if(Boost_USE_STATIC_LIBS)
		set(Boost_USE_STATIC_LIBS OFF)
	else()
		set(Boost_USE_STATIC_LIBS ON)
	endif()
	find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

if(Boost_FOUND AND Boost_VERSION GREATER 104900)
    if(Boost_VERSION GREATER 105300)
        set(OGRE_BOOST_COMPONENTS thread date_time system atomic chrono)
    else()
        set(OGRE_BOOST_COMPONENTS thread date_time system chrono)
    endif()
    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
endif()

if(Boost_FOUND AND NOT WIN32)
  list(REMOVE_DUPLICATES Boost_LIBRARIES)
endif()

# Optional Boost libs (Boost_${COMPONENT}_FOUND
macro_log_feature(Boost_FOUND "boost" "Boost (general)" "http://boost.org" FALSE "" "")
if(NOT Boost_DATE_TIME_FOUND)
    set(Boost_THREAD_FOUND FALSE)
endif()
if(Boost_VERSION GREATER 104900 AND (NOT Boost_SYSTEM_FOUND OR NOT Boost_CHRONO_FOUND))
    set(Boost_THREAD_FOUND FALSE)
endif()
if(Boost_VERSION GREATER 105300 AND NOT Boost_ATOMIC_FOUND)
    set(Boost_THREAD_FOUND FALSE)
endif()
macro_log_feature(Boost_THREAD_FOUND "boost-thread" "Used for threading support" "http://boost.org" FALSE "" "")

# POCO
find_package(POCO)
macro_log_feature(POCO_FOUND "POCO" "POCO framework" "http://pocoproject.org/" FALSE "" "")

# ThreadingBuildingBlocks
find_package(TBB)
macro_log_feature(TBB_FOUND "tbb" "Threading Building Blocks" "http://www.threadingbuildingblocks.org/" FALSE "" "")

# GLSL-Optimizer
find_package(GLSLOptimizer)
macro_log_feature(GLSL_Optimizer_FOUND "GLSL Optimizer" "GLSL Optimizer" "http://github.com/aras-p/glsl-optimizer/" FALSE "" "")

# HLSL2GLSL
find_package(HLSL2GLSL)
macro_log_feature(HLSL2GLSL_FOUND "HLSL2GLSL" "HLSL2GLSL" "http://hlsl2glslfork.googlecode.com/" FALSE "" "")

# OpenEXR
find_package(OpenEXR)
macro_log_feature(OPENEXR_FOUND "OpenEXR" "Load High dynamic range images" "http://www.openexr.com/" FALSE "" "")

# Python
set(Python_ADDITIONAL_VERSIONS 3.4) # allows using python3 on Ubuntu 14.04
find_package(PythonInterp)
find_package(PythonLibs)
macro_log_feature(PYTHONLIBS_FOUND "Python" "Language bindings to use OGRE from Python" "http://www.python.org/" FALSE "" "")

#######################################################################
# Samples dependencies
#######################################################################

# Find sdl2
if(NOT ANDROID)
  # find script does not work in cross compilation environment
  find_package(SDL2)
  macro_log_feature(SDL2_FOUND "SDL2" "Simple DirectMedia Library needed for input handling in samples" "https://www.libsdl.org/" FALSE "" "")
  if(SDL2_FOUND AND WIN32 AND NOT SDL2_BINARY)
    # fix linking static SDL2 on windows
    set(SDL2_LIBRARY ${SDL2_LIBRARY} winmm.lib imm32.lib version.lib)
  endif()
endif()

#######################################################################
# Tools
#######################################################################

find_package(Doxygen)
macro_log_feature(DOXYGEN_FOUND "Doxygen" "Tool for building API documentation" "http://doxygen.org" FALSE "" "")

# Find Softimage SDK
find_package(Softimage)
macro_log_feature(Softimage_FOUND "Softimage" "Softimage SDK needed for building XSIExporter" FALSE "6.0" "")

#######################################################################
# Tests
#######################################################################

# now see if we have a buildable Dependencies package in
# the source tree. If so, include that, and it will take care of
# setting everything up, including overriding any of the above
# findings.
set(OGREDEPS_RUNTIME_OUTPUT ${OGRE_RUNTIME_OUTPUT})
if (EXISTS "${OGRE_SOURCE_DIR}/Dependencies/CMakeLists.txt")
  add_subdirectory(Dependencies)
elseif (EXISTS "${OGRE_SOURCE_DIR}/ogredeps/CMakeLists.txt")
  add_subdirectory(ogredeps)
endif ()


# Display results, terminate if anything required is missing
MACRO_DISPLAY_FEATURE_LOG()

# Add library and include paths from the dependencies
include_directories(
  ${ZLIB_INCLUDE_DIRS}
  ${ZZip_INCLUDE_DIRS}
  ${FreeImage_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${OPENGLES_INCLUDE_DIRS}
  ${OPENGLES2_INCLUDE_DIRS}
  ${OPENGLES3_INCLUDE_DIRS}
  ${Cg_INCLUDE_DIRS}
  ${X11_INCLUDE_DIR}
  ${DirectX_INCLUDE_DIRS}
  ${GLSL_Optimizer_INCLUDE_DIRS}
  ${HLSL2GLSL_INCLUDE_DIRS}
)

link_directories(
  ${OPENGL_LIBRARY_DIRS}
  ${OPENGLES_LIBRARY_DIRS}
  ${OPENGLES2_LIBRARY_DIRS}
  ${OPENGLES3_LIBRARY_DIRS}
  ${Cg_LIBRARY_DIRS}
  ${X11_LIBRARY_DIRS}
  ${DirectX_LIBRARY_DIRS}
)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif ()

# provide option to install dependencies on Windows
include(InstallDependencies)
