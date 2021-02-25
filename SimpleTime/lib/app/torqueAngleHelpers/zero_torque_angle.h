#pragma once

#include "../types/types.h"
#include "../../infra/time/getCurrentTime.h"

namespace app {

    // Singleton
    class ZeroTimeShift {
    private:
        timestamp _no_load_time_shift;
        static ZeroTimeShift *_instance;
        ZeroTimeShift(): _instance(nullptr);
    public:
        static ZeroTimeShift* instance() {
            if (_instance == nullptr) {
                _instance = ZeroTimeShift();
            }

            return _instance;
        }

        ~ZeroTimeShift() {
            delete _instance;
        }

        void zero(app::timestamp reference_point) {
            timestamp current_time = infra::get_current_time();

            timestamp shift = current_time - reference_point;

            _no_load_time_shift = (shift > 0 ? shift : -shift );
        }

        app::timestamp get() {
            return _no_load_time_shift;
        }
    };
}