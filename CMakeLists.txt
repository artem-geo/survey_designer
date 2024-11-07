cmake_minimum_required(VERSION 3.25)
project(SurveyDesigner)

include_directories(libs)
add_subdirectory(libs)

link_directories(
  "${PROJECT_SOURCE_DIR}/libs"
)

add_executable(survey_designer "main.cpp")

target_link_libraries(survey_designer
                        libshp
)


# target_include_directories(${PROJECT_NAME} PUBLIC libs)
# target_link_libraries(${PROJECT_NAME} libshp)

