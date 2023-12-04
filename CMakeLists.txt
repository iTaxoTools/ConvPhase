cmake_minimum_required(VERSION 3.15...3.26)
project(${SKBUILD_PROJECT_NAME} LANGUAGES C CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(ExternalProject)

find_package(
    Python
    COMPONENTS Interpreter Development.Module
    REQUIRED)


# SeqPHASE

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


# PHASE

file(GLOB PHASE_SOURCES_CPP ${PROJECT_SOURCE_DIR}/src/phase/src/phase*/*.cpp)
file(GLOB PHASE_SOURCES_C ${PROJECT_SOURCE_DIR}/src/phase/src/phase*/*.c)
set(PHASE_SOURCES ${PHASE_SOURCES_CPP} ${PHASE_SOURCES_C})

file(GLOB PHASE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src/phase/src/phase*
    ${PROJECT_SOURCE_DIR}/src/itaxotools/_convphase/include
)

add_library(phase STATIC ${PHASE_SOURCES})
target_include_directories(phase PUBLIC ${PHASE_INCLUDE_DIRS})
set_property(TARGET phase PROPERTY CXX_STANDARD 14)

target_compile_definitions(phase PRIVATE
    -DCP_PHASE_LIB
    -DCP_PHASE_NOFILE
    -DCP_PHASE_DISABLE_COUT
    -DCP_PHASE_DISABLE_CERR
)


# ConvPhase

file(GLOB PYMODULE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/itaxotools/_convphase/src/*.cpp
)
set(PYMODULE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src/itaxotools/_convphase/include
    ${CMAKE_BINARY_DIR}/seqphase/src/SeqPhaseLib-build/include
    C:\\HaxeToolkit\\haxe\\lib\\hxcpp/4,3,2/include
)

python_add_library(_convphase MODULE ${PYMODULE_SOURCES} WITH_SOABI)
target_include_directories(_convphase PRIVATE ${PYMODULE_INCLUDE_DIRS})
add_dependencies(_convphase SeqPhaseLib)
target_link_directories(_convphase PRIVATE ${CMAKE_BINARY_DIR}/seqphase/src/SeqPhaseLib-build)
target_link_libraries(_convphase PRIVATE seqphase phase)
set_property(TARGET phase PROPERTY CXX_STANDARD 14)
target_compile_definitions(_convphase PRIVATE
    -DHX_WINDOWS
    -DHXCPP_M64
)


install(TARGETS _convphase DESTINATION itaxotools)
