#!/usr/bin/env python3
import os
import time
import threading
from engine.emulator import VSCPEmulator
from engine.sensors import load_sensors

UPDATE_PERIOD_S = float(os.getenv("VSCP_UPDATE_PERIOD", "0.5"))

class RealSensorBridge:
    """Periodically pulls real data from sensor adapters and updates the emulator."""
    def __init__(self, emu: VSCPEmulator, real_sensors):
        self.emu = emu
        self.sensors = real_sensors
        self.running = False
        # Pre-register keys so UPDATE works before first sample arrives
        for s in self.sensors:
            self.emu.sensor_data.setdefault(s.uid, {"type": s.kind})
            
        

    def loop(self):
        while self.running:
            for s in self.sensors:
                try:
                    data = s.read()
                    if isinstance(data, dict) and data:
                        payload = {**data, "type": s.kind}
                        self.emu.sensor_data[s.uid] = payload
                    else:
                        #print(f"[Sensor {s.uid}] no data found, using simulated values...")
                        pass
                except Exception as e:
                    print(f"[Sensor {s.uid}] error: {e}")
            time.sleep(UPDATE_PERIOD_S)

    def start(self):
        self.running = True
        self.t = threading.Thread(target=self.loop, daemon=True)
        self.t.start()

    def stop(self):
        self.running = False
        if hasattr(self, "t"):
            self.t.join(timeout=1.0)

if __name__ == "__main__":
    import serial.tools.list_ports
    
    # Get available COM ports
    available_ports = [port.device for port in serial.tools.list_ports.comports()]
    
    if available_ports:
        default_port = available_ports[0]
        print(f"Available COM ports: {', '.join(available_ports)}")
    else:
        default_port = 'COM8'
        print("No COM ports detected, using default COM8")
    
    port = input(f"Enter serial port (default: {default_port}): ").strip()
    if not port:
        port = default_port
        
    # Load virtual sensors
    virtual_sensors = {
            "mic_001": {"dBFS": 89.3, "peak": 10.0, "type": "Microphone"},
            "cam_001": {"lux_est": 10.4, "type": "Lux meter"},
            "cpu_temp": {"temp": 55.0, "type": "CPU Temperature"},
    }
    if not virtual_sensors:
        print("No virtual sensors found. Exiting.")
        raise SystemExit(1)
    emu = VSCPEmulator(virtual_sensors, port=port, baudrate=115200)

    if not emu.connect_serial():
        raise SystemExit(1)
    emu.running = True

    threading.Thread(target=emu.listen_loop, daemon=True).start()

    # Initialize protocol so UPDATE works immediately
    #print(emu.process_request("?type=INIT&app=RealRunner&db=1.0.0&api=1.2"))

    # Start real sensor bridge
    real_sensors = load_sensors()
    bridge = RealSensorBridge(emu, real_sensors)
    bridge.start()

    print("Real sensors active:", [f"{s.uid}({s.kind})" for s in real_sensors])
    print("Send UPDATE, e.g.: ?type=UPDATE&id=mic_001  |  ?type=UPDATE&id=cam_001  |  ?type=UPDATE&id=cpu_temp")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        bridge.stop()
        emu.running = False
        emu.disconnect_serial()
