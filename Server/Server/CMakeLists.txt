# Add source to this project's executable.
add_executable (Server
    "Message.cpp"
    "MessageQueue.cpp"
    "Server.cpp"
    "Service.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
endif()

# Link the generated Protobuf/gRPC library.
target_link_libraries(Server
    PRIVATE
        chat_proto
)

# Copy the dependent DLLs.
add_custom_command(TARGET Server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_RUNTIME_DLLS:Server>"
        "$<TARGET_FILE_DIR:Server>"
    COMMAND_EXPAND_LISTS

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/libcrypto-3-x64.dll"
        "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/libssl-3-x64.dll"
        "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/bin/z.dll"
        "$<TARGET_FILE_DIR:Server>"
)
