cmake_minimum_required(VERSION 3.22)

project(MyOpenGLApp VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# GLFW
find_package(glfw3 3.4 QUIET)
if(glfw3_FOUND)
    if(TARGET glfw)
        add_library(glfw_target INTERFACE)
        target_link_libraries(glfw_target INTERFACE glfw)
    elseif(TARGET glfw3::glfw)
        add_library(glfw_target INTERFACE)
        target_link_libraries(glfw_target INTERFACE glfw3::glfw)
    else()
        message(FATAL_ERROR "glfw3 found but no usable target")
    endif()
else()
    FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
    add_library(glfw_target INTERFACE)
    target_link_libraries(glfw_target INTERFACE glfw)
    set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
endif()

find_package(OpenGL REQUIRED)

# Platform-specific GLAD - Fixed path checking
if(APPLE)
    set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/mac")
elseif(WIN32)
    set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/windows")
elseif(UNIX AND NOT APPLE)
    set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/linux")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Check both glad.c AND include/glad/glad.h exist
if(NOT EXISTS "${GLAD_DIR}/src/glad.c")
    message(FATAL_ERROR "GLAD source missing: ${GLAD_DIR}/src/glad.c")
endif()
if(NOT EXISTS "${GLAD_DIR}/include/glad/glad.h")
    message(FATAL_ERROR "GLAD header missing: ${GLAD_DIR}/include/glad/glad.h")
endif()

add_library(glad_loader STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad_loader PUBLIC "${GLAD_DIR}/include")
set_target_properties(glad_loader PROPERTIES FOLDER "Dependencies")

# GLM
find_package(glm 1.0.1 QUIET)
if(NOT (glm_FOUND AND TARGET glm::glm))
    FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
    )
    FetchContent_MakeAvailable(glm)
    set_target_properties(glm PROPERTIES FOLDER "Dependencies")
endif()

add_subdirectory(App)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
