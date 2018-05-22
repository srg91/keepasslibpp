file(GLOB SOURCES *.cpp *.hpp)
add_library(keepasslib ${SOURCES})
target_link_libraries(keepasslib ${Boost_LIBRARIES})
