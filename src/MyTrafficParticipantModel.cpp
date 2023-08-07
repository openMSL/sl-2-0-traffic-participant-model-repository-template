//
// Copyright 2016 -- 2018 PMSF IT Consulting Pierre R. Mai
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
// The MPL-2.0 is only an example here. You can choose any other open source license accepted by OpenMSL, or any other license if this template is used elsewhere.
//

#include <cmath>

#include "MyTrafficParticipantModel.h"
#include "osi_sensorview.pb.h"

void MyTrafficParticipantModel::Init()
{
    acceleration_m_s_ = 1.0;
    last_time_step_ = 0;
    max_velocity_ = 13.89;
}

osi3::TrafficUpdate MyTrafficParticipantModel::Step(const osi3::SensorView& current_in, double time)
{
    osi3::TrafficUpdate current_out;

    double delta_time = time - last_time_step_;

    osi3::Identifier ego_id = current_in.global_ground_truth().host_vehicle_id();
    for (const osi3::MovingObject& obj : current_in.global_ground_truth().moving_object())
    {
        if (obj.id().value() == ego_id.value())
        {
            // Simple constant acceleration model
            auto *update = current_out.add_update();
            double velocity = obj.base().velocity().x();
            double new_velocity = velocity + acceleration_m_s_ * delta_time;
            if (new_velocity < max_velocity_)   //check if new velocity is lower than the set maximum
            {
                velocity = new_velocity;
            }
            update->mutable_base()->mutable_velocity()->set_x(velocity);
            update->mutable_base()->mutable_position()->set_x(CalcNewPosition(obj.base().position().x(), velocity, delta_time));
        }
    }

    return current_out;
}

double MyTrafficParticipantModel::CalcNewPosition(double current_position, double velocity, double delta_time)
{
    double delta_x = velocity * delta_time;
    return current_position + delta_x;
}
