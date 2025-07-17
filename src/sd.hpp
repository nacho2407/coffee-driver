#ifndef COFFEE_SD_HPP
#define COFFEE_SD_HPP

#include <Arduino.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include "def.h"

#define COFFEE_SD_CS 10
#define COFFEE_SD_MOSI 11
#define COFFEE_SD_SCK 12
#define COFFEE_SD_MISO 13

/**
 * @def COFFEE_SPI_CLK
 * 
 * @brief ESP32는 SPI 클록을 최대 80MHz까지 지원하지만, 불안정할 수 있습니다
 * 
 *        만약 SD 카드 읽기 / 쓰기가 불안정하다면, 이 값을 40000000으로 바꾸세요
 * 
 *        ESP32 supports SPI clock up to 80MHz, but it can be unstable
 * 
 *        if SD card read / write is unstable, change this value to 40000000
 */
#define COFFEE_SPI_CLK 80000000

namespace coffee
{
    /**
     * @brief SD 카드를 초기화합니다
     * 
     *        initializes the SD card
     * 
     * @return SD 카드 초기화 성공 여부
     * 
     *         SD card initialization success
     */
    bool init_sd(void);

    /**
     * @brief 디렉토리 내의 모든 파일들을 표시합니다
     * 
     *        lists all files in a directory
     * 
     * @param fs 읽어 들일 파일 시스템
     * 
     *           file system to read
     * 
     * @param dir 읽어 들일 파일 시스템 내 디렉토리
     * 
     *            directory in the file system to read
     * 
     * @param depth 디렉토리의 깊이
     * 
     *              depth of the directory
     */
    void list_dir(fs::FS& fs, const char* dir, uint8_t depth);

    /**
     * @brief 파일 시스템 내의 모든 파일을 표시합니다
     * 
     *        lists all files in the file system
     * 
     * @param fs 읽어 들일 파일 시스템
     * 
     *           file system to read
     */
    void list_all(fs::FS& fs);
}
#endif
