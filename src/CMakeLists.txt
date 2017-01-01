#-------------------------------------------------------------------
# This file is part of the CMake build system for PDFGen
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

file(GLOB_RECURSE SOURCE *.cpp)
file(GLOB_RECURSE HEADERS *.h)

if(MSVC)
	add_executable(pdfgen WIN32 ${SOURCE})
else()
	add_executable(pdfgen ${SOURCE})
endif()
target_link_libraries(pdfgen Qt5::WebEngine Qt5::WebEngineWidgets)
