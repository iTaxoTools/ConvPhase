cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(ExternalProject)

find_package(
    Python
    COMPONENTS Interpreter Development.Module
    REQUIRED)


ExternalProject_Add(
    SeqPhaseLib
    PREFIX ${CMAKE_BINARY_DIR}/seqphase
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/SeqPhase/haxe
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add_Step(
    SeqPhaseLib
    SeqPhaseLib_haxe_build
    COMMAND haxe --cpp <BINARY_DIR> -D static_link -D ABI=-MD -D HAXE_OUTPUT_FILE=seqphase SeqPhase1.hx SeqPhase2.hx -D HXCPP_M64
    WORKING_DIRECTORY <SOURCE_DIR>
)

file(GLOB PYMODULE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/itaxotools/_example/src/*.cpp
)
set(PYMODULE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src/itaxotools/_example/include
)

python_add_library(_convphase MODULE ${PYMODULE_SOURCES} WITH_SOABI)
target_include_directories(_convphase PRIVATE ${PYMODULE_INCLUDE_DIRS})
add_dependencies(_convphase SeqPhaseLib)
target_link_directories(_convphase PRIVATE ${CMAKE_BINARY_DIR}/seqphase/src/SeqPhaseLib-build)
target_link_libraries(_convphase PRIVATE seqphase)

install(TARGETS _convphase DESTINATION itaxotools)
