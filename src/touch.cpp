#include "touch.hpp"

namespace coffee
{
    int last_x = 0;

    int last_y = 0;
    
    // 터치 제어를 위한 GT911
    // GT911 for touch control
    static TAMC_GT911 touch = TAMC_GT911(COFFEE_GT911_SDA, COFFEE_GT911_SCL, COFFEE_GT911_INT, COFFEE_GT911_RST, max(COFFEE_MAP_X1, COFFEE_MAP_X2), max(COFFEE_MAP_Y1, COFFEE_MAP_Y2));

    bool init_touch(void)
    {
        // 입력 관련 LVGL 객체
        // input-related LVGL object
        static lv_indev_drv_t indev_drv;
        
        if(!Wire.begin(COFFEE_GT911_SDA, COFFEE_GT911_SCL))
            return false;

        touch.begin();

        touch.setRotation(COFFEE_GT911_ROTATION);

        lv_indev_drv_init(&indev_drv);
        
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = read_touch;

        lv_indev_drv_register(&indev_drv);

        return true;
    }

    bool is_touched(void)
    {
        touch.read();

        if (touch.isTouched) {
            last_x = map(touch.points[0].x, (long) COFFEE_MAP_X1, (long) COFFEE_MAP_X2, 0, (long) COFFEE_WIDTH - 1);
            last_y = map(touch.points[0].y, (long) COFFEE_MAP_Y1, (long) COFFEE_MAP_Y2, 0, (long) COFFEE_HEIGHT - 1);

            return true;
        } else
            return false;
    }

    void read_touch(lv_indev_drv_t* indev_driver, lv_indev_data_t* indev_data)
    {
        if (is_touched()) {
            indev_data->state = LV_INDEV_STATE_PR;

            indev_data->point.x = last_x;
            indev_data->point.y = last_y;

#if COFFEE_PRINT_TOUCH
            Serial.print("touch X: ");
            Serial.print(indev_data->point.x);

            Serial.print(", touch Y: ");
            Serial.println(indev_data->point.y);
#endif
        } else
            indev_data->state = LV_INDEV_STATE_REL;
        
        delay(15);
    }
}
