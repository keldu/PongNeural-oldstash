project(PongPing)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++14)

#set(KELHEADLESS OFF)

#add_subdirectory( /home/claudius/workspace/Krwa-Headless subproject/Krwa-Headless)


include_directories(
	        
	external/libpng-1.6.21/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/Box2D
        ${PongPing_SOURCE_DIR}/src/
)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(external)
aux_source_directory(. SRC_LIST)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        GLEW_1130
        Box2D
	png
        SDL2
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

file(GLOB_RECURSE SRC_LIST
    "src/*.cpp"
    #"external/*.cpp"
    #"external/*.h"
)

add_executable(${PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${PROJECT_NAME}
        ${ALL_LIBS}
)
