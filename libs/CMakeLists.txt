message("Linking shapelib ... ")
add_library(shapelib SHARED IMPORTED)
set_property(TARGET shapelib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/libs/libshp.dll")
set_property(TARGET shapelib PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/libs/libshp.dll.a")


