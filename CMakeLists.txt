cmake_minimum_required(VERSION 3.0)
project(Slide)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(test)

#
# Packaging
#
if(APPLE)
	set(CPACK_GENERATOR "DragNDrop")
	set(CPACK_PACKAGE_FILE_NAME "slide-macos")
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP")
	set(CPACK_PACKAGE_FILE_NAME "slide-win32")
else()
	set(CPACK_GENERATOR "TGZ;DEB")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serge Zaitsev")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libcairo2, libwebkitgtk-3.0-0")
	set(CPACK_SET_DESTDIR ON)
	set(CPACK_PACKAGE_FILE_NAME "slide-linux")
endif()
include(CPack)
