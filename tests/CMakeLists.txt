enable_testing()

find_package(GTest REQUIRED)

add_executable(LaravelGateTest
    main.cpp
)

target_compile_definitions(LaravelGateTest PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/testdata")

target_link_libraries(LaravelGateTest PRIVATE GTest::gtest Qt::Test LaravelGate)
