
#include "gas_sensor.h"


/* ========== Global variables  ============ */
unsigned long prev_mq_time = 0;
unsigned long prev_led_time = 0;
bool led_state = LOW;

bool init_pass = true;           // flag to indicate that sensor initalization process passed successfully
gas_sensor mq(SENSOR_MQ4_E);     // create sensor object which will be accessible at every point



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  if (mq.gas_sensor_init()) {    // calculate all required sensor dependent parameters 
    Serial.printf("Error: Failed to initialize \"%s\" sensort type.\n", sensor_type2str(mq.sensor_type_get()));
    init_pass = false;
  }
}



void loop() {
  float ch4_val = 0, lpg_val = 0, co_val = 0;
  
  if (init_pass) {
    if (is_time_expired(0.3, prev_mq_time)) {
      switch (mq.sensor_type_get()) {
        case SENSOR_MQ4_E:
        case SENSOR_MQ5_E:
          ch4_val = mq.gas_sensor_value_get(MEASURE_GAS_CH4_E);
          lpg_val = mq.gas_sensor_value_get(MEASURE_GAS_LPG_E);
          Serial.printf("[%s] CH4 ppm: %.3f, LPG ppm: %.3f\n", sensor_type2str(mq.sensor_type_get()), ch4_val, lpg_val);  
        break;

        case SENSOR_MQ7_E:
          co_val = mq.gas_sensor_value_get(MEASURE_GAS_CO_E);
          Serial.printf("[%s] CO ppm: %.3f\n", sensor_type2str(mq.sensor_type_get()), co_val);  
        break;

        default:
          Serial.printf("\nError: sensor %s is not supported!\n", sensor_type2str(mq.sensor_type_get()));
      }
    }
  }



  if (is_time_expired(1, prev_led_time)) {
    led_state = (led_state == HIGH) ? LOW : HIGH;
    digitalWrite(LED_BUILTIN, led_state);    
  }
}
