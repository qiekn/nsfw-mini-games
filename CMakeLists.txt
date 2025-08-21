cmake_minimum_required(VERSION 3.24...3.30)
project(game)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

#---------------------------------------------------------------------3
#                            Dependencies                             |
#---------------------------------------------------------------------3
# 1. Raylib
set(RAYLIB_VERSION 5.5)
FetchContent_Declare(
  raylib
  DOWNLOAD_EXTRACT_TIMESTAMP OFF
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)

#---------------------------------------------------------------------3
#                             Our Project                             |
#---------------------------------------------------------------------3

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp") # source files
file(GLOB_RECURSE HDRS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h") # header files

# targets
add_executable(${PROJECT_NAME} ${SRCS})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

# link libraries
target_link_libraries(${PROJECT_NAME} raylib)

set_target_properties(${PROJECT_NAME} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/game)

if ("${PLATFORM}" STREQUAL "Web")
  add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    # Emscripten '--preload-file assets' use this fold
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/../assets
  )
else()
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  )
endif()

# web configurations
if ("${PLATFORM}" STREQUAL "Web")
  # Tell Emscripten to build an example.html file.
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  target_link_options(${PROJECT_NAME} PUBLIC
    -sUSE_GLFW=3
    -sEXPORTED_RUNTIME_METHODS=['HEAPF32']
    -sEXPORTED_RUNTIME_METHODS=['ccall']
    -sEXPORTED_RUNTIME_METHODS=['requestFullscreen']
    --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html
    --preload-file assets)
endif()

# checks if OSX and links appropriate frameworks (only required on macOS)
if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework IOKit")
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
  target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# format
add_custom_target(format 
  COMMAND clang-format -i ${SRCS} ${HDRS}
  COMMENT "Running clang-format on all header & cpp files"
)

# vim: set ts=2 sw=2 et:
