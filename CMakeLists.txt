#-------------------------------------------------------------------
# This file is part of the CMake build system for PDFGen
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

cmake_minimum_required (VERSION 3.1.0)
project(PDFGen)

set(CMAKE_CXX_STANDARD 14)

# Cache properties
set(QT_LOCATION "" CACHE PATH "Location of your Qt installation.")

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH ${QT_LOCATION})

find_package(Qt5WebEngine REQUIRED)
find_package(Qt5WebEngineWidgets REQUIRED)

# Construct our inclusion list
set(INCLUDES
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/include
)
include_directories(${INCLUDES})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Define intermediate names for platform-specific output folders
if(WIN32)
	set(PLATFORM_DIR_NAME windows)
elseif(APPLE)
	set(PLATFORM_DIR_NAME apple)
else()
	set(PLATFORM_DIR_NAME linux)
endif()

# Setting the new output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_DIR_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)

add_subdirectory(src)
