#include <iostream>

#include "node.hpp"
#include "comms.hpp"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "LOLIN_I2C_BUTTON.h"

#ifdef ARDUINO
// dteam::real_comms comms_device = new dteam::real_comms();
auto comms_ptr = std::shared_ptr<dteam::comms>(new dteam::real_comms());
#else
dteam::fake_comms comms_device;
#endif

std::shared_ptr<dteam::node> node_ptr(new dteam::node());

#ifdef ARDUINO

void setup()
{
    Serial.begin(115200);
    node_ptr.get()->initialize(comms_ptr);
}
void loop() { comms_ptr.get()->update(); }

#else

int main(int, char **)
{
    auto comms_ptr = std::shared_ptr<dteam::comms>(&comms_device);

    std::shared_ptr<dteam::node> node_ptr(new dteam::node(comms_ptr));
    node_ptr->initialize();

    while (true)
    {
        comms_ptr->update();
    }

    return 0;
}

#endif

// i wonder if we can mirror lcd display onto website
// animations?
// pre mesh logic? 
// seperate laddstattioner with seperate mesh networks