cmake_minimum_required (VERSION 2.8)
cmake_policy(VERSION 2.8)

project (LearnOpenGL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


link_directories(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# find the required packages
find_package(glm REQUIRED)
message(STATUS "GLM included at ${glm_INCLUDE_DIR}")
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${glfw3_INCLUDE_DIR}")
find_package(assimp REQUIRED)
message(STATUS "Found ASSIMP in ${assimp_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
# find_package(GLEW REQUIRED)
# message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
# find_package(freetype)
set(FREETYPE_INCLUDE_DIRS /usr/include/freetype2)
include_directories(${FREETYPE_INCLUDE_DIRS})
# note that the order is important for setting the libs
# use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
set(LIBS glfw dl pthread assimp freetype)

include_directories ("../src")
set(LIBS ${LIBS} ada)
add_subdirectory(../src ${CMAKE_CURRENT_BINARY_DIR}/ada)


# configure_file(configuration/root_directory.h.in configuration/root_directory.h)
# include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries requried for other projects
# add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

# add_library(GLAD "src/glad.c")
# target_link_libraries(GLAD dl)
# target_compile_options(GLAD PUBLIC -ldl)
set(LIBS ${LIBS} GLAD)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

set(CHAPTER 3.model_loading)
set(DEMO 1.model_loading)
# then create a project file per tutorial
file(GLOB SOURCE
    "src/*.h"
    "src/*.cpp"
    "src/*.vs"
    "src/*.fs"
    "src/*.gs"
)
add_library(render ${SOURCE})
target_compile_options(render PUBLIC -ldl -lXmu -lXt -lSM -lICE -lXext -lX11 -lXi -lXext -lX11 -ggdb -std=c++11)
target_link_libraries(render ${LIBS})
# copy shader files to build directory
file(GLOB SHADERS
         "src/*.vs"
         # "src/*.frag"
         "src/*.fs"
         "src/*.gs"
)
foreach(SHADER ${SHADERS})
        file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER})
endforeach(SHADER)

include_directories(includes)
