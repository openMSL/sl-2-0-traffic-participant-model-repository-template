//
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
//

#include <cmath>

#include "../../src/MyTrafficParticipantModel.h"
#include "gtest/gtest.h"

class IModelTest : public ::testing::Test
{
};

TEST_F(IModelTest, NewPosition)
{

    const double current_position = 10.0;
    const double velocity = 5;
    const double delta_time = 0.1;

    double position = MyTrafficParticipantModel::CalcNewPosition(current_position, velocity, delta_time);

    EXPECT_NEAR(position, 10.5, 0.001);
}
