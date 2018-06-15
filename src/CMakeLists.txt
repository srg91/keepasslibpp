file(GLOB SOURCES *.cpp *.hpp)
add_library(keepasspp ${SOURCES})
target_link_libraries(keepasspp libgcrypt libargon2)
