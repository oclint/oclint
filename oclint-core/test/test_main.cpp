#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CanaryTest.h"
#include "RuleBaseTest.h"
#include "RuleConfigurationTest.h"
#include "RuleSetTest.h"
#include "ViolationSetTest.h"
#include "ViolationTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
