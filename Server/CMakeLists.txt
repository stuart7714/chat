cmake_minimum_required(VERSION 3.21)

project(Server LANGUAGES CXX)

# Find dependencies before adding subdirectories that may use them.
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Generated source files.
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Proto/chat.proto")
set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Proto")
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Library containing the generated Protobuf and gRPC code.
add_library(chat_proto)

# Generate Protobuf code.
protobuf_generate(
    TARGET chat_proto
    LANGUAGE cpp
    PROTOS "${PROTO_FILE}"
    IMPORT_DIRS "${PROTO_DIR}"
    PROTOC_OUT_DIR "${GENERATED_DIR}"
)

protobuf_generate(
    TARGET chat_proto
    LANGUAGE grpc
    PROTOS "${PROTO_FILE}"
    IMPORT_DIRS "${PROTO_DIR}"
    PROTOC_OUT_DIR "${GENERATED_DIR}"
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
)

target_include_directories(chat_proto
    PUBLIC
        "${GENERATED_DIR}"
)

target_link_libraries(chat_proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc++
)

# Define the executable and link it to the generated-code library.
add_subdirectory(Server)
