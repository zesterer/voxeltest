#######################
#### CONFIGURATION ####
#######################

#settings.h header file
configure_file(
"settings.h.in"
"settings.h"
)

########################
#### Subdirectories ####
########################

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/data")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/window")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/common")

#################
#### SOURCES ####
#################

#Add the appropriate source directories
set(LIBVOLUME_SOURCE_DIRS
${LIBVOLUME_SOURCE_DIRS}
"${PROJECT_BINARY_DIR}/src/libvolume"
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_CURRENT_SOURCE_DIR}/.."
PARENT_SCOPE
)

#Add the appropriate source files
set(LIBVOLUME_SOURCE_FILES
${LIBVOLUME_SOURCE_FILES}
PARENT_SCOPE
)
