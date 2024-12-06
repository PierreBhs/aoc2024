cmake_minimum_required(VERSION 3.28)
project(aoc2024)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers -O3)

add_executable(${PROJECT_NAME}
                # ${CMAKE_SOURCE_DIR}/src/01.cpp
                # ${CMAKE_SOURCE_DIR}/src/02.cpp
                # ${CMAKE_SOURCE_DIR}/src/03.cpp
                # ${CMAKE_SOURCE_DIR}/src/04.cpp
                ${CMAKE_SOURCE_DIR}/src/05.cpp
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
