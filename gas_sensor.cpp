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
  float raw_analog_data = this->gas_sensor_analog_value_get();  // Read analog values of sensor
  float volt = this->analog2volt(raw_analog_data);              // Convert analog values to voltage

  float rs_gas = this->sensor_rs_get(volt);                     // Convert retrieved voltage value to relevant RS value
  float ratio = rs_gas/this->RO;                                // Get ratio RS_gas/RS_air
  float ppm_log = (log10(ratio)-this->b)/this->m;               // Get ppm value in linear scale according to the the ratio value
  float ppm = pow(10, ppm_log);                                 // Convert ppm value to log scale

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

float gas_sensor::gas_sensor_define_m_value() {
  float val = 0;
  graph_dots_t first = {}, last = {};
  memcpy(&first, &this->params.graph_ch4_dots[0], sizeof(graph_dots_t));
  memcpy(&last, &this->params.graph_ch4_dots[this->params.graph_dots_num - 1], sizeof(graph_dots_t));

  /*    m = log10(y2/y1) / log10(x2/x1)  */
  val = log10(last.y / first.y) / log10(last.x / first.x);

  return val;
}

float gas_sensor::gas_sensor_define_b_value() {
  float val = 0;
  graph_dots_t point = {};
  int middle_dot_index = (this->params.graph_dots_num - 1) / 2;
  memcpy(&point, &this->params.graph_ch4_dots[middle_dot_index], sizeof(graph_dots_t));

  /* b = log10(y) - m * log10(x); */
  val = log10(point.y) - (this->m * log10(point.x));

  return val;
}

rc_t gas_sensor::gas_sensor_define_attr_value() {
  rc_t rc = RC_SUCCESS_E;

  this->RO = this->gas_sensor_define_ro_value();
  this->m = this->gas_sensor_define_m_value();
  this->b = this->gas_sensor_define_b_value();

  return rc;
}
