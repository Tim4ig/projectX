cmake_minimum_required(VERSION 3.27)
project(projectX CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INC_FILES "src/*.hpp")

include_directories("src")
include_directories("src/core")
include_directories("src/core/utils")
include_directories("src/core/window")

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /ZI")
endif()

function(create_source_group FILES PREFIX)
    foreach(FILE ${FILES})
        file(RELATIVE_PATH REL_FILE_PATH "${CMAKE_SOURCE_DIR}/src" ${FILE})
        get_filename_component(DIR_PATH "${REL_FILE_PATH}" PATH)
        if (DIR_PATH)
            string(REPLACE "/" "\\" FILTER_PATH "${DIR_PATH}")
            source_group("${PREFIX}\\${FILTER_PATH}" FILES "${FILE}")
        else()
            source_group("${PREFIX}" FILES "${FILE}")
        endif()
    endforeach()
endfunction()

create_source_group("${SRC_FILES}" "Source Files")
create_source_group("${INC_FILES}" "Header Files")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE)
