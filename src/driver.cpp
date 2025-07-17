#include "driver.hpp"

namespace coffee
{
    bool init_driver(void)
    {
        init_IO();

        if(!init_lcd()) {
            Serial.println("Error: failed to initialize LCD driver");

            return false;
        }

        if(!init_touch()) {
            Serial.println("Error: failed to initialize touch driver");

            return false;
        }

        turn_on_bl();

        if(!init_sd()) {
            Serial.println("Error: failed to initialize sd card driver");

            return false;
        }

        return true;
    }
}
