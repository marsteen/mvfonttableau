#
#

cmake_minimum_required(VERSION 3.10)
project("createfonttableau")

message("System: ${CMAKE_SYSTEM_NAME}")

set(CMAKE_C_LINK_EXECUTABLE "g++")
#set(SRCBASE "Source/src")
#set(INCBASE "Source/include")
#set(LIBBASE "Source/lib")
#set(BINDIR  "./build/bin/${CMAKE_SYSTEM_NAME}")

include_directories(
./freetype/include
)

find_package(Freetype REQUIRED)


add_executable(${CMAKE_PROJECT_NAME}
           main.cpp
           freetype/CGL/CGL_Freetype.cpp
           freetype/CGL/CGL_Texture.cpp
           freetype/CGL/CGL_FrameBuffer.cpp
           freetype/CGL/NGLprimitives.cpp
           freetype/lib/CUtf.cpp
           freetype/lib/NUtf.cpp
           freetype/lib/NUtf2.cpp
           freetype/lib/NStringTool.cpp
           freetype/lib/CDataRect.cpp
           freetype/lib/CBlockOperations.cpp
           freetype/lib/write_png.cpp
           freetype/lib/PnglibInterface.c
)



if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    #
    # Windows
    #
    message("Building Windows..")
    target_link_libraries(${CMAKE_PROJECT_NAME}
        glu32        
        opengl32
        gdi32
        winmm
        glew32
        freetype
        glfw3
        png
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #
    # Linux
    #
    SET(SYSTEM_NAME "linux")
    target_link_libraries(${CMAKE_PROJECT_NAME}
        GL
        GLU
        GLEW
        png)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #
    # Mac OS
    #
    # *TODO*
    SET(SYSTEM_NAME "mac")
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
