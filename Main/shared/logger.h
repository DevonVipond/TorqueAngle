#pragma once
#include <string>
#include <vector>
namespace shared
{
    //template <class... Args>
    //void log(Args... args)
    //{
    //    static bool isLoggerInitialized = false;
    //    if (!isLoggerInitialized)
    //    {
    //        Serial.begin(115200);
    //        isLoggerInitialized = true;
    //    }

    //    Serial.println(args...);

    //    //for (auto& a : {args...})
    //    //{
    //    //    //Serial.print(static_cast<char*>(a));
    //    //    Serial.print(a);
    //    //}

    //    Serial.println("");
    //}

    void log() {}
    template <typename T, typename... Args>
    void log(T t, Args... args)
    {
        Serial.println(t);

        if (sizeof...(args))
            log(args...);
    }

}