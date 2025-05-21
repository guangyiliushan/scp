#ifndef TESTING_H
#define TESTING_H

// 简单的测试框架
#define TEST(name) void test_##name()
#define RUN_TEST(name) printf("Running test: %s\n", #name); test_##name(); printf("Test %s completed\n\n", #name);

#define ASSERT_TRUE(condition) do { \
    if (!(condition)) { \
        printf("Assertion failed: %s, file %s, line %d\n", #condition, __FILE__, __LINE__); \
        abort(); \
    } \
} while (0)

#define ASSERT_EQUAL(expected, actual) do { \
    if ((expected) != (actual)) { \
        printf("Assertion failed: %s == %s, file %s, line %d\n", #expected, #actual, __FILE__, __LINE__); \
        printf("  Expected: %d\n  Actual: %d\n", (expected), (actual)); \
        abort(); \
    } \
} while (0)

#define ASSERT_STR_EQUAL(expected, actual) do { \
    if (strcmp((expected), (actual)) != 0) { \
        printf("Assertion failed: %s == %s, file %s, line %d\n", #expected, #actual, __FILE__, __LINE__); \
        printf("  Expected: %s\n  Actual: %s\n", (expected), (actual)); \
        abort(); \
    } \
} while (0)

#endif // TESTING_H
