#define DEFAULT_JSON_DB R"json({
  "version": "1.3",
  "application": "board",
  "devices": {
    "S00": {
      "uid": "S00",
      "role": "sensor",
      "type": "DS18B20",
      "description": "1-Wire digital thermometer with programmable 9-12 bit resolution and nonvolatile low/high temperature alarms.",
      "values": {
        "temp": {
          "value": 0.0,
          "unit": "C",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -55,
            "max": 125
          }
        },
        "alarm": {
          "value": "OK",
          "unit": "",
          "dtype": "string",
          "access": "read",
          "restrictions": {
            "options": [
              "OK",
              "LOW",
              "HIGH"
            ]
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "lowalarm": {
          "value": -55,
          "unit": "C",
          "dtype": "int",
          "restrictions": {
            "min": -55,
            "max": 125,
            "step": 1
          }
        },
        "highalarm": {
          "value": 125,
          "unit": "C",
          "dtype": "int",
          "restrictions": {
            "min": -55,
            "max": 125,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "temp": 0.0,
          "alarm": "OK"
        },
        "configs": {
          "res": 12,
          "lowalarm": -55,
          "highalarm": 125
        },
        "pins": {}
      },
      "picture": "S00_DS18B20.png",
      "Pins": [
        "DQ"
      ]
    },
    "S01": {
      "uid": "S01",
      "role": "sensor",
      "type": "DHT11",
      "description": "Digital temperature and relative-humidity sensor. Slow-response device intended for non-condensing ambient measurements.",
      "values": {
        "humi": {
          "value": 0.0,
          "unit": "%RH",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 5,
            "max": 95
          }
        },
        "temp": {
          "value": 0.0,
          "unit": "C",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -20,
            "max": 60
          }
        }
      },
      "configs": {
        "unit": {
          "value": "C",
          "unit": "",
          "dtype": "string",
          "restrictions": {
            "options": [
              "C",
              "F"
            ]
          }
        },
        "hi": {
          "value": "false",
          "unit": "",
          "dtype": "string",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        }
      },
      "default": {
        "values": {
          "humi": 0.0,
          "temp": 0.0
        },
        "configs": {
          "unit": "C",
          "hi": "false"
        },
        "pins": {}
      },
      "picture": "S01_DHT11.png",
      "Pins": [
        "DATA"
      ]
    },
    "S02": {
      "uid": "S02",
      "role": "sensor",
      "type": "Digital Hall Sensor",
      "description": "Digital Hall-effect magnetic detector; reports the module comparator output as a binary state.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S02_DigitalHall.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S03": {
      "uid": "S03",
      "role": "sensor",
      "type": "Analog Hall Sensor",
      "description": "Analog Hall-effect magnetic sensor with configurable ADC resolution and thresholds for north/south polarity classification.",
      "values": {
        "val": {
          "value": 0,
          "unit": "ADC",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 4095
          }
        },
        "polarity": {
          "value": "NO MAGNET",
          "unit": "",
          "dtype": "string",
          "access": "read",
          "restrictions": {
            "options": [
              "SOUTH",
              "NO MAGNET",
              "NORTH"
            ]
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "llimit": {
          "value": 1800,
          "unit": "ADC",
          "dtype": "int",
          "restrictions": {
            "min": 0,
            "max": 4095,
            "step": 1
          }
        },
        "hlimit": {
          "value": 2000,
          "unit": "ADC",
          "dtype": "int",
          "restrictions": {
            "min": 0,
            "max": 4095,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "val": 0,
          "polarity": "NO MAGNET"
        },
        "configs": {
          "res": 12,
          "llimit": 1800,
          "hlimit": 2000
        },
        "pins": {}
      },
      "picture": "S03_AnalogHall.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S04": {
      "uid": "S04",
      "role": "sensor",
      "type": "Photo Interrupter",
      "description": "Slot-type infrared photo-interrupter; the binary output changes when an opaque object blocks the optical path.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S04_PhotoInterrupter.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S05": {
      "uid": "S05",
      "role": "sensor",
      "type": "FC-51 Obstacle Sensor",
      "description": "Adjustable infrared reflective obstacle detector with a binary comparator output; range depends strongly on target reflectivity and ambient light.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S05_FC51.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S06": {
      "uid": "S06",
      "role": "sensor",
      "type": "HC-SR04",
      "description": "Ultrasonic time-of-flight distance module. Hardware range is approximately 2-400 cm; the firmware default maximum is 150 cm.",
      "values": {
        "distance": {
          "value": 0.0,
          "unit": "cm",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 400
          }
        }
      },
      "configs": {
        "limit": {
          "value": 150,
          "unit": "cm",
          "dtype": "int",
          "restrictions": {
            "min": 2,
            "max": 400,
            "step": 1
          }
        },
        "delay": {
          "value": 40,
          "unit": "ms",
          "dtype": "int",
          "restrictions": {
            "min": 29,
            "max": 1000,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "distance": 0.0
        },
        "configs": {
          "limit": 150,
          "delay": 40
        },
        "pins": {}
      },
      "picture": "S06_HCSR04.png",
      "Pins": [
        "TRIG",
        "ECHO"
      ]
    },
    "S07": {
      "uid": "S07",
      "role": "sensor",
      "type": "HC-SR501 PIR",
      "description": "Passive-infrared motion detector with a binary motion output; allow warm-up after power-on and avoid direct heat or sunlight.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S07_HCSR501.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S08": {
      "uid": "S08",
      "role": "sensor",
      "type": "KW11-3Z Limit Switch",
      "description": "Snap-action mechanical limit microswitch read as a binary state. Wire the selected NO or NC contact consistently with the application.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S08_KW113Z.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S09": {
      "uid": "S09",
      "role": "sensor",
      "type": "BMP280",
      "description": "I2C digital barometric pressure and temperature sensor with configurable oversampling and IIR filtering.",
      "values": {
        "temp": {
          "value": 0.0,
          "unit": "C",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -40,
            "max": 85
          }
        },
        "press": {
          "value": 1013.25,
          "unit": "hPa",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 300,
            "max": 1100
          }
        }
      },
      "configs": {
        "os_temp": {
          "value": 8,
          "unit": "x",
          "dtype": "int",
          "restrictions": {
            "options": [
              1,
              2,
              4,
              8,
              16
            ]
          }
        },
        "os_press": {
          "value": 16,
          "unit": "x",
          "dtype": "int",
          "restrictions": {
            "options": [
              1,
              2,
              4,
              8,
              16
            ]
          }
        },
        "filter": {
          "value": 4,
          "unit": "x",
          "dtype": "int",
          "restrictions": {
            "options": [
              0,
              2,
              4,
              8,
              16
            ]
          }
        }
      },
      "default": {
        "values": {
          "temp": 0.0,
          "press": 1013.25
        },
        "configs": {
          "os_temp": 8,
          "os_press": 16,
          "filter": 4
        },
        "pins": {}
      },
      "picture": "S09_BMP280.png",
      "Pins": [
        "SDA",
        "SCL"
      ]
    },
    "S10": {
      "uid": "S10",
      "role": "sensor",
      "type": "BMP180",
      "description": "Legacy I2C barometric pressure sensor. Firmware reports calibrated pressure and altitude estimated from standard sea-level pressure.",
      "values": {
        "press": {
          "value": 1013.25,
          "unit": "hPa",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 300,
            "max": 1100
          }
        },
        "altitude": {
          "value": 0.0,
          "unit": "m",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -500,
            "max": 9000
          }
        }
      },
      "configs": {
        "gain": {
          "value": 1.0,
          "unit": "x",
          "dtype": "float",
          "restrictions": {
            "min": 0.1,
            "max": 10,
            "step": 0.1
          }
        }
      },
      "default": {
        "values": {
          "press": 1013.25,
          "altitude": 0.0
        },
        "configs": {
          "gain": 1.0
        },
        "pins": {}
      },
      "picture": "S10_BMP180.png",
      "Pins": [
        "SDA",
        "SCL"
      ]
    },
    "S11": {
      "uid": "S11",
      "role": "sensor",
      "type": "TCS34725",
      "description": "I2C RGB color-light sensor with IR-blocking filter. Firmware normalizes the measured channels to 8-bit R, G and B values.",
      "values": {
        "R": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 255,
            "step": 1
          }
        },
        "G": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 255,
            "step": 1
          }
        },
        "B": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 255,
            "step": 1
          }
        }
      },
      "configs": {
        "itime": {
          "value": 600,
          "unit": "ms",
          "dtype": "int",
          "restrictions": {
            "options": [
              2,
              50,
              101,
              199,
              300,
              401,
              499,
              600
            ]
          }
        },
        "gain": {
          "value": 4,
          "unit": "x",
          "dtype": "int",
          "restrictions": {
            "options": [
              1,
              4,
              16,
              60
            ]
          }
        }
      },
      "default": {
        "values": {
          "R": 0,
          "G": 0,
          "B": 0
        },
        "configs": {
          "itime": 600,
          "gain": 4
        },
        "pins": {}
      },
      "picture": "S11_TCS34725.png",
      "Pins": [
        "SDA",
        "SCL"
      ]
    },
    "S12": {
      "uid": "S12",
      "role": "sensor",
      "type": "Infrared Receiver",
      "description": "Asynchronous infrared remote-control receiver. Firmware returns the latest decoded 32-bit code as hexadecimal text and suppresses rapid duplicates.",
      "values": {
        "code": {
          "value": "0x00000000",
          "unit": "",
          "dtype": "string",
          "access": "read"
        }
      },
      "configs": {
        "dedup": {
          "value": 150,
          "unit": "ms",
          "dtype": "int",
          "restrictions": {
            "min": 0,
            "max": 5000,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "code": "0x00000000"
        },
        "configs": {
          "dedup": 150
        },
        "pins": {}
      },
      "picture": "S12_IRReceiver.png",
      "Pins": [
        "DATA"
      ]
    },
    "S13": {
      "uid": "S13",
      "role": "sensor",
      "type": "Digital NTC Module",
      "description": "NTC thermistor threshold module with comparator output. Reports only whether the adjustable threshold is crossed, not temperature.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S13_DigitalNTC.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S14": {
      "uid": "S14",
      "role": "sensor",
      "type": "Analog NTC Thermistor",
      "description": "Analog 10 kOhm NTC thermistor divider converted to temperature with a Steinhart-Hart approximation and optional averaging.",
      "values": {
        "temp": {
          "value": 0.0,
          "unit": "C",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -40,
            "max": 125
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "filter": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "restrictions": {
            "options": [
              0,
              1,
              2
            ]
          }
        }
      },
      "default": {
        "values": {
          "temp": 0.0
        },
        "configs": {
          "res": 12,
          "filter": 0
        },
        "pins": {}
      },
      "picture": "S14_AnalogNTC.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S15": {
      "uid": "S15",
      "role": "sensor",
      "type": "Photoresistor",
      "description": "Analog light sensor using an LDR voltage divider. Lux is an approximate computed value and should be calibrated with the gain setting.",
      "values": {
        "intensity": {
          "value": 0.0,
          "unit": "lux",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 100000
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "gain": {
          "value": 1.0,
          "unit": "x",
          "dtype": "float",
          "restrictions": {
            "min": 0.01,
            "max": 100,
            "step": 0.01
          }
        }
      },
      "default": {
        "values": {
          "intensity": 0.0
        },
        "configs": {
          "res": 12,
          "gain": 1.0
        },
        "pins": {}
      },
      "picture": "S15_Photoresistor.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S16": {
      "uid": "S16",
      "role": "sensor",
      "type": "XY Joystick",
      "description": "Two-axis analog joystick with push switch. Firmware self-calibrates at rest and reports a discrete direction or CLICK.",
      "values": {
        "direction": {
          "value": "CENTER",
          "unit": "",
          "dtype": "string",
          "access": "read",
          "restrictions": {
            "options": [
              "CENTER",
              "UP",
              "DOWN",
              "LEFT",
              "RIGHT",
              "CLICK"
            ]
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "threshold": {
          "value": 25,
          "unit": "%",
          "dtype": "int",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "direction": "CENTER"
        },
        "configs": {
          "res": 12,
          "threshold": 25
        },
        "pins": {}
      },
      "picture": "S16_Joystick.png",
      "Pins": [
        "X",
        "Y",
        "SW"
      ]
    },
    "S17": {
      "uid": "S17",
      "role": "sensor",
      "type": "Linear Hall Sensor",
      "description": "Linear analog Hall-effect sensor. Select raw ADC counts, output voltage, or estimated magnetic induction; induction scaling is firmware-specific and should be calibrated.",
      "values": {
        "ADC": {
          "value": 0.0,
          "unit": "count",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 4095
          }
        },
        "Voltage": {
          "value": 0.0,
          "unit": "V",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 3.3
          }
        },
        "Induction": {
          "value": 0.0,
          "unit": "mT",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": -66,
            "max": 66
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "unit": {
          "value": "Voltage",
          "unit": "",
          "dtype": "string",
          "restrictions": {
            "options": [
              "ADC",
              "Voltage",
              "Induction"
            ]
          }
        }
      },
      "default": {
        "values": {
          "ADC": 0.0,
          "Voltage": 0.0,
          "Induction": 0.0
        },
        "configs": {
          "res": 12,
          "unit": "Voltage"
        },
        "pins": {}
      },
      "picture": "S17_LinearHall.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S18": {
      "uid": "S18",
      "role": "sensor",
      "type": "MQ-135 Digital Output",
      "description": "MQ-135 air-quality module comparator output. This binary channel indicates an adjustable threshold only and is not a calibrated ppm reading; the heater needs warm-up.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S18_MQ135.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S19": {
      "uid": "S19",
      "role": "sensor",
      "type": "Digital Soil Moisture",
      "description": "Resistive soil-moisture threshold module with binary comparator output. Avoid continuous probe energization where corrosion matters.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S19_SoilMoisture.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S20": {
      "uid": "S20",
      "role": "sensor",
      "type": "TTP223 Touch Sensor",
      "description": "Single-key capacitive touch detector with binary output; module solder options may select active level and momentary/toggle behavior.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S20_TTP223.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S21": {
      "uid": "S21",
      "role": "sensor",
      "type": "GP2Y0A21YK0F",
      "description": "Sharp analog infrared triangulation distance sensor. Datasheet measuring range is 10-80 cm; firmware currently clamps output to 19-81 cm.",
      "values": {
        "distance": {
          "value": 19.0,
          "unit": "cm",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 19,
            "max": 81
          }
        },
        "alarm": {
          "value": "LOW",
          "unit": "",
          "dtype": "string",
          "access": "read",
          "restrictions": {
            "options": [
              "OK",
              "LOW",
              "HIGH"
            ]
          }
        }
      },
      "configs": {
        "unit": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "restrictions": {
            "options": [
              0,
              1
            ]
          }
        },
        "lowalarm": {
          "value": 22.0,
          "unit": "cm",
          "dtype": "float",
          "restrictions": {
            "min": 19,
            "max": 81,
            "step": 0.1
          }
        },
        "highalarm": {
          "value": 78.0,
          "unit": "cm",
          "dtype": "float",
          "restrictions": {
            "min": 19,
            "max": 81,
            "step": 0.1
          }
        }
      },
      "default": {
        "values": {
          "distance": 19.0,
          "alarm": "LOW"
        },
        "configs": {
          "unit": 0,
          "lowalarm": 22.0,
          "highalarm": 78.0
        },
        "pins": {}
      },
      "picture": "S21_GP2Y0A21YK0F.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S22": {
      "uid": "S22",
      "role": "sensor",
      "type": "Rotary Encoder",
      "description": "Incremental quadrature rotary encoder with configurable direction and low/high position alarms.",
      "values": {
        "position": {
          "value": 0,
          "unit": "count",
          "dtype": "int",
          "access": "read"
        },
        "alarm": {
          "value": "OK",
          "unit": "",
          "dtype": "string",
          "access": "read",
          "restrictions": {
            "options": [
              "OK",
              "LOW",
              "HIGH"
            ]
          }
        }
      },
      "configs": {
        "direction": {
          "value": "normal",
          "unit": "",
          "dtype": "string",
          "restrictions": {
            "options": [
              "normal",
              "reverse"
            ]
          }
        },
        "lowalarm": {
          "value": -100,
          "unit": "count",
          "dtype": "int",
          "restrictions": {
            "min": -1000000,
            "max": 1000000,
            "step": 1
          }
        },
        "highalarm": {
          "value": 100,
          "unit": "count",
          "dtype": "int",
          "restrictions": {
            "min": -1000000,
            "max": 1000000,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "position": 0,
          "alarm": "OK"
        },
        "configs": {
          "direction": "normal",
          "lowalarm": -100,
          "highalarm": 100
        },
        "pins": {}
      },
      "picture": "S22_RotaryEncoder.png",
      "Pins": [
        "B",
        "A"
      ]
    },
    "S23": {
      "uid": "S23",
      "role": "sensor",
      "type": "HS0038B IR Detector",
      "description": "38 kHz demodulating infrared remote-control receiver read as a raw binary level; intended for modulated IR, not ambient-light measurement.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S23_HS0038B.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S24": {
      "uid": "S24",
      "role": "sensor",
      "type": "TCRT5000 Reflective Sensor",
      "description": "Short-range reflective infrared line/object sensor with binary comparator output; response depends on surface color, distance and ambient light.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S24_TCRT5000.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S25": {
      "uid": "S25",
      "role": "sensor",
      "type": "IR Flame Sensor",
      "description": "Infrared flame/light threshold module with binary comparator output. It is not a certified fire-safety detector.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S25_FlameSensor.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S26": {
      "uid": "S26",
      "role": "sensor",
      "type": "Reed Switch",
      "description": "Magnetically operated reed contact read as a binary state; avoid mechanical shock and loads beyond the sealed contact rating.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S26_ReedSwitch.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S27": {
      "uid": "S27",
      "role": "sensor",
      "type": "Small Microphone Module",
      "description": "Analog microphone envelope/amplitude module. Firmware reports an uncalibrated relative logarithmic volume, not sound-pressure level.",
      "values": {
        "volume": {
          "value": 0.0,
          "unit": "dB rel.",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 73
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "time": {
          "value": 50,
          "unit": "ms",
          "dtype": "int",
          "restrictions": {
            "min": 1,
            "max": 5000,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "volume": 0.0
        },
        "configs": {
          "res": 12,
          "time": 50
        },
        "pins": {}
      },
      "picture": "S27_MicrophoneSmall.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S28": {
      "uid": "S28",
      "role": "sensor",
      "type": "Large Microphone Module",
      "description": "Analog microphone envelope/amplitude module. Firmware reports an uncalibrated relative logarithmic volume, not sound-pressure level.",
      "values": {
        "volume": {
          "value": 0.0,
          "unit": "dB rel.",
          "dtype": "float",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 73
          }
        }
      },
      "configs": {
        "res": {
          "value": 12,
          "unit": "bit",
          "dtype": "int",
          "restrictions": {
            "min": 9,
            "max": 12,
            "step": 1
          }
        },
        "time": {
          "value": 50,
          "unit": "ms",
          "dtype": "int",
          "restrictions": {
            "min": 1,
            "max": 5000,
            "step": 1
          }
        }
      },
      "default": {
        "values": {
          "volume": 0.0
        },
        "configs": {
          "res": 12,
          "time": 50
        },
        "pins": {}
      },
      "picture": "S28_MicrophoneLarge.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S29": {
      "uid": "S29",
      "role": "sensor",
      "type": "Metal Touch Sensor",
      "description": "Conductive metal-touch switch module with a binary output; sensitivity depends on grounding, electrode area and electrical noise.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S29_MetalTouch.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S30": {
      "uid": "S30",
      "role": "sensor",
      "type": "Pulse/Heartbeat Sensor",
      "description": "Optical pulse-wave sensor. Firmware estimates beats per minute over a 5-second window; this is an educational sensor, not a medical device.",
      "values": {
        "bpm": {
          "value": 0,
          "unit": "bpm",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 240,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "bpm": 0
        },
        "pins": {}
      },
      "picture": "S30_Heartbeat.png",
      "Pins": [
        "AOUT"
      ]
    },
    "S31": {
      "uid": "S31",
      "role": "sensor",
      "type": "Push Button",
      "description": "Momentary mechanical push-button input reported as a binary state; external module circuitry determines active polarity.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S31_Button.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S32": {
      "uid": "S32",
      "role": "sensor",
      "type": "Tilt Switch",
      "description": "Ball-contact tilt/orientation switch reported as a binary state. It indicates orientation change, not an absolute angle.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S32_TiltSwitch.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S33": {
      "uid": "S33",
      "role": "sensor",
      "type": "Digital Vibration Sensor",
      "description": "Vibration/shock threshold module with binary output. Sensitivity and pulse duration depend on the module hardware.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S33_Vibration.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S34": {
      "uid": "S34",
      "role": "sensor",
      "type": "Mercury Tilt Switch",
      "description": "Sealed mercury tilt switch reported as a binary state. Handle as hazardous material and do not use if the glass capsule is damaged.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S34_MercurySwitch.png",
      "Pins": [
        "DOUT"
      ]
    },
    "S35": {
      "uid": "S35",
      "role": "sensor",
      "type": "Tap/Knock Sensor",
      "description": "Piezoelectric or spring-type tap/knock detector with binary output; intended for transient impacts rather than calibrated force.",
      "values": {
        "state": {
          "value": 0,
          "unit": "",
          "dtype": "int",
          "access": "read",
          "restrictions": {
            "min": 0,
            "max": 1,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "state": 0
        },
        "pins": {}
      },
      "picture": "S35_TapSensor.png",
      "Pins": [
        "DOUT"
      ]
    },
    "A00": {
      "uid": "A00",
      "role": "actuator",
      "type": "SG90 Servo",
      "description": "9 g hobby positional servo controlled by angle and movement-speed percentage. Use an adequate external supply; firmware constrains angle to 0-180 degrees.",
      "values": {
        "angle": {
          "value": 42,
          "unit": "deg",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 180,
            "step": 1
          }
        },
        "speed": {
          "value": 100,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "angle": 42,
          "speed": 100
        },
        "pins": {}
      },
      "picture": "A00_SG90.png",
      "Pins": [
        "PWM"
      ]
    },
    "A01": {
      "uid": "A01",
      "role": "actuator",
      "type": "4-Wire Stepper Motor",
      "description": "Four-phase stepper motor controlled through a compatible driver. Commanded angle is relative; direction and RPM are supplied with each control update.",
      "values": {
        "angle": {
          "value": 42,
          "unit": "deg",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 3600,
            "step": 1
          }
        },
        "dir": {
          "value": "true",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        },
        "rpm": {
          "value": 16,
          "unit": "rpm",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 1,
            "max": 20,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "angle": 42,
          "dir": "true",
          "rpm": 16
        },
        "pins": {}
      },
      "picture": "A01_Stepper.png",
      "Pins": [
        "IN1",
        "IN2",
        "IN3",
        "IN4"
      ]
    },
    "A02": {
      "uid": "A02",
      "role": "actuator",
      "type": "DC Motor",
      "description": "Single-direction PWM DC-motor output with speed and run/stop control. Drive the motor through a transistor or motor driver, never directly from a GPIO.",
      "values": {
        "speed": {
          "value": 50,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        },
        "state": {
          "value": "true",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "speed": 50,
          "state": "true"
        },
        "pins": {}
      },
      "picture": "A02_DCMotor.png",
      "Pins": [
        "PWM"
      ]
    },
    "A03": {
      "uid": "A03",
      "role": "actuator",
      "type": "Two-Color LED",
      "description": "Red/green two-channel LED with selectable color and PWM brightness. Use appropriate series resistors.",
      "values": {
        "color": {
          "value": "R",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "R",
              "G"
            ]
          }
        },
        "brig": {
          "value": 50,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "color": "R",
          "brig": 50
        },
        "pins": {}
      },
      "picture": "A03_TwoColorLED.png",
      "Pins": [
        "R",
        "G"
      ]
    },
    "A04": {
      "uid": "A04",
      "role": "actuator",
      "type": "Mini Two-Color LED",
      "description": "Red/green two-channel LED with selectable color and PWM brightness. Use appropriate series resistors.",
      "values": {
        "color": {
          "value": "G",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "R",
              "G"
            ]
          }
        },
        "brig": {
          "value": 100,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "color": "G",
          "brig": 100
        },
        "pins": {}
      },
      "picture": "A04_TwoColorMini.png",
      "Pins": [
        "R",
        "G"
      ]
    },
    "A05": {
      "uid": "A05",
      "role": "actuator",
      "type": "RGB LED",
      "description": "Three-channel RGB LED with independent PWM brightness from 0 to 100 percent per color. Use suitable series resistors.",
      "values": {
        "brigr": {
          "value": 0,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        },
        "brigg": {
          "value": 0,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        },
        "brigb": {
          "value": 50,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "brigr": 0,
          "brigg": 0,
          "brigb": 50
        },
        "pins": {}
      },
      "picture": "A05_RGBLED.png",
      "Pins": [
        "R",
        "G",
        "B"
      ]
    },
    "A06": {
      "uid": "A06",
      "role": "actuator",
      "type": "RGB LED",
      "description": "Three-channel RGB LED with independent PWM brightness from 0 to 100 percent per color. Use suitable series resistors.",
      "values": {
        "brigr": {
          "value": 0,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        },
        "brigg": {
          "value": 0,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        },
        "brigb": {
          "value": 50,
          "unit": "%",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 100,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "brigr": 0,
          "brigg": 0,
          "brigb": 50
        },
        "pins": {}
      },
      "picture": "A06_RGBLED.png",
      "Pins": [
        "R",
        "G",
        "B"
      ]
    },
    "A07": {
      "uid": "A07",
      "role": "actuator",
      "type": "Seven-Color Flashing LED",
      "description": "Self-flashing multicolor LED/module with binary enable control. The color sequence is generated by the device hardware.",
      "values": {
        "control": {
          "value": "true",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "control": "true"
        },
        "pins": {}
      },
      "picture": "A07_SevenColorLED.png",
      "Pins": [
        "DIN"
      ]
    },
    "A08": {
      "uid": "A08",
      "role": "actuator",
      "type": "Infrared Transmitter",
      "description": "Infrared LED transmitter. Firmware sends the supplied 32-bit hexadecimal value as an NEC-format frame.",
      "values": {
        "code": {
          "value": "0x00000000",
          "unit": "",
          "dtype": "string",
          "access": "write"
        }
      },
      "configs": {},
      "default": {
        "values": {
          "code": "0x00000000"
        },
        "pins": {}
      },
      "picture": "A08_IRTransmitter.png",
      "Pins": [
        "DIN"
      ]
    },
    "A09": {
      "uid": "A09",
      "role": "actuator",
      "type": "Laser Module",
      "description": "Low-power visible laser module with binary enable control. Avoid eye exposure and reflections; observe the module's laser classification.",
      "values": {
        "control": {
          "value": "true",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "control": "true"
        },
        "pins": {}
      },
      "picture": "A09_Laser.png",
      "Pins": [
        "DIN"
      ]
    },
    "A10": {
      "uid": "A10",
      "role": "actuator",
      "type": "Passive Buzzer",
      "description": "Passive piezo buzzer driven with a selected tone frequency and duration.",
      "values": {
        "freq": {
          "value": 1000,
          "unit": "Hz",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 1,
            "max": 20000,
            "step": 1
          }
        },
        "duration": {
          "value": 500,
          "unit": "ms",
          "dtype": "int",
          "access": "write",
          "restrictions": {
            "min": 0,
            "max": 60000,
            "step": 1
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "freq": 1000,
          "duration": 500
        },
        "pins": {}
      },
      "picture": "A10_PassiveBuzzer.png",
      "Pins": [
        "PWM"
      ]
    },
    "A11": {
      "uid": "A11",
      "role": "actuator",
      "type": "Active Buzzer",
      "description": "Active buzzer module with built-in oscillator and binary on/off control.",
      "values": {
        "control": {
          "value": "true",
          "unit": "",
          "dtype": "string",
          "access": "write",
          "restrictions": {
            "options": [
              "false",
              "true"
            ]
          }
        }
      },
      "configs": {},
      "default": {
        "values": {
          "control": "true"
        },
        "pins": {}
      },
      "picture": "A11_ActiveBuzzer.png",
      "Pins": [
        "DIN"
      ]
    }
  }
}
)json"
