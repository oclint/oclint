#include "TestHeaders.h"

TEST(CanaryTest, AlwaysTrue) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
