#pragma once
namespace shared
{

    template <class... Args>
    void log(Args... args)
    {
        for (a in args)
        {
            Serial.print(a);
        }

        Serial.println("");
    }

}