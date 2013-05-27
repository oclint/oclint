#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/Version.h"

using namespace ::testing;

TEST(VersionTest, VersionString)
{
    EXPECT_THAT(Version::identifier(), StrEq("0.7rc"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
