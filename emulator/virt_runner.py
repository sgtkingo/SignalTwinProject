from engine.emulator import VSCPEmulator
import serial.tools.list_ports

def main():
    """Main entry point"""
    
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
    
    sensors = {
            "0": {"temp": 25.5, "alarm": 60.2, "type": "DHT22"},
            "1": {"temp": 25.5, "humi": 80},
            "15": {"intensity": 85, "type": "Light"},
            "2": {"Pressure": 1013.25, "Temperature": 22.1, "type": "BMP280"},
            "3": {"X": 45, "Y": 78, "Button": 0, "type": "Joystick"},
            "5": {"MagField": 12.5, "Detected": 0, "type": "Magnetic"},
            "imu_001": {
                "acm_x": -2.1, "acm_y": 0.8, "acm_z": 9.8,
                "gyr_x": 0.05, "gyr_y": -0.02, "gyr_z": 0.01,
                "type": "IMU"
            },
            "mic_001": {"dBFS": 89.3, "peak": 10.0, "type": "Microphone"},
            "cam_001": {"lux_est": 10.4, "type": "Lux meter"},
            "cpu_temp": {"temp": 55.0, "type": "CPU Temperature"},
        }
    emulator = VSCPEmulator(sensors, port=port, baudrate=115200)
    emulator.run()

if __name__ == "__main__":
    main()