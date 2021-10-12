#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "oclint/Constants.h"

using namespace ::testing;
using namespace oclint;

TEST(VersionTest, VersionString)
{
    EXPECT_THAT(Constants::version(), StrEq("21.10"));
}

TEST(VersionTest, HomepageString)
{
    EXPECT_THAT(Constants::homepage(), StrEq("https://oclint.org"));
}

TEST(VersionTest, DocpageString)
{
    EXPECT_THAT(Constants::docpage(), StrEq("https://docs.oclint.org/en/stable"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
