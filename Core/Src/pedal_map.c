#include "pedal_map.h"
#include "flash_pedal_profiles.h"
#include "sensors.h"
#include "virtual_sensors.h"
#include <string.h>

const uint16_t* curr_profile = pedal_profile_parabolic;
static uint16_t pedal_profile_slots[PEDAL_PROFILE_SLOTS][PEDAL_LUT_SIZE];

// test function
uint16_t pedal_map_get_percentage()
{
#ifdef VIRTUAL_SENSOR
  pedalreq.update_func(&pedalreq);
  return pedal_map(pedalreq.output.values[0]);
#else
  return pedal_map(APPS_pedal->transfer_function(1, APPS_pedal->averages, APPS_pedal));
#endif
  return 0;
}

static inline uint16_t pedal_profile_get_point(const uint16_t* profile, uint8_t point)
{
  if (point == 0U)
    return 0U;
  if (point >= (PEDAL_LUT_SIZE + 1U))
    return PEDAL_MAX_VALUE;
  return profile[point - 1U];
}

uint16_t pedal_map(uint16_t pedal)
{

  if (pedal >= PEDAL_MAX_VALUE)
  {
    return PEDAL_MAX_VALUE;
  }

  uint16_t index = pedal / PEDAL_LUT_STEP;
  uint16_t remainder = pedal % PEDAL_LUT_STEP;

  uint16_t y0 = pedal_profile_get_point(curr_profile, index);
  uint16_t y1 = pedal_profile_get_point(curr_profile, index + 1);

  uint32_t delta = (uint32_t)y1 - (uint32_t)y0;
  uint32_t y = (uint32_t)y0 + ((delta * remainder) / PEDAL_LUT_STEP);

  if (y > PEDAL_MAX_VALUE)
  {
    y = PEDAL_MAX_VALUE;
  }

  return (uint16_t)y;
}

void change_preset_pedal_profile(pedal_profile profile)
{
  switch (profile)
  {
  case LINEAR:
    curr_profile = pedal_profile_linear;
    break;

  case PARABOLIC:
    curr_profile = pedal_profile_parabolic;
    break;

  case SOFT:
    curr_profile = pedal_profile_soft;
    break;

  case STUPID:
    curr_profile = pedal_profile_stupid;
    break;

  default:
    break;
  }
}

static uint8_t validate_pedal_values(const uint16_t values[3])
{
  if (values[0] > PEDAL_MAX_VALUE || values[1] > PEDAL_MAX_VALUE || values[2] > PEDAL_MAX_VALUE)
  {
    return 0;
  }

  if (values[0] > values[1] || values[1] > values[2])
  {
    return 0;
  }

  return 1;
}

void process_pedal_profile_add(CAN_Message msg)
{

  uint8_t slot = msg.Bytes[0];
  uint8_t index = msg.Bytes[1];
  uint16_t values[3];

  // check for valid indexes
  if (index != 0U && index != 3U && index != 6U)
    return;

  if (slot >= PEDAL_PROFILE_SLOTS)
    return;

  memcpy(values, &msg.Bytes[2], sizeof(values));

  if (!validate_pedal_values(values))
  {
    // TODO: send CAN error
    return;
  }

  memcpy(&pedal_profile_slots[slot][index], values, sizeof(values));
}

// for testing
uint8_t add_pedal_profile(void* values, uint8_t slot, uint8_t size)
{
  if (size != PEDAL_PROFILE_SIZE)
    return 0;

  if (!values)
    return 0;

  if (slot >= PEDAL_PROFILE_SLOTS)
    return 0;

  memcpy(pedal_profile_slots[slot], values, PEDAL_PROFILE_SIZE);
  return 1;
}

static void process_pedal_profile_status(PedalProfileStatus status)
{
  switch (status)
  {
  case PEDAL_PROFILE_STATUS_OK:
    break;

    // TODO: send error codes via CAN
  case PEDAL_PROFILE_STATUS_NULL_PTR:
    break;
  case PEDAL_PROFILE_STATUS_INVALID_SLOT:
    break;
  case PEDAL_PROFILE_STATUS_INVALID_RAM_PROFILE:
    break;
  case PEDAL_PROFILE_STATUS_INVALID_FLASH_PROFILE:
    break;
  case PEDAL_PROFILE_STATUS_FLASH_ERASE_FAILED:
    break;
  case PEDAL_PROFILE_STATUS_FLASH_WRITE_FAILED:
    break;
  default:
    break;
  }
}

void process_pedal_profile_change(CAN_Message msg)
{
  uint8_t type = msg.Bytes[0];
  uint8_t slot = msg.Bytes[1];

  if (type == 0U)
  {
    change_preset_pedal_profile(slot);
  }
  else if (type == 1U)
  {
    if (slot < PEDAL_PROFILE_SLOTS)
    {
      uint16_t* profile = pedal_profile_slots[slot];
      if (!validate_pedal_profile(profile))
      {
        PedalProfileStatus status = restore_pedal_profile_from_flash(slot, profile);
        process_pedal_profile_status(status);
        // TODO: uh yeah idk
      }
      curr_profile = profile;
    }
  }
}

void process_pedal_flash_save(CAN_Message msg)
{
  PedalProfileStatus status = save_all_pedal_profiles(pedal_profile_slots);
  process_pedal_profile_status(status);
}

uint8_t validate_pedal_profile(const uint16_t pedal_profile[PEDAL_LUT_SIZE])
{
  for (uint8_t i = 1; i < PEDAL_LUT_SIZE; i++)
  {
    if (pedal_profile[i] < pedal_profile[i - 1])
      return 0;
  }
  return 1;
}

void init_pedal_map(void)
{
  memcpy(pedal_profile_slots, get_saved_profile(0), PEDAL_PROFILES_TOT_SIZE);
}
