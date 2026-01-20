/**
 * @file sensors.hpp
 * @brief Declaration and implementation of sensor classes for sensor management using built-in exceptions.
 *
 * These sensors are derived from the BaseSensor class and provide specific implementations for ADC, Joystick, DHT11, LinearHallAndDigital, PhotoResistor, and LinearHall sensors.
 *
 * @copyright 2025 MTA
 * @author Ing. Jiri Konecny
 *
 */
#ifndef SENSORS_HPP
#define SENSORS_HPP
/*********************
 *      INCLUDES
 *********************/
#include "base_sensor.hpp" ///< BaseSensor class.

/**************************************************************************/
// EMULATED SENSORS
/**************************************************************************/

/* Microphone Sensor 
Emulated microphone sensor returning dBFS and peak values.
*/
class MicrophoneSensor : public BaseSensor
{
public:
    /**
     * @brief Constructs a new object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    MicrophoneSensor(std::string uid) : BaseSensor(uid) { init(); }

    /**
     * @brief Virtual destructor.
     */
    virtual ~MicrophoneSensor() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */

    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "SLM (dBFS)";
        Description = "Emulated microphone sensor";
        

        try
        {
            // Default values
            addValueParameter("dBFS", {"0.0", "dBm", SensorDataType::FLOAT});
            addValueParameter("peak", {"0.0", "dBm", SensorDataType::FLOAT});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};


/* Camera Sensor
Emulated camera sensor returning estimated lux value.
*/
class CameraSensor : public BaseSensor
{
public:
    /**
     * @brief Constructs a new object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    CameraSensor(std::string uid) : BaseSensor(uid) { init(); }

    /**
     * @brief Virtual destructor.
     */
    virtual ~CameraSensor() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */

    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "CAM Lux meter";
        Description = "Emulated camera-lux meter sensor";
        

        try
        {
            // Default values
            addValueParameter("lux_est", {"0.0", "lux", SensorDataType::FLOAT});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/* CPU Temperature Sensor
Emulated CPU temperature sensor returning real temperature value.
*/
class CpuTempSensor : public BaseSensor
{
public:
    /**
     * @brief Constructs a new object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    CpuTempSensor(std::string uid) : BaseSensor(uid) { init(); }

    /**
     * @brief Virtual destructor.
     */
    virtual ~CpuTempSensor() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */

    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "CPU Temp";
        Description = "Emulated cpu real temperature sensor";
        

        try
        {
            // Default values
            addValueParameter("temp", {"0.0", "C", SensorDataType::FLOAT});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/
// SENSORS
/**************************************************************************/

/**
 * @class ADC
 * @brief ADC sensor class derived from BaseSensor.
 *
 * Represents an Analog-to-Digital Converter (ADC) sensor. Implements initialization, configuration,
 * updating, and printing specific to ADC sensors.
 */
class ADC : public BaseSensor
{
public:
    /**
     * @brief Constructs a new ADC object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    ADC(std::string uid) : BaseSensor(uid) { init(); }

    /**
     * @brief Virtual destructor.
     */
    virtual ~ADC() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */

    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "ADC";
        Description = "Analog to Digital Converter";
        

        try
        {
            // Default configs
            addConfigParameter("resolution", {"12", "bits", SensorDataType::INT, 0});
            // Default values
            addValueParameter("value", {"0", "", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }


};

/**************************************************************************/

/**
 * @class Joystick
 * @brief Joystick sensor class derived from BaseSensor.
 *
 * Represents a Joystick as a peripheral constructed as sensor. Implements initialization, configuration,
 * updating, and printing specific to Joystick sensors.
 */
class Joystick : public BaseSensor
{
public:
    Joystick(std::string uid) : BaseSensor(uid) { init(); }
    virtual ~Joystick() {}
    virtual void init() override
    {
        Type = "Joystick";
        Description = "Joystick peripheral";
        
        try
        {
            addValueParameter("XCoordination", {"50", "%", SensorDataType::INT, 0});
            addValueParameter("YCoordination", {"50", "%", SensorDataType::INT, 0});
            addValueParameter("Button", {"0", "ON/OFF", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class DHT11
 * @brief DHT11 sensor class derived from BaseSensor.
 *
 * Represents a DHT11 as a temperature and humidity sensor. Implements initialization, configuration,
 * updating, and printing specific to DHT11 sensors.
 */
class DHT11 : public BaseSensor
{
public:
    DHT11(std::string uid) : BaseSensor(uid) { init(); }
    virtual ~DHT11() {}

    virtual void init() override
    {
        Type = "DHT11";
        Description = "DHT11 Temperature & Humidity sensor";
        

        addConfigParameter("Unit", {"", "", SensorDataType::STRING, 0});
        addValueParameter("temp", {"0", "°C", SensorDataType::INT, 0});
        addValueParameter("humi", {"0", "%", SensorDataType::INT, 0});
    }
};

/**************************************************************************/

/**
 * @class LinearHallAndDigital
 * @brief LinearHallAndDigital sensor class derived from BaseSensor.
 *
 * Represents a Linear Hall sensor, which returns strenght of a magnet in milliTesla. Implements initialization, configuration,
 * updating, and printing specific to LinearHallAndDigital sensors.
 */
class LinearHallAndDigital : public BaseSensor
{
public:
    /**
     * @brief Constructs a new LinearHallAndDigital object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    LinearHallAndDigital(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~LinearHallAndDigital() {}
    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "LinearHallAndDigital";
        Description = "Returns milliTesla of a measured magnet and if he goes past linearity";
        

        try
        {
            // Default configs
            addConfigParameter("precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("milliTesla Meter", {"0", "milliTesla", SensorDataType::FLOAT, 0});
            addValueParameter("Magnet Detector", {"0", "", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class PhotoResistor
 * @brief PhotoResistor sensor class derived from BaseSensor.
 *
 * Represents a PhotoResistor which measures lux in users environment. Implements initialization, configuration,
 * updating, and printing specific to PhotoResistor sensors.
 */
class PhotoResistor : public BaseSensor
{
public:
    /**
     * @brief Constructs a new PhotoResistor object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    PhotoResistor(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~PhotoResistor() {}
    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "PhotoResistor";
        Description = "Returns Lux of a measured environment, which users is in";
        

        try
        {
            // Default configs
            addConfigParameter("Res", {"5", "digits", SensorDataType::INT, 0});
            // Default values
            addValueParameter("intensity", {"0", "Lux", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class LinearHall
 * @brief LinearHall sensor class derived from BaseSensor.
 *
 * Represents a Linear Hall sensor, which returns strenght of a magnet in gauss. Implements initialization, configuration,
 * updating, and printing specific to LinearHall sensors.
 */
class LinearHall : public BaseSensor
{
public:
    /**
     * @brief Constructs a new LinearHall object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    LinearHall(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~LinearHall() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "LinearHall";
        Description = "Returns milliTesla of a measured magnet";
        

        try
        {
            // Default configs
            addConfigParameter("precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("milliTesla", {"0", "milliTesla", SensorDataType::FLOAT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class DigitalTemperature
 * @brief DigitalTemperature sensor class derived from BaseSensor.
 *
 * Represents a Temperature sensor, which returns °C and ON/OFF if the temperature goes past hardware-configured value. Implements initialization, configuration,
 * updating, and printing specific to DigitalTemperature sensors.
 */
class DigitalTemperature : public BaseSensor
{
public:
    /**
     * @brief Constructs a new DigitalTemperature object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    DigitalTemperature(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~DigitalTemperature() {}
    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "DS18B20";
        Description = "Returns temperature in °C and if the temperature goes past a hardware-configured value";
        

        try
        {
            // Default configs
            addConfigParameter("Res", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("temp", {"0", "°C", SensorDataType::FLOAT, 0});
            addValueParameter("alarm", {"0", "", SensorDataType::STRING, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class AnalogTemperature
 * @brief AnalogTemperature sensor class derived from BaseSensor.
 *
 * Represents a Temperature sensor, which returns °C. Implements initialization, configuration,
 * updating, and printing specific to AnalogTemperature sensors.
 */
class AnalogTemperature : public BaseSensor
{
public:
    /**
     * @brief Constructs a new AnalogTemperature object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    AnalogTemperature(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~AnalogTemperature() {}
    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "AnalogTemperature";
        Description = "Returns temperature in °C";
        

        try
        {
            // Default configs
            addConfigParameter("precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("Temperature", {"0", "°C", SensorDataType::FLOAT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class TH
 * @brief Temperature/Huminidy sensor class derived from BaseSensor.
 *
 * Represents a Temperature/Huminidy sensor. Implements initialization, configuration, updating, and printing
 * specific to Temperature/Huminidy sensors.
 */
class TH : public BaseSensor
{
public:
    /**
     * @brief Constructs a new TH object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    TH(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~TH() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "TH";
        Description = "Temperature & Humidity Sensor";
        

        try
        {
            // Default configs
            addConfigParameter("precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("temperature", {"0", "Celsia", SensorDataType::FLOAT, 0});
            addValueParameter("humidity", {"0", "%", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/
// DIGITAL SENSORS
/**************************************************************************/

/**
 * @class DigitalHall
 * @brief DigitalHall sensor class derived from BaseSensor.
 *
 * Represents a Linear Hall sensor, which returns 1 if it in a presence of a magnet. Implements initialization, configuration,
 * updating, and printing specific to DigitalHall sensors.
 */
class DigitalHall : public BaseSensor
{
public:
    /**
     * @brief Constructs a new DigitalHall object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    DigitalHall(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~DigitalHall() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "DigitalHall";
        Description = "Returns 1 of a measured magnet and if he goes past linearity";
        

        try
        {
            // Default configs
            addConfigParameter("resolution", {"1", "bits", SensorDataType::INT, 0});
            // Default values
            addValueParameter("Magnet Detector", {"0", "", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/

/**
 * @class PhotoInterrupter
 * @brief PhotoInterrupter sensor class derived from BaseSensor.
 *
 * Represents a PhotoInterrupter, which returns 1 if something breaks line between IR light. Implements initialization, configuration,
 * updating, and printing specific to PhotoInterrupter sensors.
 */
class PhotoInterrupter : public BaseSensor
{
public:
    /**
     * @brief Constructs a new PhotoInterrupter object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    PhotoInterrupter(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~PhotoInterrupter() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "PhotoInterrupter";
        Description = "Returns 1 of a measured magnet and if he goes past linearity";
        

        try
        {
            // Default values
            addValueParameter("Motion Detector", {"0", "", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};
/**************************************************************************/
// I2C
/**************************************************************************/
/**************************************************************************/
/**
 * @class TP
 * @brief Temperature/Pressure sensor class derived from BaseSensor.
 *
 * Represents a Temperature/Pressure sensor. Implements initialization, configuration, updating, and printing
 * specific to Temperature/Pressure sensors.
 */

class TP : public BaseSensor
{
public:
    /**
     * @brief Constructs a new TP object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    TP(std::string uid) : BaseSensor(uid)
    {
        init();
    }
    /**
     * @brief Virtual destructor.
     */
    virtual ~TP() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "TP";
        Description = "Temperature & Pressure Sensor";
        

        try
        {
            // Default configs
            addConfigParameter("Precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("Temperature", {"0", "°C", SensorDataType::FLOAT, 0});
            addValueParameter("Pressure", {"0", "hPa", SensorDataType::FLOAT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/
/**
 * @class GAT
 * @brief Gyroscope/Accelerometr/Temperature sensor class derived from BaseSensor.
 *
 * Represents a Gyroscope/Accelerometr/Temperature sensor. Implements initialization, configuration, updating, and printing
 * specific to Gyroscope/Accelerometr/Temperature sensors.
 */

class GAT : public BaseSensor
{
public:
    /**
     * @brief Constructs a new GAT object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    GAT(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~GAT() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "GAT";
        Description = "Gyroscope/Accelerometr/Temperature sensor";
        

        try
        {
            // Default configs
            addConfigParameter("Precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("Temperature", {"0", "°C", SensorDataType::FLOAT, 0});
            addValueParameter("acm_x", {"0", "g", SensorDataType::FLOAT, 0});
            addValueParameter("acm_y", {"0", "g", SensorDataType::FLOAT, 0});
            addValueParameter("acm_z", {"0", "g", SensorDataType::FLOAT, 0});
            addValueParameter("gyr_x", {"0", "°/s", SensorDataType::FLOAT, 0});
            addValueParameter("gyr_y", {"0", "°/s", SensorDataType::FLOAT, 0});
            addValueParameter("gyr_z", {"0", "°/s", SensorDataType::FLOAT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};

/**************************************************************************/
/**
 * @class TOF
 * @brief Time of flight sensor class derived from BaseSensor.
 *
 * Represents a Time of flight sensor. Implements initialization, configuration, updating, and printing
 * specific to Time of flight sensors.
 */

class TOF : public BaseSensor
{
public:
    /**
     * @brief Constructs a new GAT object.
     *
     * Initializes default values and sets the sensor type and description.
     *
     * @param uid The unique sensor identifier.
     */
    TOF(std::string uid) : BaseSensor(uid)
    {
        init();
    }

    /**
     * @brief Virtual destructor.
     */
    virtual ~TOF() {}

    /**
     * @brief Initializes the sensor.
     *
     * Additional initialization code can be added here.
     *
     * @throws Exception if initialization fails.
     */
    virtual void init() override
    {
        // Additional initialization for sensor can be added here.
        Type = "TOF";
        Description = "Time of flight sensor";
        

        try
        {
            // Default configs
            addConfigParameter("Precision", {"2", "decimals", SensorDataType::INT, 0});
            // Default values
            addValueParameter("dist", {"0", "mm", SensorDataType::INT, 0});
        }
        catch (const std::exception &e)
        {
            throw;
        }
    }
};
#endif // SENSORS_HPP