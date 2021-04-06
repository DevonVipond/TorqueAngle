#pragma once

#include "../shared/types.h"
#include "message.h"

namespace shared
{

    class State
    {
    public:
        State() : _is_running(false) {}

        virtual void start() { _is_running = true; }
        virtual void stop() { _is_running = false; }
        virtual void handle_message(Message message) = 0;

    protected:
        volatile bool _is_running;
    };

}