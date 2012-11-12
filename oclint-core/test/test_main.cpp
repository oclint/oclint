#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CanaryTest.h"
#include "ProcessorTest.h"
#include "RuleBaseTest.h"
#include "RuleConfigurationTest.h"
#include "RuleSetTest.h"
#include "ViolationNodeTest.h"
#include "ViolationSetTest.h"
#include "ViolationTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
