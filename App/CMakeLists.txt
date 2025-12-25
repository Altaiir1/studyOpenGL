# ============================================
# Sources / Headers
# ============================================

# Explicit is better than GLOB for templates, but if you prefer GLOB, keep it.
# Here: keep your GLOB, but you can later switch to explicit lists.

file(GLOB_RECURSE APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE APP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# ============================================
# Executable
# ============================================
# Use the same name as project() in the root CMakeLists.txt
add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${APP_HEADERS}
        src/App.cpp
        src/utils/ImGuiHandler.cpp
        src/UI/UI.cpp
        src/Shader/Shader.cpp
        src/BufferObjs/VAO.cpp
        src/BufferObjs/VBO.cpp
        src/BufferObjs/EBO.cpp
        src/Window.cpp
        include/Window.h
        include/utils/Debug.h
        src/utils/stb_image_impl.cpp
        src/Textures/Texture.cpp
        include/Textures/Texture.h
        src/Transform.cpp
        include/Transform.h)

# Include your own header directory
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external  # For stb_image/stb_image.h
        imgui
)

# ============================================
# Link dependencies
# ============================================
# glfw_target is defined in the root CMakeLists.txt (system or FetchContent)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        imgui
        glfw_target
        glad_loader
        glm::glm
        OpenGL::GL
)

# ============================================
# Shaders / Resources
# ============================================
# Copy shaders into a "shaders" folder next to the executable
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/shaders
            ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "Copying shader files..."
)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        COMMENT "Copying shaders and resources..."
)

add_dependencies(${PROJECT_NAME} copy_assets)