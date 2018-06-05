file(GLOB SOURCES *.cpp *.hpp)
add_library(keepasslibpp ${SOURCES})
target_link_libraries(keepasslibpp libgcrypt libargon2)
