idf_component_register(SRCS "display.cpp" "driver.cpp" "sd.cpp" "touch.cpp"
                        INCLUDE_DIRS "."
                        REQUIRES arduino-esp32 gt911-arduino LovyanGFX lvgl PCA9557)
