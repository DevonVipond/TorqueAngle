#pragma once
#include "message.h"

namespace shared
{
    class Observer
    {
    public:
        virtual ~Observer() {}
        virtual void observerCallback(Message message) = 0;
    };
}