/**
 * @file base_sensor.cpp
 * @brief Implementation of sensor management functions.
 * 
 * This file implements sensor functionalities, including the factory function for creating
 * sensors instances. The factory function dynamically allocates an sensor, calls its
 * initialization method, and returns a pointer. If initialization fails, it logs the error,
 * cleans up, and rethrows the exception.
 * 
 * @copyright 2024 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#include "base_sensor.hpp"

/*Global functions*/


/*Create functions*/
//implemented in header file as generic function

/*General functions*/

bool configSensor(BaseSensor *sensor, const std::string &config) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    try {
        std::unordered_map<std::string, std::string> params = parseParamsFromString(config, CASE_SENSITIVE);
        sensor->config(params);
        return true;
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}


bool updateSensor(BaseSensor *sensor, const std::string &update) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    try {
        std::unordered_map<std::string, std::string> params = parseParamsFromString(update, CASE_SENSITIVE);
        sensor->update(params);
        return true;   
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}

void printSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return;
    }
    sensor->clearError(); // Clear error if config successful
    
    try {
        sensor->print();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return;
    }
}

bool syncSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    try {
        return sensor->synchronize();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}

bool initSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    try {
        sensor->init();
        return true;
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}

bool connectSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    if(sensor->getPins().empty()) {
        return false; // No pins assigned, nothing to connect
    }

    try {
        return sensor->connect();  
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}

bool disconnectSensor(BaseSensor *sensor) {
    if(sensor == nullptr) {
        return false;
    }
    sensor->clearError(); // Clear error if config successful

    if(sensor->getPins().empty()) {
        return true; // No pins assigned, nothing to disconnect
    }

    try {
        return sensor->disconnect();
    } catch (const Exception &ex) {
        ex.print();
        sensor->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        sensor->setError(msg);
        return false;
    }
}


