#pragma once

#include <exception>

namespace infra {

    struct ConnectionFailed : public std::exception
    {
        const char * what () const throw ()
        {
            return "Failed to init a connection";
        }
    };

};
