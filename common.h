#ifndef __COMMON_H__
#define __COMMON_H__

/* ========== DEFINEs ============ */
/*
    D0   = 16;
    D1   = 5;
    D2   = 4;
    D3   = 0;
    D4   = 2;
    D5   = 14;
    D6   = 12;
    D7   = 13;
    D8   = 15;
    D9   = 3;
    D10  = 1;
    A0   = 17
*/
#define MQ_PIN A0
#define OPERATIONAL_VOLTAGE (5.0)
#define MAX_ADC_VALUE       (1024)    // == (1 << 10) - in this case 10 represents MAX number bits for ADC 
/* ========== ENUMs ============ */
typedef enum rc {
  RC_SUCCESS_E = 0,
  RC_FAILURE_E = 1,
  RC_UNSUPPORTED,
} rc_t;

/* ========== STRUCTs ============ */
typedef struct graph_dots {
  float x;
  float y;
} graph_dots_t;

/* ========== FUNCTIONs ============ */
void validate_rc(rc_t rc, char *func_name);

char * rc2str(rc_t rc);

#endif /* __COMMON_H__ */
