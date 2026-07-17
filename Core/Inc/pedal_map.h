#ifndef PEDAL_MAP_H
#define PEDAL_MAP_H
#include "functions.h"
#include <stdint.h>

#define PEDAL_MAX_VALUE 1000U
#define PEDAL_LUT_STEP 100U
#define PEDAL_LUT_SIZE 11U
#define PEDAL_CONFIG_SLOTS 20U

#define CAN_CHANGE_PEDAL_PROFILE (0x17)
#define CAN_ADD_PEDAL_PROFILE (0x18)

typedef enum pedal_curve
{
  LINEAR,
  PARABOLIC,
  SOFT,
  STUPID,
} pedal_curve;

static const uint16_t pedal_curve_linear[PEDAL_LUT_SIZE] = {0,   100, 200, 300, 400, 500,
                                                            600, 700, 800, 900, 1000};

static const uint16_t pedal_curve_aggressive[PEDAL_LUT_SIZE] = {0,   316, 447, 548, 632, 707,
                                                                775, 837, 894, 949, 1000};

static const uint16_t pedal_curve_soft[PEDAL_LUT_SIZE] = {0,   10,  40,  90,  160, 250,
                                                          360, 490, 640, 810, 1000};

static const uint16_t pedal_curve_stupid[PEDAL_LUT_SIZE] = {0,   20,  60,  120, 200, 320,
                                                            460, 620, 780, 910, 1000};
uint16_t pedal_map(uint16_t pedal);
uint16_t pedal_map_get_percentage();
extern const uint16_t* curr_curve;

void change_pedal_curve(pedal_curve curve);
uint8_t add_pedal_curve(uint8_t values[], uint8_t slot, uint8_t size);
void process_pedal_config(CAN_Message msg);

#endif
