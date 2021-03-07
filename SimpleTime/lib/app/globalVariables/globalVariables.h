
#pragma once

#include "../types/types.h"

namespace app {

    class Globals {
        public:

        long int g_clock_offset;
        app::timestamp g_no_load_time_shift;

        Globals* _instance = nullptr;

        Globals* instance() {

            if (_instance == nullptr) {
                _instance = Globals();
            }

            return _instance;
        }

        private:

        Globals() {

            g_clock_offset = 0;
            g_no_load_time_shift = 0;
        }
    }

};