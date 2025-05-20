cmake_minimum_required(VERSION 3.16)
project(AppName)

# Windows specific config
IF (WIN32)
    # Include local sfml cmake config
    set(SFML_DIR "C:/lib/SFML-2.5.1/lib/cmake/SFML")
    # Link sfml statically (Optional)
    set(SFML_STATIC_LIBRARIES TRUE)
ENDIF()

set (CMAKE_CXX_FLAGS "-g -W -pedantic")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

FILE(
    GLOB
    SOURCES
    "src/*"
)

# Additional resources like images and fonts
configure_file(res/OpenSans-Regular.ttf OpenSans-Regular.ttf COPYONLY)

add_executable(prog ${SOURCES})

target_link_libraries(prog sfml-graphics sfml-audio)

target_include_directories(prog
    PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
