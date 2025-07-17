#ifndef COFFEE_TOUCH_HPP
#define COFFEE_TOUCH_HPP

#include <Arduino.h>
#include <Wire.h>

#include <TAMC_GT911.h>

#include <lvgl.h>

#include "def.h"

#define COFFEE_GT911
#define COFFEE_GT911_SCL 20
#define COFFEE_GT911_SDA 19
#define COFFEE_GT911_INT 3
#define COFFEE_GT911_RST 4
#define COFFEE_GT911_ROTATION ROTATION_NORMAL
#define COFFEE_MAP_X1 800
#define COFFEE_MAP_X2 0
#define COFFEE_MAP_Y1 480
#define COFFEE_MAP_Y2 0

/**
 * @def COFFEE_PRINT_TOUCH
 * 
 * @brief 터치 이벤트가 발생할 때마다 터치 위치를 출력하려면 이 값을 1로 설정합니다
 * 
 *        set this value to 1 to print the touch position whenever a touch event occurs
 */
#define COFFEE_PRINT_TOUCH 1

namespace coffee
{
    /**
     * @brief 마지막으로 터치된 위치의 X 좌표
     * 
     *        X coordinate of the last touched location
     */
    extern int last_x;
    
    /**
     * @brief 마지막으로 터치된 위치의 Y 좌표
     * 
     *        Y coordinate of the last touched location
     */
    extern int last_y;

    /**
     * @brief 터치 스크린을 초기화합니다
     * 
     *        initializes the touch screen
     * 
     * @return 터치 스크린 초기화 성공 여부
     * 
     *         touch screen initialization success
     */
    bool init_touch(void);

    /**
     * @brief 터치를 감지하면 마지막으로 터치된 위치를 last_x와 last_y에 저장합니다
     * 
     *        detects touch and stores the last touched position in last_x and last_y
     * 
     * @return 터치 감지 여부
     * 
     *         whether touch is detected
     */
    bool is_touched(void);

    /**
     * @brief 터치 정보를 읽고 해당 정보를 처리합니다
     * 
     *        reads the current touch state and updates the input data accordingly
     * 
     * @param indev_driver 터치 관련 작업을 처리하는 LVGL 입력 드라이버
     * 
     *                     LVGL input device driver object that manages touch input handling
     * 
     * @param indev_data 터치 관련 정보를 포함하는 LVGL 입력 데이터 구조체
     * 
     *                   the structure that will be filled with the current touch data(position and state)
     */
    void read_touch(lv_indev_drv_t* indev_driver, lv_indev_data_t* indev_data);
}
#endif
