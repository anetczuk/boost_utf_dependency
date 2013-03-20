project(BoostUnitTestDependency)

cmake_minimum_required(VERSION 2.8)

## The version number.
#set (Tutorial_VERSION_MAJOR 1)
#set (Tutorial_VERSION_MINOR 0)


# build in Release-mode by default if not explicitly set
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories( "./include" )

find_package(Boost)

set( EXT_LIBS boost_unit_test_framework)

set( TARGET_NAME boost_test_frame)

file(GLOB_RECURSE SOURCE "src/*.cpp")

#add_executable( ${TARGET_NAME} ${SOURCE} ${HEADERS})
#target_link_libraries( ${TARGET_NAME} ${EXT_LIBS})

add_library( ${TARGET_NAME} SHARED ${SOURCE})
target_link_libraries( ${TARGET_NAME} ${EXT_LIBS})


#------------------------------------- building tests -------------------------------


add_subdirectory( tests )

