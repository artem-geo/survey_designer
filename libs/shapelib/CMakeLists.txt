message("Linking shapelib ... ")
add_library(shapelib SHARED IMPORTED)
set_property(TARGET shapelib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/libs/shapelib/libshp.dll")
set_property(TARGET shapelib PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/libs/shapelib/libshp.dll.a")


