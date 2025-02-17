cmake_minimum_required (VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
project (CMakeHelloWorld)

#version number
set (CMakeHelloWorld_VERSION_MAJOR 1)
set (CMakeHelloWorld_VERSION_MINOR 0)

set (CMAKE_INSTALL_BINDIR "bin")

set(CMAKE_CONFIGURATION_TYPES "Release")
set(CMAKE_BUILD_TYPE "Release")

include(cmake/conan_helper.cmake)

conan_cmake_configure(
    REQUIRES

    grpc/1.39.1
    protobuf/3.20.0

    zlib/1.2.11
GENERATORS cmake_find_package
    KEEP_RPATHS
    IMPORTS "bin, *.dll -> ${CMAKE_BINARY_DIR}"
    IMPORTS "lib, *.so* -> ${CMAKE_BINARY_DIR}"

)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    SETTINGS ${settings})

find_package(gRPC REQUIRED)

#include the subdirectory containing our libs
add_subdirectory (Hello gen)
include_directories(Hello gen)
#indicate the entry point for the executable
add_executable (CMakeHelloWorld Hello HelloWorld.cpp 
    ${PROTO_SRCS}
    ${PROTO_HDRS})

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GRPC REQUIRED grpc)
#pkg_search_module(GRPCPP REQUIRED grpc++>=1.22.0)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

set(ProtoFiles
proto/helloworld/hello_world.proto
)

protobuf_generate(OUT_VAR ProtoSources ProtoHeaders
PROTOS ${ProtoFiles}
LANGUAGE cpp
IMPORT_DIRS proto
)

protobuf_generate(OUT_VAR ServiceSources ServiceHeaders
PROTOS ${ProtoFiles}
LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
IMPORT_DIRS proto
)

add_library(Messages STATIC ${ProtoSources} ${ProtoHeaders} ${ServiceSources} ${ServiceHeaders})

# Indicate which libraries to include during the link process.
target_link_libraries(CMakeHelloWorld
PUBLIC
${PROTOBUF_LIBRARY}
protobuf::libprotobuf
gRPC::grpc
gRPC::grpc++
Hello
)

# install targets
install (TARGETS CMakeHelloWorld
DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
