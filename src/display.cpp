#include "display.hpp"

namespace coffee
{
    LCD lcd;

    // 화면에 실제 그려질 픽셀 색 정보 배열
    // the array of pixel color values to be drawn
    static lv_color_t* pixels = nullptr;

    LCD::LCD(void)
    {
        {
            // 패널 기본 정보 설정
            // set panel preferences
            auto cfg = _panel.config();
            cfg.memory_width = (uint16_t) COFFEE_WIDTH;
            cfg.memory_height = (uint16_t) COFFEE_HEIGHT;
            cfg.panel_width = (uint16_t) COFFEE_WIDTH;
            cfg.panel_height = (uint16_t) COFFEE_HEIGHT;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            _panel.config(cfg);
        }

        {
            // 버스 정보 설정
            // set bus information
            auto cfg = _bus.config();
            cfg.panel = &_panel;

            cfg.pin_d0 = GPIO_NUM_15;
            cfg.pin_d1 = GPIO_NUM_7;
            cfg.pin_d2 = GPIO_NUM_6;
            cfg.pin_d3 = GPIO_NUM_5;
            cfg.pin_d4 = GPIO_NUM_4;

            cfg.pin_d5 = GPIO_NUM_9;
            cfg.pin_d6 = GPIO_NUM_46;
            cfg.pin_d7 = GPIO_NUM_3;
            cfg.pin_d8 = GPIO_NUM_8;
            cfg.pin_d9 = GPIO_NUM_16;
            cfg.pin_d10 = GPIO_NUM_1;

            cfg.pin_d11 = GPIO_NUM_14;
            cfg.pin_d12 = GPIO_NUM_21;
            cfg.pin_d13 = GPIO_NUM_47;
            cfg.pin_d14 = GPIO_NUM_48;
            cfg.pin_d15 = GPIO_NUM_45;

            cfg.pin_henable = GPIO_NUM_41;
            cfg.pin_vsync = GPIO_NUM_40;
            cfg.pin_hsync = GPIO_NUM_39;
            cfg.pin_pclk = GPIO_NUM_0;
            cfg.freq_write = 15000000;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 40;
            cfg.hsync_pulse_width = 48;
            cfg.hsync_back_porch  = 40;
            
            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 1;
            cfg.vsync_pulse_width = 31;
            cfg.vsync_back_porch  = 13;

            cfg.pclk_active_neg = 1;
            cfg.de_idle_high = 0;
            cfg.pclk_idle_high = 0;

            _bus.config(cfg);
        }
            
        _panel.setBus(&_bus);

        setPanel(&_panel);
    }

    void init_IO(void)
    {
        // IO 확장용 칩
        // chip for IO expansion
        static PCA9557 pca9557;

        // ESP32-S3의 Direction(output-only) IO 핀(IO38) 초기화
        // initializes Direction(output-only) IO pin(IO38) of ESP32-S3
        pinMode(38, OUTPUT);
        digitalWrite(38, LOW);
        
        // PCA9557 설정
        // configurates PCA9557
        Wire.begin(19, 20);

        pca9557.reset();
        pca9557.setMode(IO_OUTPUT);

        pca9557.setState(IO0, IO_LOW);
        pca9557.setState(IO1, IO_LOW);
        delay(20);

        pca9557.setState(IO0, IO_HIGH);
        delay(100);
        pca9557.setMode(IO1, IO_INPUT);
    }

    bool init_lcd(void)
    {
        // 화면에 그려질 데이터 배열
        // data buffer to be drawn to the screen
        static lv_disp_draw_buf_t draw_buf;

        // 디스플레이 관련 작업을 수행하기 위한 LVGL 드라이버 객체
        // LVGL display driver object used for display-related operations
        static lv_disp_drv_t disp_drv;

        if (!lcd.begin())
            return false;

        lcd.fillScreen(TFT_BLACK);
        lcd.setTextSize(3);
        delay(100);

        uint32_t pixel_size = (uint32_t) COFFEE_WIDTH * COFFEE_HEIGHT / COFFEE_DISP_BUF_BLOCKS;

        // 화면 버퍼는 내부 메모리 상 DMA 영역에 할당
        // the screen buffer is allocated in a DMA area on internal memory
        pixels = (lv_color_t*) heap_caps_malloc(sizeof(lv_color_t) * pixel_size, MALLOC_CAP_DMA);
        if (!pixels)
            return false;
        
        lv_init();

        lv_disp_draw_buf_init(&draw_buf, pixels, NULL, pixel_size);

        lv_disp_drv_init(&disp_drv);

        disp_drv.hor_res = lcd.width();
        disp_drv.ver_res = lcd.height();
        disp_drv.flush_cb = flush_disp;
        disp_drv.draw_buf = &draw_buf;
        
        lv_disp_drv_register(&disp_drv);

        return true;
    }

    void turn_on_bl(void)
    {
        ledcSetup(1, 300, 8);

        ledcAttachPin((uint8_t) COFFEE_BACKLIGHT, 1);

        // 두 번째 매개변수를 수정하여 화면 밝기를 조절할 수 있습니다(0-255)
        // screen brightness can be modified by adjusting second parameter(0-255)
        ledcWrite(1, 0);

        pinMode((uint8_t) COFFEE_BACKLIGHT, OUTPUT);

        digitalWrite((uint8_t) COFFEE_BACKLIGHT, LOW);
        delay(500);

        digitalWrite((uint8_t) COFFEE_BACKLIGHT, HIGH);

        lcd.fillScreen(TFT_BLACK);
    }

    void flush_disp(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* pixels)
    {
        int32_t img_w = area->x2 - area->x1 + 1;
        int32_t img_h = area->y2 - area->y1 + 1;

        lcd.pushImageDMA(area->x1, area->y1, img_w, img_h, (lgfx::rgb565_t*) &pixels->full);

        lv_disp_flush_ready(disp_drv);
    }
}
