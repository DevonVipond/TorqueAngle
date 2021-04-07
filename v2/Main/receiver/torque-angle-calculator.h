#pragma once

#include <vector>
#include <limits.h>
#include <algorithm>
#include "../shared/logger.h"
#include "../shared/types.h"

using namespace types;
using namespace shared;

namespace receiver
{

    class TorqueAngleCalculator
    {
    private:
        timestamp _zerodTimeShift = 0;

        timestamp calculateTimeShift(timestamp t1, timestamp t2) { return (t1 > t2) ? t1 - t2 : t2 - t1; }

        timestamp timeShiftForReferencePoint(const timestamp &referencePoint, const std::vector<timestamp> &zeroCrossings)
        {
            auto index = 0;
            for (; index < zeroCrossings.size(); index++) {
                if (zeroCrossings[index] > referencePoint)
                    break;
            }

            if (index == 0 || index >= zeroCrossings.size()) {
                throw std::exception();
            }

            return calculateTimeShift(zeroCrossings[index-1], referencePoint);
        }

        double convertTimeShiftToTorqueAngle(timestamp timeShift) {
            return (90.0 / 4000.0) * static_cast<double>(timeShift);
        }

    public:
        void zeroTorqueAngle(std::vector<timestamp> referencePoints, std::vector<timestamp> zeroCrossings)
        {
            log("zeroTorqueAngle coming soon!");
        }

        double calculate(std::vector<timestamp> &&referencePoints, std::vector<timestamp> &&zeroCrossings)
        {
            timestamp minTimeShift = std::numeric_limits<timestamp>::max();
            for (const timestamp &point : referencePoints)
            {
                try {

                    auto timeShift = timeShiftForReferencePoint(point, zeroCrossings);
                    minTimeShift = std::min(minTimeShift, calculateTimeShift(timeShift, _zerodTimeShift));

                } catch (std::exception) {}
            }

            if (minTimeShift == std::numeric_limits<timestamp>::max()) throw std::exception();

            return convertTimeShiftToTorqueAngle(minTimeShift);
        }
    };

}