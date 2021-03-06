/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_INERTIAL_SENSOR_L3G4200D_H__
#define __AP_INERTIAL_SENSOR_L3G4200D_H__

#include <AP_HAL/AP_HAL.h>
#if CONFIG_HAL_BOARD == HAL_BOARD_LINUX

#include <pthread.h>

#include "AP_InertialSensor.h"
#include <Filter/Filter.h>
#include <Filter/LowPassFilter2p.h>

class AP_InertialSensor_L3G4200D : public AP_InertialSensor_Backend
{
public:

    AP_InertialSensor_L3G4200D(AP_InertialSensor &imu);
    ~AP_InertialSensor_L3G4200D();

    /* update accel and gyro state */
    bool update();

    bool gyro_sample_available(void) { return _have_sample; }
    bool accel_sample_available(void) { return _have_sample; }

    // detect the sensor
    static AP_InertialSensor_Backend *detect(AP_InertialSensor &imu);

    // return product ID
    int16_t product_id(void) const { return AP_PRODUCT_ID_L3G4200D; }

private:
    bool            _init_sensor(void);
    void            _accumulate(void);

    struct {
        Vector3f accel_filtered;
        Vector3f gyro_filtered;
    } _data[2];
    int _data_idx;
    pthread_spinlock_t _data_lock;

    bool _have_gyro_sample;
    bool _have_accel_sample;
    volatile bool _have_sample;

    // support for updating filter at runtime
    uint8_t         _last_filter_hz;

    void _set_filter_frequency(uint8_t filter_hz);

    // Low Pass filters for gyro and accel 
    LowPassFilter2pVector3f _accel_filter;
    LowPassFilter2pVector3f _gyro_filter;

    // gyro and accel instances
    uint8_t _gyro_instance;
    uint8_t _accel_instance;
};
#endif
#endif // __AP_INERTIAL_SENSOR_L3G4200D_H__
