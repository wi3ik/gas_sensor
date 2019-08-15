
#include "gas_sensor.h"


/* ========== Global variables  ============ */
unsigned long prev_mq_time = 0;
unsigned long prev_led_time = 0;
bool led_state = LOW;


gas_sensor mq; // create sensor object which will be accessible at every point



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  mq.gas_sensor_init(SENSOR_MQ4_E);
}



void loop() {
  if (is_time_expired(0.3, prev_mq_time)) {
    float ch4_val = mq.gas_sensor_value_get(MEASURE_GAS_CH4_E);
    float lpg_val = mq.gas_sensor_value_get(MEASURE_GAS_LPG_E);
    Serial.printf("CH4 ppm: %.3f, LPG ppm: %.3f\n", ch4_val, lpg_val);    
  }



  if (is_time_expired(1, prev_led_time)) {
    led_state = (led_state == HIGH) ? LOW : HIGH;
    digitalWrite(LED_BUILTIN, led_state);    
  }
}

/*
- buzzer
- MQ5
- CO 
*/
