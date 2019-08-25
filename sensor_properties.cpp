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
    {{200, 2.65}, {1000, 1.65}, {5000, 0.9}, {10000, 0.73}},   // LPG
    {},                                                        // CO is not supported by MQ4 - leave it empty
    4.4,                                                       // sensor ration for clear Air (per Datasheet graph)
    10.0,                                                      // RL resistance value, kOhm
  };

  sensor_properties_t mq5 {
    // Datasheet link - http://radiocom.dn.ua/image/data/pdf/MQ5-Datasheet.pdf
    2,
    {MEASURE_GAS_CH4_E, MEASURE_GAS_LPG_E},                    // supported gases
    5,
    {{200, 0.94}, {1000, 0.52}, {3000, 0.34}, {5000, 0.28}, {10000, 0.21}},  // CH4
    {{200, 0.7}, {1000, 0.365}, {3000, 0.23}, {5000, 0.19}, {10000, 0.15}},  // LPG 
    {},                                                        // CO is not supported by MQ5 - leave it empty
    6.4,                                                       // sensor ration for clear Air (per Datasheet graph)
    10.0,                                                      // RL resistance value, kOhm
  };


  sensor_properties_t mq7 {
    // Datasheet link - https://www.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FM%252FMQ-7.pdf
    1,
    {MEASURE_GAS_CO_E},                    // supported gases
    5,
    {},                                                               // CH4 is not supported by MQ7 - leave it empty
    {},                                                               // LPG is not supported by MQ7 - leave it empty
    {{50, 1.75}, {100, 1.0}, {400, 0.4}, {1000, 0.21}, {4000, 0.09}}, // CO
    28.4,                                                             // sensor ration for clear Air (per Datasheet graph)
    10.0,                                                             // RL resistance value, kOhm
  };

/*
   CO, ppm  |    Symptoms
   ---------------------------------------------------------------------------------------------------------------------------
   50       | No adverse effects with 8 hours of exposure.
   200      | Mild headache after 2-3 hours of exposure.
   400      | Headache and nausea after 1-2 hours of exposure.
   800      | Headache, nausea, and dizziness after 45 minutes; collapse and loss of consciousness after 1 hour of exposure.
   1,000    | Loss of consciousness after 1 hour of exposure.
   1,600    | Headache, nausea, and dizziness after 20 minutes of exposure.
   3,200    | Headache, nausea, and dizziness after 5-10 minutes; collapse and loss of consciousness after 30 minutes of exposure.
   6,400    | Headache and dizziness after 1-2 minutes; loss of consciousness and danger of death after 10-15 minutes of exposure.
   12,800   | Immediate physiological effects, loss of consciousness and danger of death after 1-3 minutes of exposure
*/


  switch (sensor_type) {
    
    case SENSOR_MQ4_E:
      memcpy(prop_p, &mq4, sizeof(sensor_properties_t));  
    break;

    case SENSOR_MQ5_E:
      memcpy(prop_p, &mq5, sizeof(sensor_properties_t));  
    break;

    case SENSOR_MQ7_E:
      memcpy(prop_p, &mq7, sizeof(sensor_properties_t));  
    break;
 
    default:
      rc = RC_UNSUPPORTED;
    break;
  }

  if (rc != RC_SUCCESS_E) {
    Serial.printf("Error: Failed to initialize \"%s\" sensor parameters. rc = %s.\n", sensor_type2str(sensor_type), rc2str(rc));
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
      return "Unknown"; 
  }
  return "N/A"; 
}

char *sensor_type2str(sensor_mq_e sensort_type) {
  switch (sensort_type) {
    case SENSOR_MQ2_E:
      return "MQ2";
    break;
    
    case SENSOR_MQ3_E:
      return "MQ3";
    break;

    case SENSOR_MQ4_E:
      return "MQ4";
    break;
    
    case SENSOR_MQ5_E:
      return "MQ5";
    break;

    case SENSOR_MQ6_E:
      return "MQ6";
    break;

    case SENSOR_MQ7_E:
      return "MQ7";
    break;
    
    default:
      return "Unknown";    
  }
  return "N/A";
}
