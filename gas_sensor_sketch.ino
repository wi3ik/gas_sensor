
#include "gas_sensor.h"


// create sensor object which will be accessible at every point
gas_sensor mq;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  mq.gas_sensor_init(SENSOR_MQ4_E);


}


unsigned long prev_mq_time = 0;
unsigned long prev_led_time = 0;
bool led_state = LOW;


void loop() {
  if (is_time_expired(0.3, prev_mq_time)) {
    float value = mq.gas_sensor_value_get(MEASURE_CH4_E); // TODO - gas type is not important for now
    Serial.printf("CH4 ppm: %.3f\n", value);
  }



  if (is_time_expired(1, prev_led_time)) {
    led_state = (led_state == HIGH) ? LOW : HIGH;
    digitalWrite(LED_BUILTIN, led_state);    
  }
}
