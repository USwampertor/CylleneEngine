add_executable(cyCrashHandler
  src/main.cpp
  src/cyPlaformCrashHandlerUI.cpp
)

target_include_directories(cyCrashHandler
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(cyCrashHandler
  PRIVATE
    cyUtilities
)

message(WARNING "cyCrashHandler requires ImGui with a GLFW/SDL+OpenGL backend on Linux. "
                "The Windows D3D11 backend is not available.")
