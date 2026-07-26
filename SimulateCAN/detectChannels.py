import can
import subprocess


def list_can_interfaces():
    print("--- Linux CAN Interface Detector ---")

    # 1. Check via 'ip link' (The most reliable way on Linux)
    print("\n[System Level - ip link]")
    try:
        result = subprocess.run(["ip", "link", "show"], capture_output=True, text=True)
        lines = result.stdout.split("\n")
        found_any = False
        for line in lines:
            if "can" in line:
                print(f"Detected: {line.strip()}")
                found_any = True
        if not found_any:
            print("No CAN interfaces found in 'ip link'. Is the driver loaded?")
    except FileNotFoundError:
        print("Error: 'ip' command not found. Install 'iproute2'.")

    # 2. Check via python-can library
    print("\n[Library Level - python-can]")
    try:
        # On Linux, python-can can list interfaces by checking /sys/class/net
        from can.interfaces.socketcan.constants import CAN_RAW
        import os

        interfaces = os.listdir("/sys/class/net")
        can_interfaces = [
            i for i in interfaces if i.startswith("can") or i.startswith("vcan")
        ]

        if can_interfaces:
            for iface in can_interfaces:
                print(f"Interface Name: {iface}")
                # Try to check if it's UP or DOWN
                with open(f"/sys/class/net/{iface}/operstate", "r") as f:
                    state = f.read().strip()
                    print(f"  Status: {state}")
        else:
            print("No 'can' prefixed interfaces found in /sys/class/net.")

    except Exception as e:
        print(f"Error accessing system net classes: {e}")


if __name__ == "__main__":
    list_can_interfaces()
