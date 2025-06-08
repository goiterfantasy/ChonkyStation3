#include "SysLwMutex.hpp"
#include "PlayStation3.hpp"


u64 SysLwMutex::sysLwMutexLock() {
    const u32 ptr = ARG0;
    const u64 timeout = ARG1;
    //log("sysLwMutexLock(ptr: 0x%08x, timeout: %lld)\n", ptr, timeout);

    LwMutex* mtx = (LwMutex*)ps3->mem.getPtr(ptr);
    if (!mtx->sleep_queue) {
        log("WARNING: TRIED TO LOCK INVALID LWMUTEX!!!\n"); // Arkedo relies on this
        return CELL_EINVAL;
    }

    Lv2Mutex* lv2_mtx = ps3->lv2_obj.get<Lv2Mutex>(mtx->sleep_queue);
    lv2_mtx->lock();

    return CELL_OK;
}

u64 SysLwMutex::sysLwMutexTryLock() {
    const u32 ptr = ARG0;
    //log("sysLwMutexTryLock(ptr: 0x%08x)\n", ptr);

    LwMutex* mtx = (LwMutex*)ps3->mem.getPtr(ptr);
    if (!mtx->sleep_queue) {
        log("WARNING: TRIED TO LOCK INVALID LWMUTEX!!!\n");
        return CELL_EINVAL;
    }
    Lv2Mutex* lv2_mtx = ps3->lv2_obj.get<Lv2Mutex>(mtx->sleep_queue);
    if(!lv2_mtx->tryLock()) return CELL_EBUSY;

    return CELL_OK;
}

u64 SysLwMutex::sysLwMutexUnlock() {
    const u32 ptr = ARG0;
    //log("sysLwMutexUnlock(ptr: 0x%08llx)\n", ptr);

    LwMutex* mtx = (LwMutex*)ps3->mem.getPtr(ptr);
    if (!mtx->sleep_queue) {
        log("WARNING: TRIED TO UNLOCK INVALID LWMUTEX!!!\n");
        return CELL_EINVAL;
    }

    Lv2Mutex* lv2_mtx = ps3->lv2_obj.get<Lv2Mutex>(mtx->sleep_queue);
    lv2_mtx->unlock();

    return CELL_OK;
}

u64 SysLwMutex::sysLwMutexCreate() {
    const u32 ptr = ARG0;
    const u64 attrib_ptr = ARG1;
    LwMutexAttrib* attrib = (LwMutexAttrib*)ps3->mem.getPtr(attrib_ptr);
    auto name = Helpers::readString(attrib->name);
    log("sysLwMutexCreate(ptr: 0x%08x, attrib_ptr: 0x%08llx (protocol: 0x%08x, recursive: 0x%08x, name: \"%s\")\n", ptr, attrib_ptr, (u32)attrib->protocol, (u32)attrib->recursive, name.c_str());

    Lv2Mutex* lv2_mtx = ps3->lv2_obj.create<Lv2Mutex>();
    lv2_mtx->owner = -1;    // free
    if (attrib->recursive == SYS_SYNC_RECURSIVE) lv2_mtx->recursive = true;
    else if (attrib->recursive == SYS_SYNC_NOT_RECURSIVE) lv2_mtx->recursive = false;
    else Helpers::panic("sysLwMutexCreate: invalid attr->recursive\n");
    
    LwMutex* mtx = (LwMutex*)ps3->mem.getPtr(ptr);
    mtx->owner = -1;    // free
    mtx->waiter = 0;
    mtx->attribute = attrib->protocol | attrib->recursive;
    mtx->recursive_count = 0;
    mtx->sleep_queue = lv2_mtx->handle();   // The real CellOS would store actual information here - we can just store our mutex id instead (this information is hidden from games anyway)

    return CELL_OK;
}

u64 SysLwMutex::sysLwMutexDestroy() {
    const u32 ptr = ARG0;
    log("sysLwMutexDestroy(ptr: 0x%08x)\n", ptr);

    return CELL_OK;
}
