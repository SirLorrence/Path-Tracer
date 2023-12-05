cmake_minimum_required(VERSION 3.0) # version of cmake
project(RayRenderer) # name of the exe
set(CMAKE_CXX_STANDARD 20) # set the c++ version

# Generating a JSON compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set all warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

# set file paths
set(src_dir "${PROJECT_SOURCE_DIR}/src/")
set(include_dir "${PROJECT_SOURCE_DIR}/include/")

# include external libraries
add_library(Tinylib lib/TinyPngOut.cc) # needed when trying to include source files into a library
target_include_directories(Tinylib PUBLIC lib)

# Packages
find_package(OpenMP)
find_package(OpenACC)



# get the files
file (GLOB source_files "${src_dir}/*.cc")
file (GLOB include_files "${include_dir}/*.h")

# add_compile_definitions(MTHREAD)
# add_compile_definitions(MTHREAD_V2)
# add_compile_definitions(ENABLE_MP)
add_compile_definitions(ENABLE_ACC)

add_executable(${PROJECT_NAME} ${source_files} ${include_files}) 

# Library Linkage

target_link_libraries(${PROJECT_NAME} Tinylib)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()

if(OpenACC_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenACC::OpenACC_CXX)
endif()

