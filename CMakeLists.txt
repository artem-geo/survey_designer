cmake_minimum_required(VERSION 3.25)

project(survey-designer VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    shapelib
    GIT_REPOSITORY https://github.com/OSGeo/shapelib.git
    GIT_TAG v1.6.3
)

block()
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BUILD_SHARED_LIBS OFF)
    set(BUILD_APPS OFF)
    set(BUILD_SHAPELIB_CONTRIB OFF)
    set(BUILD_TESTING OFF)
    FetchContent_MakeAvailable(shapelib)
endblock()

add_executable(survey-designer)

target_compile_features(survey-designer PUBLIC cxx_std_20)

target_sources(survey-designer
    PRIVATE
        src/app.cpp
        src/line.cpp
        src/point.cpp
        src/rectangle.cpp
        src/shp_info.cpp
        src/survey_scheme.cpp
        src/utils.cpp
)

target_include_directories(survey-designer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(survey-designer
    PRIVATE
        shp
)
