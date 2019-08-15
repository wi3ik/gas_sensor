#include "gas_sensor.h"

#include "Arduino.h" // probably can be removed later - for now it is required for debug


rc_t gas_sensor::gas_sensor_init(sensor_mq_e sensor_type) {
  rc_t rc = RC_SUCCESS_E;

  rc = sensor_properties_init(sensor_type, &this->params);
  validate_rc(rc, "sensor_properties_init");

  rc = this->gas_sensor_define_attr_value();
  validate_rc(rc, "gas_sensor_define_attr_value");
  
  return rc;
}

float gas_sensor::gas_sensor_value_get(measure_gas_type_e gas_type) {
  int gas_index = this->gas_sensor_type_index_get(gas_type);
  
  float raw_analog_data = this->gas_sensor_analog_value_get();          // Read analog values of sensor
  float volt = this->analog2volt(raw_analog_data);                      // Convert analog values to voltage

  float rs_gas = this->sensor_rs_get(volt);                             // Convert retrieved voltage value to relevant RS value
  float ratio = rs_gas/this->RO;                                        // Get ratio RS_gas/RS_air
  float ppm_log = (log10(ratio)-this->b[gas_index])/this->m[gas_index]; // Get ppm value in linear scale according to the the ratio value
  float ppm = pow(10, ppm_log);                                         // Convert ppm value to log scale

  return ppm;
}


float gas_sensor::gas_sensor_analog_value_get(int iterations) {
  float raw_value = 0;
  for (int i = iterations; i > 0; i--) {
     raw_value = raw_value + analogRead(MQ_PIN);
  }
  raw_value /= iterations;

  return raw_value;
}

float gas_sensor::analog2volt(float analog_value) {
  float val = 0;
  val =  (analog_value * (OPERATIONAL_VOLTAGE / MAX_ADC_VALUE));
  return val;
}

float gas_sensor::sensor_rs_get(float volt) {
  float val = 0;
  float rl_resistance = this->params.sensor_rl_resistance;
  val = ((OPERATIONAL_VOLTAGE * rl_resistance) / volt) - rl_resistance;
  return val;
}

float gas_sensor::gas_sensor_define_ro_value() {
  float raw_analog_data = this->gas_sensor_analog_value_get();  // Read analog values of sensor
  float volt = this->analog2volt(raw_analog_data);              // Convert analog values to voltage
  float rs = this->sensor_rs_get(volt);                         // Convert retrieved voltage value to relevant RS value 
                                                                // (it will be treated as value for clear air)
  float ro = rs / this->params.sensor_air_ratio;                // Calculate R0

  return ro;
}

int gas_sensor::gas_sensor_type_index_get(measure_gas_type_e gas_type) {
  int index = 0;
  bool found = false;
  for (int i = 0; i < this->params.gas_types_support_num; i++) {
    if (gas_type == this->params.supported_gases[i]) {
      found = true;
      break;
    }
    index++;
  }
  if (found == false) {
    index = -1;
    Serial.printf("\nError: index for gas_type: %d [%s] was not found!\n", gas_type, gas2str(gas_type));
  }
  return index;
}

rc_t gas_sensor::gas_sensor_define_m_and_b_values(measure_gas_type_e gas_type) {
  rc_t rc = RC_SUCCESS_E;
  int gas_index = 0;
  int middle_dot_index = (this->params.graph_dots_num - 1) / 2;
  graph_dots_t first = {}, middle = {}, last = {};
  graph_dots_t *cur_dots = NULL;
  
  switch (gas_type) {
    case MEASURE_GAS_CH4_E:
      cur_dots = this->params.graph_ch4_dots;
      break;

    case MEASURE_GAS_LPG_E:
      cur_dots = this->params.graph_lpg_dots;    
      break;

    default:
      Serial.printf("\nError: gas_type: %d [%s] is not supported!\n", gas_type, gas2str(gas_type));
      rc = RC_UNSUPPORTED;
  }

  if (rc == RC_SUCCESS_E) {
    memcpy(&first, &cur_dots[0], sizeof(graph_dots_t));
    memcpy(&middle, &cur_dots[middle_dot_index], sizeof(graph_dots_t));
    memcpy(&last, &cur_dots[this->params.graph_dots_num - 1], sizeof(graph_dots_t));
  
    gas_index = this->gas_sensor_type_index_get(gas_type);
  
    /* calculate m value */
    /*    m = log10(y2/y1) / log10(x2/x1)  */
    this->m[gas_index] = log10(last.y / first.y) / log10(last.x / first.x);
  
    /* calculate b value */
    /* b = log10(y) - m * log10(x); */
    this->b[gas_index] = log10(middle.y) - (this->m[gas_index] * log10(middle.x));
  }
  
  return rc;
}

rc_t gas_sensor::gas_sensor_define_attr_value() {
  rc_t rc = RC_SUCCESS_E;

  this->RO = this->gas_sensor_define_ro_value();

  /* in this loop we will go over all supported gas types and calculate appropriate m and b value */
  for (int i = 0; i < this->params.gas_types_support_num; i++) {
    rc = this->gas_sensor_define_m_and_b_values(this->params.supported_gases[i]); 
    validate_rc(rc, "gas_sensor_define_m_and_b_values");
        
  }
  
  return rc;
}
