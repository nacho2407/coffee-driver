#ifndef COFFEE_DRIVER_HPP
#define COFFEE_DRIVER_HPP

#include "def.h"
#include "display.hpp"
#include "sd.hpp"
#include "touch.hpp"

namespace coffee
{
    /**
     * @brief 디스플레이 운용을 위한 각종 드라이버를 초기화합니다
     * 
     *        initializes various drivers for the display operation
     * 
     * @return 초기화 성공 여부
     * 
     *         initialization success
     */
    bool init_driver(void);
}
#endif