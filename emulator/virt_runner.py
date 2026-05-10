from engine.emulator import VSCPEmulator, available_serial_ports, load_catalog_defaults

def main():
    """Main entry point"""
    
    # Get available COM ports
    available_ports = available_serial_ports()
    
    if available_ports:
        default_port = available_ports[0]
        print(f"Available COM ports: {', '.join(available_ports)}")
    else:
        default_port = 'COM8'
        print("No COM ports detected, using default COM8")
    
    port = input(f"Enter serial port (default: {default_port}): ").strip()
    if not port:
        port = default_port
    
    sensors, metadata = load_catalog_defaults()
    print(f"Using catalog {metadata.get('path', 'fallback defaults')}")
    print(f"Catalog app={metadata.get('application')} db={metadata.get('version')}")
    emulator = VSCPEmulator(sensors, port=port, baudrate=115200)
    emulator.run()

if __name__ == "__main__":
    main()
