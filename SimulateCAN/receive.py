import can
import struct

CHANNEL = "can0"
try:
    bus = can.interface.Bus(channel=CHANNEL, interface="socketcan")
except Exception as e:
    print(f"Failed to open CAN interface: {e}")
    raise SystemExit(1)

try:
    print("Receiving.")
    while True:
        rx = bus.recv(timeout=1.0)

        if rx is None:
            continue

        if rx.dlc > 1:
            val = struct.unpack("<h", bytes(rx.data)[:2])[0]
            print(f"RX: id=0x{rx.arbitration_id:X}, dlc={rx.dlc}, data={val}")

except KeyboardInterrupt:
    print("\nStopping CAN receiver.")

finally:
    bus.shutdown()
