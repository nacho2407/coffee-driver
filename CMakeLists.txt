idf_component_register(SRCS "src/display.cpp" "src/driver.cpp" "src/sd.cpp" "src/touch.cpp"
                        INCLUDE_DIRS "src"
                        REQUIRES arduino-esp32 gt911-arduino LovyanGFX lvgl PCA9557)
