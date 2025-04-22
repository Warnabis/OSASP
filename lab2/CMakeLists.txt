cmake_minimum_required(VERSION 3.10)

# Название проекта
project(ProcessLab C)

# Устанавливаем стандарт C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Добавляем исполняемые файлы
add_executable(parent parent.c)
add_executable(child child.c)

# Устанавливаем флаги компиляции
target_compile_options(parent PRIVATE -Wall -Wextra -Wpedantic -std=c11)
target_compile_options(child PRIVATE -Wall -Wextra -Wpedantic -std=c11)
