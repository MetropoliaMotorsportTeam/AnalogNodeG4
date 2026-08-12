#include "pedal_map.h"
#include "flash_pedal_lut.h"
#include "sensors.h"
#include "virtual_sensors.h"
#include <string.h>

static int16_t pedal_profile_slots[PEDAL_LUT_PROFILE_SLOTS][PEDAL_LUT_LENGTH];
const int16_t* curr_pedal_profile = pedal_profile_linear;
const int16_t pedal_profile_linear[PEDAL_LUT_LENGTH] = {-1000, -833, -667, -500, -333, -167, 0,
                                                        0,     77,   154,  231,  308,  385,  462,
                                                        538,   615,  692,  769,  846,  923,  1000};
const int16_t pedal_profile_parabolic[PEDAL_LUT_LENGTH] = {
    -1000, -694, -444, -250, -111, -28, 0,   0,   6,   24,  53,
    95,    148,  213,  290,  379,  479, 592, 716, 852, 1000};

static void process_pedal_status(PedalMapStatus status)
{
  CAN_Message msg = {.Id = CAN_RETURN_MSG_ID, .DLC = 2, .Bytes = {0}};
  switch (status)
  {
  case PEDAL_STATUS_OK:
    msg.Bytes[0] = 200;
    break;

  case PEDAL_STATUS_NULL_PTR:
    msg.Bytes[0] = 1;
    break;

  case PEDAL_STATUS_INVALID_SLOT:
    msg.Bytes[0] = 2;
    break;

  case PEDAL_STATUS_INVALID_INDEX:
    msg.Bytes[0] = 3;
    break;

  case PEDAL_STATUS_INVALID_RAM_PROFILE:
    msg.Bytes[0] = 4;
    break;

  case PEDAL_STATUS_INVALID_FLASH_PROFILE:
    msg.Bytes[0] = 5;
    break;

  case PEDAL_STATUS_FLASH_ERASE_FAILED:
    msg.Bytes[0] = 6;
    break;

  case PEDAL_STATUS_FLASH_WRITE_FAILED:
    msg.Bytes[0] = 7;
    break;

  case PEDAL_STATUS_INVALID_DATA_CHUNK:
    msg.Bytes[0] = 8;
    break;

  default:
    break;
  }
  CanSendMsg(msg);
}

int16_t pedal_map(uint16_t pedal)
{
  if (!curr_pedal_profile)
  {
    return 0;
  }

  if (pedal >= PEDAL_INPUT_MAX)
  {
    return PEDAL_OUTPUT_MAX;
  }

  if (pedal >= PEDAL_REGEN_END && pedal <= PEDAL_TORQUE_START)
  {
    // manually checking if pedal is in deadzone range
    return 0;
  }

  uint16_t pedal_step = PEDAL_INPUT_MAX / PEDAL_LUT_INTERVAL;

  int16_t index = pedal / pedal_step;
  int16_t remainder = pedal % pedal_step;

  int16_t y0 = curr_pedal_profile[index];
  int16_t y1 = curr_pedal_profile[index + 1];

  int32_t delta = (int32_t)y1 - (int32_t)y0;
  int32_t y = (int32_t)y0 + ((delta * remainder) / pedal_step);

  if (y > PEDAL_OUTPUT_MAX)
  {
    y = PEDAL_OUTPUT_MAX;
  }

  if (y < PEDAL_OUTPUT_MIN)
  {
    y = PEDAL_OUTPUT_MIN;
  }

  return (int16_t)y;
}

void change_preset_pedal_profile(pedal_profile profile)
{
  switch (profile)
  {
  case LINEAR:
    curr_pedal_profile = pedal_profile_linear;
    break;

  case PARABOLIC:
    curr_pedal_profile = pedal_profile_parabolic;
    break;

  default:
    break;
  }
}

uint8_t validate_pedal_profile(const int16_t pedal_profile[])
{
  if (!pedal_profile)
    return 0;

  if ((pedal_profile[6] | pedal_profile[7]) != 0)
    return 0;

  for (uint8_t i = 1; i < PEDAL_LUT_LENGTH; i++)
  {
    if (pedal_profile[i] < pedal_profile[i - 1])
      return 0;
  }
  return 1;
}

static inline uint8_t validate_pedal_values(const int16_t values[3])
{
  if (values[0] > PEDAL_OUTPUT_MAX || values[1] > PEDAL_OUTPUT_MAX || values[2] > PEDAL_OUTPUT_MAX)
    return 0;
  if (values[0] < PEDAL_OUTPUT_MIN || values[1] < PEDAL_OUTPUT_MIN || values[2] < PEDAL_OUTPUT_MIN)
    return 0;
  if (values[0] > values[1] || values[1] > values[2])
    return 0;
  return 1;
}

void process_pedal_profile_add(CAN_Message msg)
{
  uint8_t slot = msg.Bytes[0];
  uint8_t index = msg.Bytes[1];
  int16_t values[3];

  if ((index % 3U) != 0U)
  {
    process_pedal_status(PEDAL_STATUS_INVALID_INDEX);
    return;
  }

  if ((index + 3U) > PEDAL_LUT_LENGTH)
  {
    process_pedal_status(PEDAL_STATUS_INVALID_INDEX);
    return;
  }

  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    return;

  memcpy(values, &msg.Bytes[2], sizeof(values));

  if (!validate_pedal_values(values))
  {
    process_pedal_status(PEDAL_STATUS_INVALID_DATA_CHUNK);
    return;
  }
  process_pedal_status(PEDAL_STATUS_OK);
  memcpy(&pedal_profile_slots[slot][index], values, sizeof(values));
}

// for testing
uint8_t add_pedal_profile(void* values, uint8_t slot, uint8_t size)
{
  if (size > PEDAL_LUT_SIZE_BYTES)
    return 0;

  if (!values)
    return 0;

  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    return 0;

  memcpy(pedal_profile_slots[slot], values, size);
  return 1;
}

void process_pedal_profile_change(CAN_Message msg)
{
  uint8_t type = msg.Bytes[0];
  uint8_t slot = msg.Bytes[1];

  if (type == 0)
  {
    change_preset_pedal_profile(slot);
  }
  else if (type == 1)
  {
    if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    {
      process_pedal_status(PEDAL_STATUS_INVALID_SLOT);
      return;
    }

    int16_t* profile = pedal_profile_slots[slot];

    if (!validate_pedal_profile(profile))
    {
      process_pedal_status(PEDAL_STATUS_INVALID_RAM_PROFILE);
      return;
    }
    curr_pedal_profile = profile;
  }
  process_pedal_status(PEDAL_STATUS_OK);
}

void process_pedal_flash_save(CAN_Message msg)
{
  uint8_t stype = msg.Bytes[0];
  uint8_t slot = msg.Bytes[1];

  if (stype == 0)
  {
    PedalMapStatus status = save_all_pedal_profiles(pedal_profile_slots);
    process_pedal_status(status);
    return;
  }
  else if (stype == 1)
  {
    if (slot >= PEDAL_LUT_PROFILE_SLOTS)
    {
      process_pedal_status(PEDAL_STATUS_INVALID_SLOT);
      return;
    }
    PedalMapStatus status = save_current_pedal_profile(slot);
    process_pedal_status(status);
  }
}

int16_t* get_ram_pedal_profile(uint8_t profile)
{
  if (profile >= PEDAL_LUT_PROFILE_SLOTS)
    return NULL;
  return pedal_profile_slots[profile];
}

void init_pedal_map(void)
{
  const int16_t* saved_profiles = get_all_saved_pedal_profiles();

  if (saved_profiles != NULL)
  {
    memcpy(pedal_profile_slots, saved_profiles, PEDAL_LUT_TOT_SIZE);
  }

  for (uint8_t slot = 0; slot < PEDAL_LUT_PROFILE_SLOTS; slot++)
  {
    if (!validate_pedal_profile(pedal_profile_slots[slot]))
    {
      memcpy(pedal_profile_slots[slot], pedal_profile_linear, PEDAL_LUT_SIZE_BYTES);
    }
  }
  uint8_t slot = get_current_pedal_profile();
  if (slot >= PEDAL_LUT_PROFILE_SLOTS)
  {
    return;
  }

  curr_pedal_profile = pedal_profile_slots[slot];
}
