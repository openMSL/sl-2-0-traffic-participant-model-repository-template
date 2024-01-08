//
// Copyright 2016 -- 2018 PMSF IT Consulting Pierre R. Mai
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
// The MPL-2.0 is only an example here. You can choose any other open source license accepted by OpenMSL, or any other license if this template is used elsewhere.
//

#include "MyTrafficParticipantModel.h"

#include <cmath>

#include "osi_sensorview.pb.h"

void MyTrafficParticipantModel::Init(std::string theinstance_name, fmi2CallbackFunctions thefunctions, bool thelogging_on)
{
    logging_categories_.clear();
    logging_categories_.insert("FMI");
    logging_categories_.insert("OSMP");
    logging_categories_.insert("OSI");
    instance_name_ = theinstance_name;
    functions_ = thefunctions;
    logging_on_ = thelogging_on;

    acceleration_m_s_ = 1.0;
    last_time_step_ = 0;
    target_velocity_ = 8.333;  // set 30 km/h as default target velocity of the ego vehicle
}

void MyTrafficParticipantModel::Step(const osi3::SensorView& sensor_view_in,
                                     const osi3::TrafficCommand& traffic_command_in,
                                     osi3::TrafficUpdate& traffic_update_out,
                                     osi3::TrafficCommandUpdate& traffic_command_update_out,
                                     double time)
{
    double delta_time = time - last_time_step_;

    if (traffic_command_in.action_size() > 0)
    {
        for (const osi3::TrafficAction& current_action : traffic_command_in.action())
        {
            if (current_action.has_speed_action())  // in this example, only speed actions are used
            {
                target_velocity_ = current_action.speed_action().absolute_target_speed();
            }
        }
    }

    osi3::Identifier ego_id = sensor_view_in.global_ground_truth().host_vehicle_id();
    for (const osi3::MovingObject& obj : sensor_view_in.global_ground_truth().moving_object())
    {
        if (obj.id().value() == ego_id.value())
        {
            // Simple constant acceleration model
            auto* update = traffic_update_out.add_update();
            double velocity = obj.base().velocity().x();
            double new_velocity = velocity + acceleration_m_s_ * delta_time;
            if (new_velocity <= target_velocity_)  // check if new velocity is lower than the set maximum
            {
                velocity = new_velocity;
            }
            update->mutable_base()->mutable_velocity()->set_x(velocity);
            update->mutable_base()->mutable_position()->set_x(CalcNewPosition(obj.base().position().x(), velocity, delta_time));
        }
    }

    last_time_step_ = time;
}

double MyTrafficParticipantModel::CalcNewPosition(double current_position, double velocity, double delta_time)
{
    double delta_x = velocity * delta_time;
    return current_position + delta_x;
}
