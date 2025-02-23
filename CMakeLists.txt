########################################################################
# check if required vers are already defined from top-down building approach
########################################################################
if(NOT DEFINED required_cmake_ver)
    set(required_cmake_ver 3.10)
endif()

if(NOT DEFINED required_cxx_ver)
    set(required_cxx_ver 14)
endif()

########################################################################
#  Project Setup
########################################################################
set(target Log)
cmake_minimum_required(VERSION ${required_cmake_ver})
project(${target} LANGUAGES CXX C)

########################################################################
# set build params if need
########################################################################
set(CMAKE_CXX_STANDARD ${required_cxx_ver})
if(NOT CMAKE_BUILD_TYPE)
   	set(CMAKE_BUILD_TYPE "Release")
endif()

message("================ cfg beg ====================================")
message(STATUS "required_cmake_ver set to ${required_cmake_ver}")
message(STATUS "required_cxx_ver set to ${required_cxx_ver}")
message(STATUS "CMAKE_BUILD_TYPE set to ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX set to ${CMAKE_INSTALL_PREFIX}")
message("================ cfg end ====================================")


########################################################################
# Create the LoggingUtils Library
########################################################################
if( NOT TARGET Log ) 
    add_library(${target} STATIC
            src/cxx/SpdLogHelper.cpp
            )
endif()

########################################################################
# Include directories
########################################################################
target_include_directories(${target} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)


install( TARGETS ${target} DESTINATION lib/ )

########################################################################
# end statement
########################################################################
message("${target} Finished Compiling.")
