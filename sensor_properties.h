#ifndef __SENSOR_PROPERTIES_H__
#define __SENSOR_PROPERTIES_H__

#include "common.h"

/* ========== DEFINEs ============ */
#define MAX_GRAPH_DOTS     (10)

/* ========== ENUMs ============ */
typedef enum sensor_mq {
  SENSOR_MQ2_E = 0,
  SENSOR_MQ3_E,  
  SENSOR_MQ4_E,
  SENSOR_MQ5_E,
  SENSOR_MQ6_E,
  SENSOR_MQ7_E,
  SENSOR_MQ_MAX_E = SENSOR_MQ7_E,
} sensor_mq_e;

typedef enum measure_gas_type {
  MEASURE_GAS_CH4_E = 0x01,
  MEASURE_GAS_LPG_E = 0x02,
  MEASURE_GAS_CO_E  = 0x03,
  MEASURE_GAS_MAX_E = MEASURE_GAS_CO_E,
  
} measure_gas_type_e;


/* ========== STRUCTs ============ */
typedef struct sensor_properties {
  int gas_types_support_num;
  measure_gas_type_e supported_gases[MEASURE_GAS_MAX_E + 1];
  int graph_dots_num;                            // get these dots from Datasheet graph
  graph_dots_t graph_ch4_dots[MAX_GRAPH_DOTS];
  graph_dots_t graph_lpg_dots[MAX_GRAPH_DOTS];
  graph_dots_t graph_co_dots[MAX_GRAPH_DOTS];   
  float sensor_air_ratio;
  float sensor_rl_resistance;

} sensor_properties_t;


/* ========== FUNCTIONs ============ */
rc_t sensor_properties_init(sensor_mq_e sensor_type, sensor_properties_t *prop_p);

char *gas2str(measure_gas_type_e gas_type);


#endif /* __SENSOR_PROPERTIES__ */
