cmake_minimum_required(VERSION 3.20)
project(unizyy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(Core/Models)
include_directories(Core/Utils)
include_directories(Core/Database)
include_directories(System/Users)
include_directories(System/Authentication)
include_directories(System/Modules/Meal)
include_directories(System/Interface)

# Collect all source files
set(SOURCES
    main.cpp
    Core/Models/date.cpp
    Core/Database/DatabaseManager.cpp
    System/Users/student.cpp
    System/Users/Admin.cpp
    System/Users/Teacher.cpp
    System/Users/DiningAuthority.cpp
    System/Users/PublicRelationsAdmin.cpp
    System/Authentication/auth.cpp
    System/Modules/Meal/meal.cpp
    System/Interface/MenuInterface.cpp
    System/Interface/MainMenu.cpp
    System/Interface/StudentInterface.cpp
    System/Interface/AdminInterface.cpp
    System/Interface/TeacherInterface.cpp
    System/Interface/DiningAuthorityInterface.cpp
)

add_executable(unizyy ${SOURCES})

# Link filesystem library for macOS/Linux
if(APPLE OR UNIX)
    find_library(FILESYSTEM_LIB c++fs)
    if(FILESYSTEM_LIB)
        target_link_libraries(unizyy ${FILESYSTEM_LIB})
    endif()
endif()

# Compiler options
target_compile_options(unizyy PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Set output directory
set_target_properties(unizyy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Create runtime directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Database)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Meal Tokens")
