#ifndef PEDAL_MAP_H
#define PEDAL_MAP_H
#include "functions.h"
#include <stdint.h>

#define PEDAL_INPUT_MAX 1000U
#define PEDAL_OUTPUT_MAX 1000
#define PEDAL_OUTPUT_MIN (-1000)

#define PEDAL_LUT_LENGTH 21
#define PEDAL_LUT_INTERVAL (PEDAL_LUT_LENGTH - 1U)

#define PEDAL_REGEN_END 300U
#define PEDAL_TORQUE_START 350U
#define PEDAL_DEADZONE_WIDTH (PEDAL_TORQUE_START - PEDAL_REGEN_END)

#define PEDAL_LUT_PROFILE_SLOTS 10U
#define PEDAL_LUT_SIZE_BYTES (PEDAL_LUT_LENGTH * sizeof(int16_t))
#define PEDAL_LUT_TOT_SIZE (PEDAL_LUT_SIZE_BYTES * PEDAL_LUT_PROFILE_SLOTS)

#define CAN_CHANGE_PEDAL_PROFILE (0x17)
#define CAN_ADD_PEDAL_PROFILE (0x18)
#define CAN_PEDAL_SAVE_FLASH (0x19)

typedef enum pedal_profile
{
  LINEAR,
  PARABOLIC,
} pedal_profile;

typedef enum
{
  PEDAL_STATUS_OK = 0,

  PEDAL_STATUS_NULL_PTR,
  PEDAL_STATUS_INVALID_SLOT,
  PEDAL_STATUS_INVALID_RAM_PROFILE,
  PEDAL_STATUS_INVALID_FLASH_PROFILE,
  PEDAL_STATUS_INVALID_DATA_CHUNK,

  PEDAL_STATUS_FLASH_ERASE_FAILED,
  PEDAL_STATUS_FLASH_WRITE_FAILED,

} PedalMapStatus;

extern const int16_t pedal_profile_linear[PEDAL_LUT_LENGTH];
extern const int16_t pedal_profile_parabolic[PEDAL_LUT_LENGTH];
extern const int16_t pedal_profile_soft[PEDAL_LUT_LENGTH];
extern const int16_t pedal_profile_stupid[PEDAL_LUT_LENGTH];

extern const int16_t* curr_pedal_profile;
void init_pedal_map(void);
int16_t pedal_map(uint16_t pedal);
int16_t pedal_map_get_percentage();

void change_preset_pedal_profile(pedal_profile profile);
uint8_t add_pedal_profile(void* values, uint8_t slot, uint8_t size);
void process_pedal_profile_add(CAN_Message msg);
void process_pedal_profile_change(CAN_Message msg);
void process_pedal_flash_save(CAN_Message msg);
uint8_t validate_pedal_profile(const int16_t pedal_profile[]);

#endif
