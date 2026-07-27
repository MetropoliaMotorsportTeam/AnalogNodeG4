import can
import time
import struct


CHANNEL = "can0"
BAUD_RATE = 1000000  # need to manually set baudrate when running on linux
ID1 = 0x21
ID2 = 0x24
linear = [
    -1000,
    -833,
    -667,
    -500,
    -333,
    -167,
    0,
    0,
    77,
    154,
    231,
    308,
    385,
    462,
    538,
    615,
    692,
    769,
    846,
    923,
    1000,
]
parabolic = [
    -1000,
    -694,
    -444,
    -250,
    -111,
    -28,
    0,
    0,
    6,
    24,
    53,
    95,
    148,
    213,
    290,
    379,
    479,
    592,
    716,
    852,
    1000,
]

custom = linear

RESP_ACK = 0x23
TIMEOUT_S = 2.0
SLOT = 0


def run_test():
    print(f"Opening CAN channel {CHANNEL}...")

    if len(custom) % 3 != 0:
        print("Curve must contain a multiple of three values.")
        return

    try:
        bus = can.interface.Bus(channel=CHANNEL, interface="socketcan")
    except Exception as e:
        print(f"Failed to open CAN interface: {e}")
        return

    print(f"Sending Command, ID: {hex(ID1)}")

    idx = 0
    while idx < len(custom):
        values = custom[idx : idx + 3]
        data = struct.pack("<BBhhh", SLOT, idx, *values)
        print(f"TX slot={SLOT}, idx={idx}, values={values}, bytes={data.hex(' ')}")
        msg = can.Message(arbitration_id=ID1, data=data, is_extended_id=False)

        try:
            bus.send(msg)
        except can.CanError as e:
            print(f"TX failed: {e}")
            shutdown_bus(bus)
            return

        ret = wait_for_res(bus)
        if ret == 0:
            shutdown_bus(bus)
            return

        idx += 3

    msg = can.Message(arbitration_id=ID2, data=[], is_extended_id=False)
    try:
        bus.send(msg)
    except can.CanError as e:
        print(f"TX failed: {e}")
        shutdown_bus(bus)
        return

    wait_for_res(bus)
    shutdown_bus(bus)


def shutdown_bus(bus):
    print("Shutting down bus.")
    bus.shutdown()


def wait_for_res(bus):
    deadline = time.monotonic() + TIMEOUT_S
    success = False

    while time.monotonic() < deadline:
        rx = bus.recv(timeout=1.0)
        if rx is None:
            continue

        print(f"RX: id=0x{rx.arbitration_id:X}, dlc={rx.dlc}, data={list(rx.data)}")

        if rx.arbitration_id == RESP_ACK:
            print("SUCCESS: Received ACK.")
            success = True
            break

    if not success:
        print("FAILED: No response.")
        return 0
    return 1


if __name__ == "__main__":
    run_test()
