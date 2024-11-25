cmake_minimum_required(VERSION 3.27)
project(projectX CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INC_FILES "src/*.hpp")

file(GLOB_RECURSE SHADER_FILES "resources/shaders/*.hlsl")

include_directories("src")
include_directories("src/core")
include_directories("src/core/utils")
include_directories("src/core/window")
include_directories("src/render")
include_directories("src/world")

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

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES} ${SHADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 dxgi d3dcompiler)

foreach(shader ${SHADER_FILES})

    if(shader MATCHES ".*vs.*")
        set(shader_type "Vertex")
    elseif(shader MATCHES ".*ps.*")
        set(shader_type "Pixel")
    else()
        message(SEND_ERROR "Unknown shader type for file: ${shader}")
    endif()

    target_sources(${PROJECT_NAME} PRIVATE ${shader})

    set_source_files_properties(${shader} PROPERTIES
        VS_SHADER_TYPE ${shader_type}
        VS_SHADER_ENTRYPOINT "main"
        VS_SHADER_MODEL "5.0"
    )
endforeach()

source_group("Shaders" FILES ${SHADER_FILES})
