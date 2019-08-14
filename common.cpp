#include "common.h"

#include  "Arduino.h"

void validate_rc(rc_t rc, char *func_name) {
  if (rc != RC_SUCCESS_E) {
    Serial.printf("Failure at %s function, rc = %d [%s].", func_name, rc, rc2str(rc));
    //assert(0);
  }
}


char * rc2str(rc_t rc) {
  
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
