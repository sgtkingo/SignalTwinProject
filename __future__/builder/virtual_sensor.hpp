
#include "base_device.hpp"
#include "lvgl.h"

#include <string>
#include <unordered_map>
#include <exception>


class VirtualSensor : public BaseDevice
{
private:
    // Container
    lv_obj_t *ui_Widget;
public:
    VirtualSensor(/* args */){}
    VirtualSensor(std::string uid) : BaseDevice(uid) {}
    ~VirtualSensor(){}

    void init() override {};
};
