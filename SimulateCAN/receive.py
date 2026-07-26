import can

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

        print(f"RX: id=0x{rx.arbitration_id:X}, dlc={rx.dlc}, data={list(rx.data)}")

except KeyboardInterrupt:
    print("\nStopping CAN receiver.")

finally:
    bus.shutdown()
