
#ifndef TYPES_H // include guard
#define TYPES_H

namespace app{
    typedef unsigned timestamp;
    typedef unsigned TorqueAngle;
    typedef unsigned SensorMeasurement;
    typedef unsigned int TerminalVoltageMeasurement;

    class TerminalVoltagePoint {
    private:
        timestamp _timestamp;
        TerminalVoltageMeasurement _measurement;

    public:
        TerminalVoltagePoint(timestamp time, TerminalVoltageMeasurement measurement):
                _timestamp(time),
                _measurement(measurement) { }

        timestamp get_timestamp() { return _timestamp;}
        unsigned int get_measurement() { return _measurement;}
    };
};


#endif