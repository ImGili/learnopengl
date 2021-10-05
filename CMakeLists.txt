cmake_minimum_required(VERSION 3.0)
project(Engine)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Engine/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Engine/Vendor/assimp)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-parentheses  -std=c++11")
        message("Apple")
    elseif(WIN32)
        message("WIN32")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fexec-charset=GBK  -std=c++11")
    elseif(UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic  -std=c++11")
        message("Linux")        
    endif()
    
        
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()


include_directories(                Engine/include/
                                    Engine/Vendor/glad/include/
                                    Engine/Vendor/glfw/include/
                                    Engine/Vendor/stb
                                    Engine/Vendor/glm
                                    Engine/Vendor/imgui
                                    Engine/Vendor/assimp/include/
                                    )

file(GLOB VENDORS_SOURCES           Engine/Vendor/glad/src/glad.c
                                    Engine/Vendor/imgui/*.cpp
                                    Engine/Vendor/imgui/backends/imgui_impl_opengl3.cpp
                                    Engine/Vendor/imgui/backends/imgui_impl_glfw.cpp
                                    )
file(GLOB PROJECT_HEADERS           Engine/include/*.hpp
                                    Engine/include/*.h
                                    Engine/test/*.h
                                    )
file(GLOB PROJECT_SOURCES           Engine/src/*.cpp)
file(GLOB PROJECT_SHADERS           resources/shader/*)
file(GLOB PROJECT_RESOURCES_FONTS   Engine/resources/fonts/*.ttf
                                    )
file(GLOB PROJECT_RESOURCES_IMGS    Engine/resources/imgs/*
                                    )


add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -DASSIMP_BUILD_ASSIMP_TOOLS="OFF")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      )
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Engine/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Engine/Resources $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_RESOURCES_FONTS} ${PROJECT_RESOURCES_IMGS})
    