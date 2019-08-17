#include "common.h"

#include  "Arduino.h"




char *rc2str(rc_t rc) {
  
  switch (rc) {
   
    case RC_SUCCESS_E:
      return "SUCCESS";        
    break;
    
    case RC_FAILURE_E:
      return "FAILURE";    
    break;

    case RC_UNSUPPORTED:
      return "UNSUPPORTED";
    break;
    
    default:
      return "N/A"; 
  }
  return "N/A";
}


bool is_time_expired(float period, unsigned long &prev_time) {
  float interval = period * 1000;
  bool res = false;
  unsigned long cur_time = millis();
  if ((cur_time - prev_time) >= interval) {
    res = true;

    // update prev_time value
    prev_time = millis();
  }
  return res;
}
