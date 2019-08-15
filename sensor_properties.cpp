#include "sensor_properties.h"


#include "Arduino.h" // required for memcpy function 



rc_t sensor_properties_init(sensor_mq_e sensor_type, sensor_properties_t *prop_p) {
  rc_t rc = RC_SUCCESS_E;

  sensor_properties_t mq4 {
    // Datasheet link - https://www.pololu.com/file/0J311/MQ4.pdf
    2,
    {MEASURE_GAS_CH4_E, MEASURE_GAS_LPG_E},                    // supported gases
    4,
    {{200, 1.75}, {1000, 1}, {5000, 0.58}, {10000, 0.43}},     // CH4
    {{200, 2.65}, {1000, 1.65}, {5000, 0.9}, {10000, 0.73}},   // LPG <== TODO
    {},                                                        // CO is not supported by MQ4 - leave it empty
    4.4,                                                       // sensor ration for clear Air (per Datasheet graph)
    10.0,                                                      // RL resistance value, kOhm
  };


  switch (sensor_type) {
    
    case SENSOR_MQ4_E:
      memcpy(prop_p, &mq4, sizeof(sensor_properties_t));  
    break;
    
    default:
      rc = RC_UNSUPPORTED;
    break;
  }
  
  
  return rc;
}


char *gas2str(measure_gas_type_e gas_type) {
  switch (gas_type) {
   
    case MEASURE_GAS_CH4_E:
      return "CH4";        
    break;
    
    case MEASURE_GAS_LPG_E:
      return "LPG";    
    break;

    case MEASURE_GAS_CO_E:
      return "CO";
    break;
    
    default:
      return "N/A"; 
  }
  return "N/A"; 
}
