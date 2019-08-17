#ifndef __GAS_SENSOR_H__
#define __GAS_SENSOR_H__

#include "common.h"
#include "sensor_properties.h"


class gas_sensor {
  private:
    gas_sensor() {};  // empty constructor which is not allowed
    
  public:
    gas_sensor(sensor_mq_e sensor_type);
    //virtual ~gas_sensor();
    rc_t gas_sensor_init();
    float gas_sensor_value_get(measure_gas_type_e gas_type);
    
  private:
    sensor_mq_e sensor_type;
    sensor_properties_t params;
    
    float RO;
    float m[MEASURE_GAS_MAX_E + 1];  //per gas type // TODO - allocate correct memory based on value of supported gases + release in destructor
    float b[MEASURE_GAS_MAX_E + 1];
  
    rc_t gas_sensor_define_attr_value();
    
    float gas_sensor_analog_value_get(int iterations = 200);
    float analog2volt(float analog_value);
    float sensor_rs_get(float volt);
    
    float gas_sensor_define_ro_value();
    rc_t gas_sensor_define_m_and_b_values(measure_gas_type_e gas_type);

    int gas_sensor_type_index_get(measure_gas_type_e gas_type);    
};


#endif /* .__GAS_SENSOR_H__ */
