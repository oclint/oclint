#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/Version.h"

using namespace ::testing;
using namespace oclint;

TEST(VersionTest, VersionString)
{
    EXPECT_THAT(Version::identifier(), StrEq("0.11.1"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
