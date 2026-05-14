#include "lvgl_storage_fs.hpp"

#include "../managers/storage_manager.hpp"

#include "expt.hpp"
#include "lvgl.h"

namespace
{
constexpr char LVGL_STORAGE_DRIVE_LETTER = 'S';

struct LvglStorageFile
{
    File file;
};

bool storageReady(lv_fs_drv_t *)
{
    return storageManager().isAvailable() || storageManager().init();
}

void *storageOpen(lv_fs_drv_t *, const char *path, lv_fs_mode_t mode)
{
    if (!path) {
        return nullptr;
    }

    const bool writeMode = (mode & LV_FS_MODE_WR) != 0;
    auto *handle = new LvglStorageFile();
    handle->file = storageManager().open(path, writeMode ? FILE_WRITE : FILE_READ);
    if (!handle->file) {
        debugLogMessage("lvgl_storage_fs::storageOpen", "storage read failed", "path=%s", path);
        delete handle;
        return nullptr;
    }

    return handle;
}

lv_fs_res_t storageClose(lv_fs_drv_t *, void *file_p)
{
    auto *handle = static_cast<LvglStorageFile *>(file_p);
    if (!handle) {
        return LV_FS_RES_INV_PARAM;
    }

    handle->file.close();
    delete handle;
    return LV_FS_RES_OK;
}

lv_fs_res_t storageRead(lv_fs_drv_t *, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    auto *handle = static_cast<LvglStorageFile *>(file_p);
    if (!handle || !buf || !br) {
        return LV_FS_RES_INV_PARAM;
    }

    *br = handle->file.read(static_cast<uint8_t *>(buf), btr);
    return LV_FS_RES_OK;
}

lv_fs_res_t storageWrite(lv_fs_drv_t *, void *file_p, const void *buf, uint32_t btw, uint32_t *bw)
{
    auto *handle = static_cast<LvglStorageFile *>(file_p);
    if (!handle || !buf || !bw) {
        return LV_FS_RES_INV_PARAM;
    }

    *bw = handle->file.write(static_cast<const uint8_t *>(buf), btw);
    return *bw == btw ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

lv_fs_res_t storageSeek(lv_fs_drv_t *, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
    auto *handle = static_cast<LvglStorageFile *>(file_p);
    if (!handle) {
        return LV_FS_RES_INV_PARAM;
    }

    SeekMode mode = SeekSet;
    if (whence == LV_FS_SEEK_CUR) {
        mode = SeekCur;
    } else if (whence == LV_FS_SEEK_END) {
        mode = SeekEnd;
    }

    return handle->file.seek(pos, mode) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

lv_fs_res_t storageTell(lv_fs_drv_t *, void *file_p, uint32_t *pos_p)
{
    auto *handle = static_cast<LvglStorageFile *>(file_p);
    if (!handle || !pos_p) {
        return LV_FS_RES_INV_PARAM;
    }

    *pos_p = handle->file.position();
    return LV_FS_RES_OK;
}
}

void ensureLvglStorageFsRegistered()
{
    static bool registered = false;
    static lv_fs_drv_t driver;

    if (registered) {
        return;
    }

    lv_fs_drv_init(&driver);
    driver.letter = LVGL_STORAGE_DRIVE_LETTER;
    driver.ready_cb = storageReady;
    driver.open_cb = storageOpen;
    driver.close_cb = storageClose;
    driver.read_cb = storageRead;
    driver.write_cb = storageWrite;
    driver.seek_cb = storageSeek;
    driver.tell_cb = storageTell;
    lv_fs_drv_register(&driver);

    registered = true;
    debugLogMessage("ensureLvglStorageFsRegistered", "gui init", "drive=%c", LVGL_STORAGE_DRIVE_LETTER);
}
