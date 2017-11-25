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
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP")
else()
	set(CPACK_GENERATOR "TGZ;DEB")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Serge Zaitsev")
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libcairo2, libwebkitgtk-3.0-0")
	set(CPACK_SET_DESTDIR ON)
endif()
include(CPack)
