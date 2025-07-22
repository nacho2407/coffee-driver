#ifndef COFFEE_SD_HPP
#define COFFEE_SD_HPP

#include <string.h>

#include <Arduino.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <lvgl.h>

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

/**
 * @def COFFEE_LIST_FILES
 * 
 * @brief SD 카드가 초기화될 때, SD 카드 내 모든 파일 목록을 출력하려면 이 값을 1로 설정합니다
 * 
 *        set this value to 1 to print a list of all files on the SD card when it is initialized
 */
#define COFFEE_LIST_FILES 0

/**
 * @def COFFEE_FS_LETTER
 * 
 * @brief LVGL 드라이버 식별 문자(대문자 알파벳)
 * 
 *        the drive letter(capitalized alphabet) used by LVGL to identify this file system
 */
#define COFFEE_FS_LETTER 'S'

namespace coffee
{
    /**
     * @brief SD 카드를 초기화합니다
     * 
     *        initializes the SD card
     * 
     * @param fs_letter LVGL 드라이버 식별 문자(대문자 알파벳)
     * 
     *                  the drive letter(capitalized alphabet) used by LVGL to identify this file system
     * 
     * @return SD 카드 초기화 성공 여부
     * 
     *         SD card initialization success
     */
    bool init_sd(char fs_letter);

    /**
     * @brief 파일 시스템 내의 모든 파일을 표시합니다
     * 
     *        lists all files in the file system
     */
    void list_all(void);

    /**
     * @brief 디렉토리 내의 모든 파일들을 표시합니다
     * 
     *        lists all files in a directory
     * 
     * @param root 읽어 들일 파일 시스템 내 디렉토리
     * 
     *             directory in the file system to read
     * 
     * @param dir_name 읽어 들일 디렉토리의 이름
     * 
     *                 directory name to read
     * 
     * @param depth 디렉토리의 깊이
     * 
     *              depth of the directory
     */
    void list_dir(File& root, const char* dir_name, uint8_t depth = 0);
}
#endif
