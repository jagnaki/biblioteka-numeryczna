enable_testing()

# Dodaj executable dla testów
add_executable(biblioteka_tests
        test_interpolacja.cpp
        test_aproksymacja.cpp
        test_calkowanie.cpp
)

target_link_libraries(biblioteka_tests
        PRIVATE
        biblioteka_numeryczna
        GTest::gtest
        GTest::gtest_main
)

add_test(
        NAME biblioteka_tests
        COMMAND biblioteka_tests
)

# Włącz wykrywanie testów
include(GoogleTest)
gtest_discover_tests(biblioteka_tests
        DISCOVERY_TIMEOUT 120
)
