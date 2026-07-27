import can
import time
import struct

CHANNEL = "can0"
BAUD_RATE = 1000000  # need to manually set baudrate when running on linux
ID = 0x20
sensor = 3
calib_select = 1

RESP_ACK = 0x23
TIMEOUT_S = 10.0


def run_test():
    print(f"Opening CAN channel {CHANNEL}...")

    try:
        bus = can.interface.Bus(channel=CHANNEL, interface="socketcan")
    except Exception as e:
        print(f"Failed to open CAN interface: {e}")
        return

    print(f"Sending Command, ID: {hex(ID)}")
    data = [sensor, calib_select]
    msg = can.Message(arbitration_id=ID, data=data, is_extended_id=False)
    try:
        bus.send(msg)
    except can.CanError as e:
        print(f"TX failed: {e}")

    deadline = time.monotonic() + TIMEOUT_S
    while time.monotonic() < deadline:
        rx = bus.recv(timeout=1.0)
        if rx is None:
            continue

        print(f"RX: id=0x{rx.arbitration_id:X}, dlc={rx.dlc}, data={list(rx.data)}")

        if rx.arbitration_id == RESP_ACK:
            print("SUCCESS: Received ACK.")
            break

        print("FAILED: No response.")

    bus.shutdown()


if __name__ == "__main__":
    run_test()
