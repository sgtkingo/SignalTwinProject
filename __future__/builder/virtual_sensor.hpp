
#include "base_sensor.hpp"
#include "lvgl.h"

#include <string>
#include <unordered_map>
#include <exception>


class VirtualSensor : public BaseSensor
{
private:
    // Container
    lv_obj_t *ui_Widget;
public:
    VirtualSensor(/* args */){}
    VirtualSensor(std::string uid) : BaseSensor(uid) {}
    ~VirtualSensor(){}

    void init() override {};
};
