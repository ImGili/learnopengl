cmake_minimum_required(VERSION 3.0)
project(MyEngine)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Engine/Vendor/glfw)

include_directories(Engine/Headers/
                    Engine/Vendor/glfw/include/
                    )

