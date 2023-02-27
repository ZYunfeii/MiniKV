cmake_minimum_required(VERSION 3.5.1)

project(MiniKV C CXX)
include(./common.cmake)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)

set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/../proto/kvserver.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/../proto/kvserver.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/../proto/kvserver.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/../proto/kvserver.grpc.pb.h")

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(grpc_proto ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs})

target_link_libraries(grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

file(GLOB_RECURSE server_src "${CMAKE_CURRENT_BINARY_DIR}/../server/*.cc")
set(server_src "${server_src};${CMAKE_CURRENT_BINARY_DIR}/../log/log.cc")

file(GLOB_RECURSE client_src "${CMAKE_CURRENT_BINARY_DIR}/../client/*.cc")
set(client_src "${client_src};${CMAKE_CURRENT_BINARY_DIR}/../log/log.cc")

add_executable(kvserver ${server_src})
add_executable(kvclient ${client_src})
target_link_libraries(kvserver grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(kvclient grpc_proto ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
