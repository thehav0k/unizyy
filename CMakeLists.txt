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

# Collect all source files (excluding main.cpp for library)
set(LIBRARY_SOURCES
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

# Create a static library with common code
add_library(unizyy_lib STATIC ${LIBRARY_SOURCES})

# Create main executable
add_executable(unizyy main.cpp)
target_link_libraries(unizyy unizyy_lib)

# Create test executable (separate main if needed)
# add_executable(test_token_review_persistence test_main.cpp)
# target_link_libraries(test_token_review_persistence unizyy_lib)

# Modern C++17 filesystem support
target_compile_features(unizyy_lib PUBLIC cxx_std_17)
target_compile_features(unizyy PUBLIC cxx_std_17)

# Link filesystem library properly
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(unizyy_lib PUBLIC stdc++fs)
    target_link_libraries(unizyy PUBLIC stdc++fs)
endif()

# Compiler options
target_compile_options(unizyy_lib PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

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