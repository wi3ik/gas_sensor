
#include "gas_sensor.h"


// create sensor object which will be accessible at every point
gas_sensor mq;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  mq.gas_sensor_init(SENSOR_MQ4_E);


}

unsigned long prev_millis = 0;
bool led_state = LOW;
void bling(float n); // blink every X secconds

void loop() {

  float value = mq.gas_sensor_value_get(MEASURE_CH4_E); // TODO - gas type is not important for now
  Serial.printf("CH4 ppm: %.3f\n", value);
  delay(1000);
  //blink(1); // TODO - use the same logic for quering GAS sensor
}

void blink(float n) {
  unsigned long interval = n * 1000; 
  unsigned long cur_millis = millis();
  
  if (cur_millis - prev_millis >= interval) {
    led_state = (led_state == HIGH) ? LOW : HIGH;
    //led_state ^= led_state;
    digitalWrite(LED_BUILTIN, led_state);
    prev_millis = millis();
  }
  
}
