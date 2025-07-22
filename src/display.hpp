#ifndef COFFEE_DISPLAY_HPP
#define COFFEE_DISPLAY_HPP

#include <driver/i2c.h>
#include <esp_heap_caps.h>

#include <Arduino.h>
#include <Wire.h>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

#include <lvgl.h>

#include <PCA9557.h>

#include "def.h"

/**
 * @def COFFEE_DISP_BUF_BLOCKS
 * 
 * @brief lvgl은 디스플레이에 표시할 이미지를 버퍼 크기로 나누어서 표현합니다
 * 
 *        때문에 이 버퍼 블록 크기가 적을 수록 더 부드러운 표현이 가능하지만, 메모리가 부족할 수 있습니다
 * 
 *        실행 중 화면이 흔들리거나 깜빡거리는 현상이 발생하면 이 값을 더 크게 조정하세요
 * 
 *        lvgl represents the image to be shown on the display by dividing it by the buffer size
 * 
 *        so having fewer of these blocks to determine the size of the buffer allows for a smoother representation, but may run out of memory
 * 
 *        if you experience screen shaking or flickering while running, adjust this value to a larger value
 */
#define COFFEE_DISP_BUF_BLOCKS 8

#define COFFEE_BACKLIGHT 2

/**
 * @def COFFEE_BRIGHTNESS
 * 
 * @brief 이 값을 수정하여 화면 밝기를 조절할 수 있습니다(0-255)
 * 
 *        screen brightness can be modified by adjusting this value(0-255)
 */
#define COFFEE_BRIGHTNESS 255

namespace coffee
{
    class LCD: public lgfx::LGFX_Device
    {
    public:
        lgfx::Panel_RGB _panel;

        lgfx::Bus_RGB _bus;

        LCD(void);
    };

    /**
     * @brief 디스플레이 출력과 관련된 객체
     * 
     *        object related to display output
     */
    extern LCD lcd;

    /**
     * @brief 디스플레이 운용에 필요한 각종 IO 핀들을 초기화합니다
     * 
     *        initializes the various IO pins required for display operation
     * 
     * @return IO 핀 초기화 성공 여부
     * 
     *         IO pin initialization success
     */
    bool init_IO(void);

    /**
     * @brief LCD를 초기화합니다
     * 
     *        initializes the LCD
     * 
     * @return LCD 초기화 성공 여부
     * 
     *         LCD initialization success
     */
    bool init_lcd(void);
}
#endif
