import serial.tools.list_ports
from engine.emulator_patterns import VSCPEmulator

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
    emulator = VSCPEmulator(port=port, baudrate=115200)
    emulator.run()

if __name__ == "__main__":
    main()