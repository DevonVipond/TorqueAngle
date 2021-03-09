#pragma once
#include <exception>

namespace app {
    struct CalculationFailed : public std::exception
    {
        const char * what () const throw ()
        {
            return "Failed to calculate torque angle!";
        }
    };
}