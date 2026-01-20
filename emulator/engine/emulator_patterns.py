#!/usr/bin/env python3
"""
Virtual Sensors Protocol (VSCP) Emulator
========================================

Reactive client emulator that implements the complete VSCP communication protocol
as specified in vscp.hpp. Handles all protocol methods with dummy responses.

Protocol Methods:
- INIT: Handshake and version compatibility check
- UPDATE: Sensor data update requests
- CONFIG: Configuration changes from HMI to HW
- RESET: Sensor reset operations
- CONNECT: Connect sensor to specific pin
- DISCONNECT: Disconnect sensor from pin

Protocol Format: URL-like with key-value pairs
Request: ?type=METHOD&param1=value1&param2=value2
Response: ?status=1/0&param1=value1&error=message

Author: Generated for VSCP Protocol Testing
"""

import serial
import time
import random
import threading
import re
import math
from urllib.parse import parse_qs, unquote
from typing import Dict, Any, Optional

class VSCPEmulator:
    """Virtual Sensors Communication Protocol Emulator"""
    
    def __init__(self, port='COM3', baudrate=115200, timeout=0.1):
        """Initialize the VSCP emulator"""
        self.API_VERSION = "1.2"
        self.DB_VERSION = "dummy"
        self.APP_NAME = "VSCP Emulator"
        self.APP_VERSION = "1.0.0"
        
        # Protocol state
        self.initialized = False
        self.connected_sensors = {}  # uid -> pin mapping
        self.sensor_configs = {}     # uid -> config dict
        
        # Serial connection
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        self.ser = None
        self.running = False
        
        # Simulation state tracking
        self.start_time = time.time()
        self.sensor_states = {}  # Track sensor simulation state
        self.simulation_mode = "normal"  # Current simulation scenario
        
        # Enhanced sensor data with base values and simulation parameters
        self.sensor_data = {
            "S00": {
                "temp": {"base": 25.5, "range": 10, "trend": 0.1, "noise": 0.5},
                "alarm": {"base": 60.2, "range": 20, "trend": 0, "noise": 2},
                "type": "DHT22"
            },
            "S01": {
                "temp": {"base": 25.5, "range": 8, "trend": 0.05, "noise": 0.3},
                "humi": {"base": 80, "range": 30, "trend": -0.2, "noise": 2},
                "type": "DHT22"
            },
            "S15": {
                "intensity": {"base": 85, "range": 70, "trend": 0, "noise": 5},
                "type": "Light"
            },
            "S02": {
                "Pressure": {"base": 1013.25, "range": 50, "trend": 0.01, "noise": 1.5},
                "Temperature": {"base": 22.1, "range": 12, "trend": 0.08, "noise": 0.4},
                "type": "BMP280"
            },
            "S03": {
                "X": {"base": 45, "range": 90, "trend": 0, "noise": 3},
                "Y": {"base": 78, "range": 90, "trend": 0, "noise": 3},
                "Button": {"base": 0, "range": 1, "trend": 0, "noise": 0},
                "type": "Joystick"
            },
            "S05": {
                "MagField": {"base": 12.5, "range": 20, "trend": 0, "noise": 0.8},
                "Detected": {"base": 0, "range": 1, "trend": 0, "noise": 0},
                "type": "Magnetic"
            },
            "imu_001": {
                "acm_x": {"base": -2.1, "range": 4, "trend": 0, "noise": 0.3},
                "acm_y": {"base": 0.8, "range": 4, "trend": 0, "noise": 0.3},
                "acm_z": {"base": 9.8, "range": 2, "trend": 0, "noise": 0.1},
                "gyr_x": {"base": 0.05, "range": 0.2, "trend": 0, "noise": 0.02},
                "gyr_y": {"base": -0.02, "range": 0.2, "trend": 0, "noise": 0.02},
                "gyr_z": {"base": 0.01, "range": 0.2, "trend": 0, "noise": 0.02},
                "type": "IMU"
            },
            "mic_001": {
                "dBFS": {"base": 89.5, "range": 40, "trend": 0, "noise": 3},
                "peak": {"base": 12.0, "range": 20, "trend": 0, "noise": 1.5},
                "type": "SLM"
            },
            "cam_001": {
                "lux_est": {"base": 11.5, "range": 100, "trend": 0, "noise": 2},
                "type": "CAM"
            },
            "cpu_temp": {
                "temp": {"base": 55.3, "range": 25, "trend": 0.2, "noise": 1.2},
                "type": "CPU Temp" 
            }
        }
        
        # Initialize sensor states
        for uid in self.sensor_data:
            self.sensor_states[uid] = {
                "last_update": 0,
                "phase_offset": random.uniform(0, 2 * math.pi),
                "peak_timer": 0,
                "dropout_timer": 0,
                "trend_accumulator": 0,
                "spike_countdown": random.randint(50, 200)
            }
        
    def connect_serial(self) -> bool:
        """Connect to serial port"""
        try:
            self.ser = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                timeout=self.timeout
            )
            if not self.ser.is_open:
                self.ser.open()
            print(f"✓ Connected to {self.port} at {self.baudrate} baud")
            return True
        except Exception as e:
            print(f"✗ Failed to connect to {self.port}: {e}")
            return False
    
    def disconnect_serial(self):
        """Disconnect from serial port"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            print("✓ Serial connection closed")
    
    def parse_message(self, message: str) -> Dict[str, str]:
        """Parse protocol message into key-value pairs"""
        params = {}
        
        # Remove leading '?' if present
        clean_message = message.strip()
        if clean_message.startswith('?'):
            clean_message = clean_message[1:]
        
        # Split by '&' and parse key=value pairs
        if clean_message:
            pairs = clean_message.split('&')
            for pair in pairs:
                if '=' in pair:
                    key, value = pair.split('=', 1)
                    params[key.strip()] = unquote(value.strip())
        
        return params
    
    def build_message(self, params: Dict[str, Any]) -> str:
        """Build protocol message from key-value pairs"""
        if not params:
            return "?status=0&error=No parameters"
        
        parts = []
        for key, value in params.items():
            parts.append(f"{key}={value}")
        
        return "?" + "&".join(parts)
    
    def handle_init(self, params: Dict[str, str]) -> str:
        """Handle INIT method - handshake and version check"""
        print(f"🔄 INIT request: {params}")
        
        # Dummy response for testing
        response_params = {'status': '1'}
        self.initialized = True
        return self.build_message(response_params)
        
        # Extract parameters
        app = params.get('app', 'Unknown')
        dbversion = params.get('db', 'Unknown')
        api = params.get('api', '0.0.0')
        
        # Simulate version compatibility check
        response_params = {}
        
        if api == self.API_VERSION:
            self.initialized = True
            response_params = {
                'status': '1',
                'message': f'Initialized with {app}'
            }
            print(f"✓ Initialization successful for {app}")
        else:
            response_params = {
                'status': '0',
                'error': f'Version mismatch - API:{api} (need {self.API_VERSION}), DB:{dbversion} (need {self.DB_VERSION})'
            }
            print(f"✗ Version mismatch: API {api}, DB {dbversion}")
        
        return self.build_message(response_params)
    
    def simulate_realistic_value(self, uid: str, param_name: str, config: Dict[str, float]) -> float:
        """Generate realistic sensor values with peaks, dropdowns, trends, and noise"""
        current_time = time.time()
        elapsed = current_time - self.start_time
        state = self.sensor_states[uid]
        
        base = config["base"]
        range_val = config["range"]
        trend = config["trend"]
        noise_level = config["noise"]
        
        # Apply scenario-based modifications
        scenario_multiplier = 1.0
        scenario_offset = 0.0
        
        if self.simulation_mode == "high_activity":
            scenario_multiplier = 1.5
            noise_level *= 2.0
        elif self.simulation_mode == "thermal_event" and param_name in ["temp", "Temperature"]:
            scenario_offset = range_val * 0.3
            scenario_multiplier = 1.3
        elif self.simulation_mode == "environmental" and param_name in ["Pressure", "intensity", "lux_est"]:
            scenario_multiplier = 0.7
            noise_level *= 1.5
        elif self.simulation_mode == "vibration" and uid in ["imu_001", "S05"]:
            scenario_multiplier = 2.0
            noise_level *= 3.0
        elif self.simulation_mode == "recovery":
            scenario_multiplier = 0.8
        
        # Time-based components
        slow_cycle = math.sin(elapsed * 0.05 + state["phase_offset"]) * 0.3  # Slow oscillation
        fast_cycle = math.sin(elapsed * 0.5 + state["phase_offset"]) * 0.1   # Fast oscillation
        
        # Trend accumulation (drift over time)
        state["trend_accumulator"] += trend * 0.01
        if abs(state["trend_accumulator"]) > range_val * 0.5:
            state["trend_accumulator"] *= 0.8  # Prevent runaway trends
        
        # Peak generation (sudden spikes)
        peak_factor = 0
        state["spike_countdown"] -= 1
        if state["spike_countdown"] <= 0:
            state["peak_timer"] = random.randint(5, 15)  # Peak duration
            state["spike_countdown"] = random.randint(100, 300)  # Next spike timing
        
        if state["peak_timer"] > 0:
            peak_intensity = math.exp(-((15 - state["peak_timer"]) ** 2) / 10)  # Gaussian peak
            peak_factor = random.uniform(0.5, 1.5) * peak_intensity * scenario_multiplier
            state["peak_timer"] -= 1
        
        # Dropout simulation (sudden drops)
        dropout_factor = 0
        if random.random() < (0.005 * scenario_multiplier):  # Scenario affects dropout chance
            state["dropout_timer"] = random.randint(3, 10)
        
        if state["dropout_timer"] > 0:
            dropout_factor = -random.uniform(0.3, 0.8) * scenario_multiplier
            state["dropout_timer"] -= 1
        
        # Environmental noise patterns
        if param_name in ["temp", "Temperature"]:
            # Temperature has daily cycle + weather patterns
            daily_cycle = math.sin(elapsed * 0.1) * 0.4  # Simulate daily temperature variation
            weather_noise = math.sin(elapsed * 0.02) * 0.3  # Weather pattern
            environmental = daily_cycle + weather_noise
        elif param_name in ["humi"]:
            # Humidity inversely related to temperature
            environmental = -math.sin(elapsed * 0.1) * 0.3
        elif param_name in ["Pressure"]:
            # Barometric pressure has weather patterns
            environmental = math.sin(elapsed * 0.01) * 0.5 + math.sin(elapsed * 0.03) * 0.2
        elif param_name in ["intensity", "lux_est"]:
            # Light follows day/night cycle with cloud variations
            day_night = math.sin(elapsed * 0.15) * 0.8
            clouds = math.sin(elapsed * 0.3) * 0.3 * random.uniform(0.5, 1.0)
            environmental = max(0.1, day_night + clouds)  # Don't go completely dark
        elif param_name in ["dBFS", "peak"]:
            # Audio has bursts and quiet periods
            burst_factor = scenario_multiplier
            if random.random() < 0.02:  # 2% chance of audio burst
                burst_factor *= random.uniform(1.5, 3.0)
            environmental = math.sin(elapsed * 0.2) * 0.2 * burst_factor
        else:
            environmental = 0
        
        # Random noise (always present)
        noise = random.gauss(0, noise_level * 0.1)
        
        # Occasional large spikes/glitches (affected by scenario)
        if random.random() < (0.001 * scenario_multiplier):  # 0.1% base chance
            noise += random.uniform(-noise_level * 2, noise_level * 2) * scenario_multiplier
        
        # Combine all factors
        total_variation = (
            slow_cycle * range_val * 0.3 * scenario_multiplier +
            fast_cycle * range_val * 0.1 * scenario_multiplier +
            state["trend_accumulator"] +
            peak_factor * range_val * 0.4 +
            dropout_factor * range_val * 0.6 +
            environmental * range_val * 0.3 * scenario_multiplier +
            noise +
            scenario_offset
        )
        
        # Calculate final value with bounds checking
        final_value = base + total_variation
        
        # Apply realistic bounds for specific sensor types
        if param_name in ["humi"]:
            final_value = max(0, min(100, final_value))  # Humidity 0-100%
        elif param_name in ["Button", "Detected"]:
            # Binary sensors more active during high activity scenarios
            active_chance = 0.1 if self.simulation_mode != "high_activity" else 0.3
            final_value = 1 if random.random() < active_chance else 0
        elif param_name in ["intensity", "lux_est"] and final_value < 0:
            final_value = abs(final_value) * 0.1  # Light can't be negative
        elif param_name in ["Pressure"]:
            final_value = max(900, min(1100, final_value))  # Reasonable pressure range
        
        return final_value

    def handle_update(self, params: Dict[str, str]) -> str:
        """Handle UPDATE method - return realistic sensor data"""
        uid = params.get('id', '')
        print(f"📊 UPDATE request for sensor: {uid}")
        
        if not self.initialized:
            return self.build_message({'status': '0', 'error': 'Protocol not initialized'})
        
        if uid in self.sensor_data:
            # Get sensor configuration
            sensor_config = self.sensor_data[uid]
            sensor_info = {}
            
            # Generate realistic values for each parameter
            for key, config in sensor_config.items():
                if key == 'type':
                    sensor_info[key] = config  # Type is static
                elif isinstance(config, dict) and 'base' in config:
                    # Generate realistic value using simulation
                    value = self.simulate_realistic_value(uid, key, config)
                    
                    # Format appropriately (int vs float)
                    if key in ["Button", "Detected", "X", "Y"]:
                        sensor_info[key] = int(round(value))
                    else:
                        sensor_info[key] = round(value, 2)
                else:
                    # Fallback for any static values
                    sensor_info[key] = config
            
            response_params = {'id': uid, 'status': '1'}
            response_params.update(sensor_info)
            
            print(f"✓ Sensor {uid} data: {sensor_info}")
        else:
            response_params = {
                'id': uid,
                'status': '0',
                'error': f'Sensor {uid} not found'
            }
            print(f"✗ Sensor {uid} not found")
        
        return self.build_message(response_params)
    
    def adjust_simulation_scenario(self):
        """Dynamically adjust simulation scenarios to create interesting patterns"""
        current_time = time.time()
        elapsed = current_time - self.start_time
        
        # Every 60 seconds, introduce scenario changes
        scenario_phase = int(elapsed / 60) % 6
        
        if scenario_phase == 0:
            # Normal operation
            self.simulation_mode = "normal"
        elif scenario_phase == 1:
            # High activity period (all sensors more active)
            self.simulation_mode = "high_activity"
            for uid in self.sensor_states:
                self.sensor_states[uid]["spike_countdown"] = min(
                    self.sensor_states[uid]["spike_countdown"], 20
                )
        elif scenario_phase == 2:
            # Thermal event (temperature sensors spike)
            self.simulation_mode = "thermal_event"
            for uid in ["S00", "S01", "S02", "cpu_temp"]:
                if uid in self.sensor_states:
                    self.sensor_states[uid]["peak_timer"] = 30
        elif scenario_phase == 3:
            # Environmental disturbance (pressure and light affected)
            self.simulation_mode = "environmental"
            for uid in ["S02", "S15", "cam_001"]:
                if uid in self.sensor_states:
                    self.sensor_states[uid]["dropout_timer"] = 15
        elif scenario_phase == 4:
            # Mechanical vibration (IMU and magnetic sensors affected)
            self.simulation_mode = "vibration"
            for uid in ["imu_001", "S05"]:
                if uid in self.sensor_states:
                    self.sensor_states[uid]["spike_countdown"] = 5
        else:
            # Recovery period (gradual return to normal)
            self.simulation_mode = "recovery"
    
    def handle_config(self, params: Dict[str, str]) -> str:
        """Handle CONFIG method - configure sensor"""
        uid = params.get('id', '')
        print(f"⚙️  CONFIG request for sensor: {uid}")
        
        if not self.initialized:
            return self.build_message({'status': '0', 'error': 'Protocol not initialized'})
        
        # Extract configuration parameters (exclude 'type' and 'id')
        config_params = {k: v for k, v in params.items() if k not in ['type', 'id']}
        
        if uid:
            # Store configuration
            self.sensor_configs[uid] = config_params
            response_params = {
                'id': uid,
                'status': '1',
                'message': f'Configuration applied: {config_params}'
            }
            print(f"✓ Sensor {uid} configured: {config_params}")
        else:
            response_params = {
                'id': uid,
                'status': '0',
                'error': 'Invalid sensor ID'
            }
            print(f"✗ Invalid sensor ID: {uid}")
        
        return self.build_message(response_params)
    
    def handle_reset(self, params: Dict[str, str]) -> str:
        """Handle RESET method - reset sensor"""
        uid = params.get('id', '')
        print(f"🔄 RESET request for sensor: {uid}")
        
        if not self.initialized:
            return self.build_message({'status': '0', 'error': 'Protocol not initialized'})
        
        if uid in self.sensor_data or uid == 'all':
            # Reset sensor(s)
            if uid == 'all':
                self.sensor_configs.clear()
                self.connected_sensors.clear()
                print("✓ All sensors reset")
            else:
                self.sensor_configs.pop(uid, None)
                self.connected_sensors.pop(uid, None)
                print(f"✓ Sensor {uid} reset")
            
            response_params = {
                'id': uid,
                'status': '1'
            }
        else:
            response_params = {
                'id': uid,
                'status': '0',
                'error': f'Sensor {uid} not found'
            }
            print(f"✗ Sensor {uid} not found for reset")
        
        return self.build_message(response_params)
    
    def handle_connect(self, params: Dict[str, str]) -> str:
        """Handle CONNECT method - connect sensor to pin"""
        uid = params.get('id', '')
        pins = params.get('pins', '')
        print(f"🔌 CONNECT request: sensor {uid} to pins {pins}")
        
        if not self.initialized:
            return self.build_message({'status': '0', 'error': 'Protocol not initialized'})
        
        if uid and pins:
            try:
                pins_num = [int(pin) for pin in pins.split(',')]
                # Check if pin is already used
                used_by = None
                for sensor_id, used_pin in self.connected_sensors.items():
                    if used_pin in pins_num:
                        used_by = sensor_id
                        break
                
                if used_by and used_by != uid:
                    response_params = {
                        'id': uid,
                        'status': '0',
                        'error': f'Pins {pins} already used by sensor {used_by}'
                    }
                    print(f"✗ Pins {pins} conflict: used by {used_by}")
                else:
                    self.connected_sensors[uid] = pins_num
                    response_params = {
                        'id': uid,
                        'status': '1'
                    }
                    print(f"✓ Sensor {uid} connected to pins {pins}")

            except ValueError:
                response_params = {
                    'id': uid,
                    'status': '0',
                    'error': f'Invalid pin number: {pins}'
                }
                print(f"✗ Invalid pin number: {pins}")
        else:
            response_params = {
                'id': uid,
                'status': '0',
                'error': 'Missing sensor ID or pin number'
            }
            print(f"✗ Missing parameters: uid={uid}, pins={pins}")
        
        return self.build_message(response_params)
    
    def handle_disconnect(self, params: Dict[str, str]) -> str:
        """Handle DISCONNECT method - disconnect sensor from pin"""
        uid = params.get('id', '')
        print(f"🔌 DISCONNECT request for sensor: {uid}")
        
        if not self.initialized:
            return self.build_message({'status': '0', 'error': 'Protocol not initialized'})
        
        if uid in self.connected_sensors:
            pin = self.connected_sensors.pop(uid)
            response_params = {
                'id': uid,
                'status': '1',
                'pin': str(pin)
            }
            print(f"✓ Sensor {uid} disconnected from pin {pin}")
        else:
            response_params = {
                'id': uid,
                'status': '0',
                'error': f'Sensor {uid} not connected'
            }
            print(f"✗ Sensor {uid} not connected")
        
        return self.build_message(response_params)
    
    def process_request(self, message: str) -> str:
        """Process incoming protocol request"""
        params = self.parse_message(message)
        request_type = params.get('type', '').upper()
        
        # Route to appropriate handler
        handlers = {
            'INIT': self.handle_init,
            'UPDATE': self.handle_update,
            'CONFIG': self.handle_config,
            'RESET': self.handle_reset,
            'CONNECT': self.handle_connect,
            'DISCONNECT': self.handle_disconnect
        }
        
        if request_type in handlers:
            return handlers[request_type](params)
        else:
            return self.build_message({
                'status': '0',
                'error': f'Unknown request type: {request_type}'
            })
    
    def listen_loop(self):
        """Main listening loop for incoming requests"""
        print("🎧 Listening for protocol requests...")
        buffer = ""
        last_scenario_update = time.time()
        
        while self.running:
            try:
                # Update simulation scenarios periodically
                current_time = time.time()
                if current_time - last_scenario_update > 10:  # Update every 10 seconds
                    self.adjust_simulation_scenario()
                    last_scenario_update = current_time
                
                if self.ser and self.ser.in_waiting > 0:
                    data = self.ser.read(self.ser.in_waiting).decode('utf-8', errors='ignore')
                    buffer += data
                    
                    if data and '\n' in buffer:
                        print(f"DEBUG: {data}") 
                    # Process complete messages (ending with newline or containing '?')
                    while '\n' in buffer or '?' in buffer:
                        if '\n' in buffer:
                            line, buffer = buffer.split('\n', 1)
                        else:
                            # If no newline but contains '?', process the whole buffer
                            line = buffer
                            buffer = ""
                        
                        line = line.strip()
                        # Substring from ? char if exists
                        qmark_index = line.find('?')
                        if qmark_index != -1:
                            line = line[qmark_index:]
                        if line and line.startswith('?'):
                            print(f"📨 Received: {line}")
                            response = self.process_request(line)
                            
                            # Send response
                            if response:
                                self.ser.write((response + '\n').encode('utf-8'))
                                print(f"📤 Sent: {response}")
                
                time.sleep(0.01)  # Small delay to prevent busy waiting
                
            except Exception as e:
                print(f"❌ Error in listen loop: {e}")
                time.sleep(0.1)
    
    def run(self):
        """Start the emulator"""
        print("🚀 Starting Enhanced VSCP Emulator...")
        print(f"   API Version: {self.API_VERSION}")
        print(f"   DB Version: {self.DB_VERSION}")
        print(f"   Available sensors: {list(self.sensor_data.keys())}")
        print("   🎭 Simulation Features:")
        print("      • Realistic peaks and dropdowns")
        print("      • Environmental patterns (daily cycles, weather)")
        print("      • Dynamic scenarios (thermal events, vibrations, etc.)")
        print("      • Sensor-specific noise and trends")
        print("      • Correlated behavior between related sensors")
        
        if not self.connect_serial():
            return
        
        self.running = True
        
        # Start listening thread
        listen_thread = threading.Thread(target=self.listen_loop, daemon=True)
        listen_thread.start()
        
        try:
            print("\n💡 Enhanced emulator ready! Realistic sensor data patterns active.")
            print("   Example: ?type=INIT&app=TestApp&version=1.0.0&dbversion=1.0.0&api=1.2")
            print("   Press Ctrl+C to stop\n")
            
            # Keep main thread alive and show simulation status
            last_status = time.time()
            while True:
                time.sleep(1)
                current_time = time.time()
                if current_time - last_status > 30:  # Show status every 30 seconds
                    elapsed = int(current_time - self.start_time)
                    print(f"   📊 Runtime: {elapsed}s | Mode: {self.simulation_mode}")
                    last_status = current_time
                
        except KeyboardInterrupt:
            print("\n🛑 Shutting down enhanced emulator...")
        
        finally:
            self.running = False
            self.disconnect_serial()

def main():
    """Main entry point"""
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
    emulator = VSCPEmulator(port=port, baudrate=115200)
    emulator.run()

if __name__ == "__main__":
    main()
