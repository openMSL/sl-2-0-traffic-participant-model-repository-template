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
    target_velocity_ = 0.0;
}

void MyTrafficParticipantModel::Step(const osi3::SensorView& sensor_view_in,
                                     const osi3::TrafficCommand& traffic_command_in,
                                     osi3::TrafficUpdate& traffic_update_out,
                                     osi3::TrafficCommandUpdate& traffic_command_update_out,
                                     double time)
{
    double delta_time = time - last_time_step_;
    osi3::Identifier ego_id = sensor_view_in.global_ground_truth().host_vehicle_id();
    traffic_command_update_out.mutable_traffic_participant_id()->set_value(ego_id.value());

    if (traffic_command_in.action_size() > 0)
    {
        for (const osi3::TrafficAction& current_action : traffic_command_in.action())
        {
            // in this example, only speed actions are used
            if (current_action.has_speed_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                target_velocity_ = current_action.speed_action().absolute_target_speed();

                printf("Received speed action (id %u) started: objId %d, targetSpeed %.2f m/s, shape %d, duration %.2f s distance %.2f m\n",
                       static_cast<unsigned int>(current_action.speed_action().action_header().action_id().value()),
                       static_cast<unsigned int>(traffic_command_in.traffic_participant_id().value()),
                       current_action.speed_action().absolute_target_speed(),
                       static_cast<int>(current_action.speed_action().dynamics_shape()),
                       current_action.speed_action().duration(),
                       current_action.speed_action().distance());
            }
            // dismiss all other actions
            else if (current_action.has_follow_trajectory_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.follow_trajectory_action().action_header().action_id().value());
            }
            else if (current_action.has_follow_path_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.follow_path_action().action_header().action_id().value());
            }
            else if (current_action.has_acquire_global_position_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.acquire_global_position_action().action_header().action_id().value());
            }
            else if (current_action.has_lane_change_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.lane_change_action().action_header().action_id().value());
            }
            else if (current_action.has_abort_actions_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.abort_actions_action().action_header().action_id().value());
            }
            else if (current_action.has_end_actions_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.end_actions_action().action_header().action_id().value());
            }
            else if (current_action.has_custom_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.custom_action().action_header().action_id().value());
            }
            else if (current_action.has_longitudinal_distance_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.longitudinal_distance_action().action_header().action_id().value());
            }
            else if (current_action.has_lane_offset_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.lane_offset_action().action_header().action_id().value());
            }
            else if (current_action.has_lateral_distance_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.lateral_distance_action().action_header().action_id().value());
            }
            else if (current_action.has_teleport_action() && traffic_command_in.traffic_participant_id().value() == ego_id.value())
            {
                auto* dismissed_action = traffic_command_update_out.add_dismissed_action();
                dismissed_action->mutable_dismissed_action_id()->set_value(current_action.teleport_action().action_header().action_id().value());
            }
        }
    }

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
