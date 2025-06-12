# Dodaj executable dla testów
add_executable(test_interpolacji
        test_interpolacja.cpp
)

target_link_libraries(test_interpolacji
        PRIVATE
        biblioteka_numeryczna
        GTest::gtest_main
)

# Włącz wykrywanie testów
include(GoogleTest)
gtest_discover_tests(test_interpolacji)