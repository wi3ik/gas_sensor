#ifndef __GAS_SENSOR_H__
#define __GAS_SENSOR_H__

#include "common.h"
#include "sensor_properties.h"


class gas_sensor {
  public:
    //gas_sensor();
    //virtual ~gas_sensor();
    rc_t gas_sensor_init(sensor_mq_e sensor_type);
    float gas_sensor_value_get(measure_gas_type_e gas_type);
    
  private:

    sensor_properties_t params;
    float RO;
    float m;
    float b;
  
    rc_t gas_sensor_define_attr_value();
    
    float gas_sensor_analog_value_get(int iterations = 200);
    float analog2volt(float analog_value);
    float sensor_rs_get(float volt);
    
    float gas_sensor_define_ro_value();
    float gas_sensor_define_m_value();
    float gas_sensor_define_b_value();    
};


#endif /* .__GAS_SENSOR_H__ */
