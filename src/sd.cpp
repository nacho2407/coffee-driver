#include "sd.hpp"

namespace coffee
{
    /**
     * @brief lvgl에서 관리되는 파일 시스템을 현재 사용중인 Arduino 파일 시스템과 연동합니다
     * 
     *        Connects the lvgl-managed file system to the currently used Arduino file system
     * 
     * @param fs_letter lvgl 드라이버 식별 문자(대문자 알파벳)
     * 
     *                  the drive letter(capitalized alphabet) used by lvgl to identify this file system
     * 
     * @return lvgl 파일 시스템 초기화 성공 여부
     * 
     *         lvgl file system initialization success
     */
    static bool init_lv_fs(char fs_letter);

    /**
     * @brief lv_fs에서 파일을 열 때 콜백됩니다
     * 
     *        called by lv_fs when opening a file
     */
    static void* open_file(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode);

    /**
     * @brief lv_fs에서 파일을 닫을 때 콜백됩니다
     * 
     *        called by lv_fs when closing a file
     */
    static lv_fs_res_t close_file(lv_fs_drv_t* drv, void* file_p);

    /**
     * @brief lv_fs에서 파일을 읽을 때 콜백됩니다
     * 
     *        called by lv_fs when reading from a file
     */
    static lv_fs_res_t read_file(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);

    /**
     * @brief lv_fs에서 파일을 쓸 때 콜백됩니다
     * 
     *        called by lv_fs when writing to a file
     */
    static lv_fs_res_t write_file(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);

    /**
     * @brief lv_fs에서 파일 커서를 이동할 때 콜백됩니다
     * 
     *        called by lv_fs when seeking within a file
     */
    static lv_fs_res_t seek_file(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence);

    /**
     * @brief lv_fs에서 파일 커서의 위치를 확인할 때 콜백됩니다
     * 
     *        called by lv_fs when telling the current file position
     */
    static lv_fs_res_t tell_file(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);

    /**
     * @brief lv_fs에서 디렉토리를 열 때 콜백됩니다
     * 
     *        called by lv_fs when opening a directory
     */
    static void* open_dir(lv_fs_drv_t* drv, const char* path);

    /**
     * @brief lv_fs에서 디렉토리를 읽을 때 콜백됩니다
     * 
     *        called by lv_fs when reading a directory entry
     */
    static lv_fs_res_t read_dir(lv_fs_drv_t* drv, void* rddir_p, char* fn);

    /**
     * @brief lv_fs에서 디렉토리를 닫을 때 콜백됩니다
     * 
     *        called by lv_fs when closing a directory
     */
    static lv_fs_res_t close_dir(lv_fs_drv_t* drv, void* rddir_p);

    bool init_sd(char fs_letter)
    {
        SPI.begin(COFFEE_SD_SCK, COFFEE_SD_MISO, COFFEE_SD_MOSI, COFFEE_SD_CS);

        if(!SD.begin(COFFEE_SD_CS, SPI, COFFEE_SPI_CLK)) {
            Serial.println("error: failed to initialize SD card driver");

            return false;
        }

        if(!init_lv_fs(fs_letter))
            return false;

#if COFFEE_LIST_FILES
        list_all();
#endif

        return true;
    }

    void list_all(void)
    {
        Serial.printf("files in SD card: %c:", COFFEE_FS_LETTER);

        File root = SD.open("/");
        list_dir(root, "");
    }

    void list_dir(File& root, const char* dir_name, uint8_t depth)
    {
        if(!root || !root.isDirectory()) {
            Serial.printf("error: not a directory, or cannot be opened(%s)\n", dir_name);

            return;
        }

        Serial.printf("%*s%s/\n", depth * 4, "", dir_name);

        File file = root.openNextFile();

        while (file) {
            const char* file_name = file.name();

            if(file.isDirectory())
                list_dir(file, file_name, depth + 1);
            else
                Serial.printf("%*s%s(%dB)\n", (depth + 1) * 4, "", file_name, file.size());

            file = root.openNextFile();
        }
    }

    static bool init_lv_fs(char fs_letter)
    {
        // lvgl 파일 시스템 드라이버
        // lvgl file system driver
        static lv_fs_drv_t drv;

        if(fs_letter < 'A' || fs_letter > 'Z') {
            Serial.println("error: the file driver identification character must be an uppercase alphabet");

            return false;
        }

        lv_fs_drv_init(&drv);

        drv.letter = fs_letter;
        drv.cache_size = 0;

        drv.ready_cb = nullptr;

        drv.open_cb = open_file;
        drv.close_cb = close_file;
        drv.read_cb = read_file;
        drv.write_cb = write_file;
        drv.seek_cb = seek_file;
        drv.tell_cb = tell_file;

        drv.dir_open_cb = open_dir;
        drv.dir_read_cb = read_dir;
        drv.dir_close_cb = close_dir;

        lv_fs_drv_register(&drv);

        return true;
    }

    static void* open_file(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode)
    {
        const char* mode_str = (mode == LV_FS_MODE_WR) ? FILE_WRITE :
                            (mode == LV_FS_MODE_RD) ? FILE_READ :
                            (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) ? FILE_READ : nullptr;

        if(!mode_str)
            return nullptr;

        File* file = new File(SD.open(path, mode_str));
        if(!*file || !file->available()) {
            delete file;

            return nullptr;
        }

        return file;
    }

    static lv_fs_res_t close_file(lv_fs_drv_t* drv, void* file_p)
    {
        File* file = static_cast<File*>(file_p);

        if(file) {
            file->close();

            delete file;
        }

        return LV_FS_RES_OK;
    }

    static lv_fs_res_t read_file(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br)
    {
        File* file = static_cast<File*>(file_p);
        if(!file)
            return LV_FS_RES_INV_PARAM;

        *br = file->read(static_cast<uint8_t*>(buf), btr);

        return LV_FS_RES_OK;
    }

    static lv_fs_res_t write_file(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw)
    {
        File* file = static_cast<File*>(file_p);
        if(!file)
            return LV_FS_RES_INV_PARAM;

        *bw = file->write(static_cast<const uint8_t*>(buf), btw);

        return (*bw == btw) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
    }

    static lv_fs_res_t seek_file(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence)
    {
        File* file = static_cast<File*>(file_p);
        if(!file)
            return LV_FS_RES_INV_PARAM;

        if(whence == LV_FS_SEEK_SET)
            file->seek(pos);
        else if(whence == LV_FS_SEEK_CUR)
            file->seek(file->position() + pos);
        else if(whence == LV_FS_SEEK_END)
            file->seek(file->size() + pos);
        else
            return LV_FS_RES_INV_PARAM;

        return LV_FS_RES_OK;
    }

    static lv_fs_res_t tell_file(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p)
    {
        File* file = static_cast<File*>(file_p);
        if(!file)
            return LV_FS_RES_INV_PARAM;

        *pos_p = file->position();

        return LV_FS_RES_OK;
    }

    static void* open_dir(lv_fs_drv_t* drv, const char* path)
    {
        File dir = SD.open(path);
        if(!dir || !dir.isDirectory())
            return nullptr;

        return new File(dir);
    }

    static lv_fs_res_t read_dir(lv_fs_drv_t* drv, void* rddir_p, char* fn)
    {
        File* file = static_cast<File*>(rddir_p);
        if(!file)
            return LV_FS_RES_INV_PARAM;

        File entry = file->openNextFile();
        if(!entry) {
            fn[0] = '\0';

            return LV_FS_RES_OK;
        }

        strncpy(fn, entry.name(), LV_FS_MAX_FN_LENGTH - 1);
        fn[LV_FS_MAX_FN_LENGTH - 1] = '\0';

        entry.close();

        return LV_FS_RES_OK;
    }

    static lv_fs_res_t close_dir(lv_fs_drv_t* drv, void* rddir_p)
    {
        File* dir = static_cast<File*>(rddir_p);

        if(dir) {
            dir->close();

            delete dir;
        }
        
        return LV_FS_RES_OK;
    }
}
