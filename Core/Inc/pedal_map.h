#ifndef PEDAL_MAP_H
#define PEDAL_MAP_H
#include "functions.h"
#include <stdint.h>

#define PEDAL_MAX_VALUE 1000U
#define PEDAL_LUT_STEP 100U
#define PEDAL_LUT_SIZE 9U
#define PEDAL_PROFILE_SLOTS 10U
#define PEDAL_TRANSFER_DONE 0xFFU
#define PEDAL_PROFILE_SIZE (PEDAL_LUT_SIZE * 2)
#define PEDAL_PROFILES_TOT_SIZE (PEDAL_PROFILE_SIZE * PEDAL_PROFILE_SLOTS)

#define CAN_CHANGE_PEDAL_PROFILE (0x17)
#define CAN_ADD_PEDAL_PROFILE (0x18)
#define CAN_PEDAL_SAVE_FLASH (0x19)

typedef enum pedal_profile
{
  LINEAR,
  PARABOLIC,
  SOFT,
  STUPID,
} pedal_profile;

static const uint16_t pedal_profile_linear[PEDAL_LUT_SIZE] = {100, 200, 300, 400, 500,
                                                              600, 700, 800, 900};
static const uint16_t pedal_profile_parabolic[PEDAL_LUT_SIZE] = {316, 447, 548, 632, 707,
                                                                 775, 837, 894, 949};
static const uint16_t pedal_profile_soft[PEDAL_LUT_SIZE] = {10,  40,  90,  160, 250,
                                                            360, 490, 640, 810};
static const uint16_t pedal_profile_stupid[PEDAL_LUT_SIZE] = {20,  60,  120, 200, 320,
                                                              460, 620, 780, 910};

extern const uint16_t* curr_profile;
void init_pedal_map(void);
uint16_t pedal_map(uint16_t pedal);
uint16_t pedal_map_get_percentage();

void change_pedal_profile(pedal_profile profile);
uint8_t add_pedal_profile(void* values, uint8_t slot, uint8_t size);
void process_pedal_profile_add(CAN_Message msg);
void process_pedal_profile_change(CAN_Message msg);
void process_pedal_flash_save(CAN_Message msg);

#endif
