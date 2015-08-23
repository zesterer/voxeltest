#Specify the cmake version being used
cmake_minimum_required(VERSION 2.6)

#Name of the project
project(voxeltest)

#Tell the compiler to include -std=c++11
add_definitions(-std=c++11 -Wall)

#Add LibVolume
add_subdirectory(libvolume)
include_directories(${LIBVOLUME_INCLUDES})

#All the directories containing header files
include_directories(
"${PROJECT_SOURCE_DIR}/src"
)

set(SOURCE_FILES
src/main.cpp
)

#Take the code and turn it into the executable
add_executable(voxeltest ${SOURCE_FILES})

#Link the final executable to the needed libraries
target_link_libraries(voxeltest libvolume)
