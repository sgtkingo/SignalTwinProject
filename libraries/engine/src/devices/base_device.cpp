/**
 * @file base_device.cpp
 * @brief Implementation of generic device management functions.
 * 
 * This file implements device helper functions, including the factory function for creating
 * device instances. The factory function dynamically allocates a device, calls its
 * initialization method, and returns a pointer. If initialization fails, it logs the error,
 * cleans up, and rethrows the exception.
 * 
 * @copyright 2024 MTA
 * @author Ing. Jiri Konecny
 * 
 */

#include "base_device.hpp"

/*Global functions*/


/*Create functions*/
//implemented in header file as generic function

/*General functions*/

bool configDevice(BaseDevice *device, const std::string &config) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    try {
        std::unordered_map<std::string, std::string> params = parseParamsFromString(config, CASE_SENSITIVE);
        device->config(params);
        return true;
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}


bool updateDevice(BaseDevice *device, const std::string &update) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    try {
        std::unordered_map<std::string, std::string> params = parseParamsFromString(update, CASE_SENSITIVE);
        device->update(params);
        return true;   
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}

bool controlDevice(BaseDevice *device, const std::string &control) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    try {
        std::unordered_map<std::string, std::string> params = parseParamsFromString(control, CASE_SENSITIVE);
        device->control(params);
        return true;
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during device control: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during device control!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}

void printDevice(BaseDevice *device) {
    if(device == nullptr) {
        return;
    }
    device->clearError();
    
    try {
        device->print();
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return;
    }
}

bool syncDevice(BaseDevice *device) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    try {
        return device->synchronize();
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}

bool initDevice(BaseDevice *device) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    try {
        device->init();
        return true;
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}

bool connectDevice(BaseDevice *device) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    if(device->getPins().empty()) {
        return false; // No pins assigned, nothing to connect
    }

    try {
        return device->connect();  
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}

bool disconnectDevice(BaseDevice *device) {
    if(device == nullptr) {
        return false;
    }
    device->clearError();

    if(device->getPins().empty()) {
        return true; // No pins assigned, nothing to disconnect
    }

    try {
        return device->disconnect();
    } catch (const Exception &ex) {
        ex.print();
        device->setError(ex.flush(0));
        return false;
    }
    catch (const std::exception &e)
    {
        std::string msg = buildMessage("Standard exception during synchronization: %s\n", e.what());
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
    catch(...)
    {
        std::string msg = "Unknown exception during synchronization!\n";
        logMessage("%s", msg.c_str());
        device->setError(msg);
        return false;
    }
}


