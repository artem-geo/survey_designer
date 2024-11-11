cmake_minimum_required(VERSION 3.25)
project(SurveyDesigner)

set (CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/shapelib)
add_subdirectory(src)
link_directories(
  "${PROJECT_SOURCE_DIR}/libs/shapelib"
)
file(GLOB SOURCES "src/*.cpp")
add_executable(survey_designer "Application.cpp"
                                ${SOURCES})
                                include_directories("libs/shapelib"
                    "include")


target_link_libraries(survey_designer
                        libshp
)


# target_include_directories(${PROJECT_NAME} PUBLIC libs)
# target_link_libraries(${PROJECT_NAME} libshp)

