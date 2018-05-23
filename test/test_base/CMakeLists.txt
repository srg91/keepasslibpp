file(GLOB TEST_BASE_SOURCES *.cpp *.hpp)
add_executable(test_base ${TEST_BASE_SOURCES})
target_link_libraries(test_base ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_SYSTEM_LIBRARY})

