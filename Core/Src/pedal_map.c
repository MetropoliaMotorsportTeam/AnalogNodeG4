#include "pedal_map.h"
#include "sensors.h"
#include "virtual_sensors.h"
#include <string.h>

// TODO: write these to flash
const uint16_t* curr_profile = pedal_profile_aggressive;
static uint16_t pedal_profile_slots[PEDAL_CONFIG_SLOTS][PEDAL_LUT_SIZE];

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

uint16_t pedal_map(uint16_t pedal)
{

  if (pedal >= PEDAL_MAX_VALUE)
  {
    return PEDAL_MAX_VALUE;
  }

  uint16_t index = pedal / PEDAL_LUT_STEP;
  uint16_t remainder = pedal % PEDAL_LUT_STEP;

  uint16_t y0 = curr_profile[index];
  uint16_t y1 = curr_profile[index + 1];

  uint32_t delta = (uint32_t)(y1 - y0);
  uint32_t y = (uint32_t)y0 + ((delta * remainder) / PEDAL_LUT_STEP);

  if (y > PEDAL_MAX_VALUE)
  {
    y = PEDAL_MAX_VALUE;
  }

  return (uint16_t)y;
}

void change_pedal_profile(pedal_profile profile)
{
  switch (profile)
  {
  case LINEAR:
    curr_profile = pedal_profile_linear;
    break;

  case PARABOLIC:
    curr_profile = pedal_profile_aggressive;
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

static uint8_t validate_pedal_values(uint16_t values[], uint8_t size)
{
  // TODO: make when i feel like it
  return 1;
}

void process_pedal_profile_add(CAN_Message msg)
{
  static uint16_t CAN_msg_buf[PEDAL_LUT_SIZE];
  CAN_msg_buf[0] = 0;
  CAN_msg_buf[10] = 1000;
  static uint8_t slot = 0xFF;
  static uint8_t size = 0;

  if (slot == 0xFF)
    slot = msg.Bytes[0];

  if (slot >= PEDAL_CONFIG_SLOTS)
  {
    slot = 0xFF;
    return;
  }

  uint8_t index = msg.Bytes[1];

  // check for valid indexes
  if (index != 1U && index != 4U && index != 7U)
    return;

  memcpy(&CAN_msg_buf[index], &msg.Bytes[2], 6);
  size += 6;

  if (msg.Bytes[0] == PEDAL_TRANSFER_DONE)
  {
    if (validate_pedal_values(CAN_msg_buf, size))
      add_pedal_profile(CAN_msg_buf, slot, size);

    size = 0;
    slot = 0xFF;
    // clear everything except first and last element
    memset(&CAN_msg_buf[1], 0, sizeof(CAN_msg_buf) - sizeof(uint16_t) * 2);
  }

  index = msg.Bytes[0];
}

uint8_t add_pedal_profile(void* values, uint8_t slot, uint8_t size)
{
  if (size < PEDAL_LUT_SIZE)
    return 0;

  if (!values)
    return 0;

  if (slot >= PEDAL_CONFIG_SLOTS)
    return 0;

  memcpy(pedal_profile_slots[slot], values, PEDAL_LUT_SIZE);
  return 1;
}

void process_pedal_profile_change(CAN_Message msg)
{
  uint8_t profile_type = msg.Bytes[0];
  uint8_t profile_index = msg.Bytes[1];

  if (profile_type == 0U)
  {
    change_pedal_profile(profile_index);
  }
  else if (profile_type == 1U)
  {
    if (profile_index < PEDAL_CONFIG_SLOTS)
    {
      curr_profile = pedal_profile_slots[profile_index];
    }
  }
}
