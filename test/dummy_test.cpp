#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "dummy.h"

class DummyTest : public testing::Test {};

TEST_F(DummyTest, fooTest) { ASSERT_TRUE(true); }
