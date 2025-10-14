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
        Core/Models/Notice.cpp
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
        System/Interface/PublicRelationsAdminInterface.cpp
)

# Create main executable
add_executable(unizyy ${SOURCES})

# Create test executable (includes main.cpp for now)
add_executable(test_token_review_persistence
        main.cpp  # Added this line
        Core/Models/date.cpp
        Core/Models/Notice.cpp
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
        System/Interface/PublicRelationsAdminInterface.cpp
)

# Link filesystem library for macOS/Linux
if(APPLE OR UNIX)
    find_library(FILESYSTEM_LIB c++fs)
    if(FILESYSTEM_LIB)
        target_link_libraries(unizyy ${FILESYSTEM_LIB})
        target_link_libraries(test_token_review_persistence ${FILESYSTEM_LIB})
    endif()
endif()

# Compiler options for main target
target_compile_options(unizyy PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Compiler options for test target
target_compile_options(test_token_review_persistence PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Set output directory for both targets
set_target_properties(unizyy PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(test_token_review_persistence PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Create runtime directories
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Database)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Meal Tokens")