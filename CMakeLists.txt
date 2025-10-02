cmake_minimum_required(VERSION 3.15)
project(CoalOS VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Use FetchContent for dependencies
include(FetchContent)

# GLFW - Window/Input management
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM - Math library
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# FreeType - Font rendering
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-13-0
)
FetchContent_MakeAvailable(freetype)

# miniaudio - Audio (header-only)
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.21
)
FetchContent_MakeAvailable(miniaudio)

# nlohmann/json - JSON parsing
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# GLAD - Build as library (make sure src/glad.c exists!)
if(EXISTS "${CMAKE_SOURCE_DIR}/src/glad.c")
    add_library(glad "${CMAKE_SOURCE_DIR}/src/glad.c")
    target_include_directories(glad PUBLIC "${CMAKE_SOURCE_DIR}/include")
    message(STATUS "GLAD found at: ${CMAKE_SOURCE_DIR}/src/glad.c")
else()
    message(FATAL_ERROR "GLAD not found! Please place glad.c in the src/ directory")
endif()

# Collect all source files (excluding glad.c since it's already compiled separately)
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Collect all header files
file(GLOB_RECURSE HEADERS 
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    OpenGL::GL
    glfw
    glm::glm
    freetype
    glad
    nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
    "${miniaudio_SOURCE_DIR}"
)

# Copy assets to build directory at build time
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets to build directory"
)

# Print configuration info
message(STATUS "CMake Source Dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Include Dir: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "Source Dir: ${CMAKE_SOURCE_DIR}/src")