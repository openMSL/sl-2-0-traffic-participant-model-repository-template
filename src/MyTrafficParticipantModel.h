//
// Copyright 2016 -- 2018 PMSF IT Consulting Pierre R. Mai
// Copyright 2023 BMW AG
// SPDX-License-Identifier: MPL-2.0
// The MPL-2.0 is only an example here. You can choose any other open source license accepted by OpenMSL, or any other license if this template is used elsewhere.
//

#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "OSMPConfig.h"
#include "fmi2FunctionTypes.h"
#include "fmi2Functions.h"
#include "osi_sensordata.pb.h"
#include "osi_trafficupdate.pb.h"

using namespace std;

class MyTrafficParticipantModel
{
  public:
    void Init(string theinstance_name, fmi2CallbackFunctions thefunctions, bool thelogging_on);
    osi3::TrafficUpdate Step(const osi3::SensorView& current_in, double time);

    static double CalcNewPosition(double current_position, double velocity, double delta_time);

  private:
    double acceleration_m_s_;
    double last_time_step_;
    double max_velocity_;

    string instance_name_;
    bool logging_on_;
    set<string> logging_categories_;
    fmi2CallbackFunctions functions_;

    /* Private File-based Logging just for Debugging */
#ifdef PRIVATE_LOG_PATH
    static ofstream private_log_file;
#endif

    static void FmiVerboseLogGlobal(const char* format, ...)
    {
#ifdef VERBOSE_FMI_LOGGING
#ifdef PRIVATE_LOG_PATH
        va_list ap;
        va_start(ap, format);
        char buffer[1024];
        if (!private_log_file.is_open())
        {
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        }
        if (private_log_file.is_open())
        {
#ifdef _WIN32
            vsnprintf_s(buffer, 1024, format, ap);
#else
            vsnprintf(buffer, 1024, format, ap);
#endif
            private_log_file << "OSMPDummySensor"
                             << "::Global:FMI: " << buffer << endl;
            private_log_file.flush();
        }
#endif
#endif
    }

    void InternalLog(const char* category, const char* format, va_list arg)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        char buffer[1024];
#ifdef _WIN32
        vsnprintf_s(buffer, 1024, format, arg);
#else
        vsnprintf(buffer, 1024, format, arg);
#endif
#ifdef PRIVATE_LOG_PATH
        if (!private_log_file.is_open())
        {
            private_log_file.open(PRIVATE_LOG_PATH, ios::out | ios::app);
        }

        if (private_log_file.is_open())
        {
            private_log_file << "OSMPDummySensor"
                             << "::"
                             << "template"
                             << "<" << ((void*)this) << ">:" << category << ": " << buffer << endl;
            private_log_file.flush();
        }
#endif
#ifdef PUBLIC_LOGGING
        if (logging_on_ && logging_categories_.count(category))
        {
            functions_.logger(functions_.componentEnvironment, instance_name_.c_str(), fmi2OK, category, buffer);
        }
#endif
#endif
    }

    void FmiVerboseLog(const char* format, ...)
    {
#if defined(VERBOSE_FMI_LOGGING) && (defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING))
        va_list ap;
        va_start(ap, format);
        InternalLog("FMI", format, ap);
        va_end(ap);
#endif
    }

    /* Normal Logging */
    void NormalLog(const char* category, const char* format, ...)
    {
#if defined(PRIVATE_LOG_PATH) || defined(PUBLIC_LOGGING)
        va_list ap;
        va_start(ap, format);
        InternalLog(category, format, ap);
        va_end(ap);
#endif
    }
};