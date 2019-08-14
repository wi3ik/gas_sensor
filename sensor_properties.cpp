#include "sensor_properties.h"


#include "Arduino.h" // required for memcpy function 



rc_t sensor_properties_init(sensor_mq_e sensor_type, sensor_properties_t *prop_p) {
  rc_t rc = RC_SUCCESS_E;

  sensor_properties_t mq4 {
    // Datasheet link - https://www.pololu.com/file/0J311/MQ4.pdf
    "MQ4",
    4,
    {{200, 1.75}, {1000, 1}, {5000, 0.58}, {10000, 0.43}},     // CH4
    {{200, 2.65}, {1000, 1.65}, {5000, 0.9}, {10000, 0.73}},   // LPG <== TODO
    {},                                                        // CO is not supported by CH4
    4.4,
    10.0,
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
