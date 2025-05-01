#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr size_t CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE = 512;

class CellSpurs {
public:
    CellSpurs(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct alignas(8) CellSpursTasksetAttribute {
        BEField<u32> revision;           
        BEField<u32> sdk_version;        
        BEField<u64> args;               
        u8 priority[8];               
        BEField<u32> max_contention;     
        BEField<u32> name_ptr;         
        BEField<u32> taskset_size;       
        BEField<s32> enable_clear_ls;    
        u8 reserved[472];
    };

    struct CellSpursWorkloadAttribute {
        BEField<u32> revision;
        BEField<u32> sdk_ver;
        BEField<u32> pm_ptr;
        BEField<u32> size;
        BEField<u64> data;
        u8 prio[8];
        BEField<u32> min_contention;
        BEField<u32> max_contention;
        BEField<u32> name_class_ptr;
        BEField<u32> name_instance_ptr;
        BEField<u32> hook_ptr;
        BEField<u32> hook_arg_ptr;
        u8 padding[456];
    };
    static_assert(sizeof(CellSpursWorkloadAttribute) == CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE);

    struct CellSpursWorkloadFlag {
        BEField<u64> unused0;
        BEField<u32> unused1;
        BEField<u32> flag;
    };


    u32 spurs_thread_id = 0;

    struct Task {
        u32 handle = 0;
        u32 elf_ptr = 0;
        u32 arg_ptr = 0;
    };

    struct Taskset {
        u32 handle = 0;
        std::vector<Task> tasks;

        Task* getTask(u32 handle) {
            for (auto& i : tasks) {
                if (i.handle == handle) return &i;
            }
            return nullptr;
        }
    };

    std::vector<Taskset> tasksets;
    Taskset* getTaskset(u32 handle) {
        for (auto& i : tasksets) {
            if (i.handle == handle) return &i;
        }
        return nullptr;
    }

    u64 _cellSpursLFQueueInitialize();
    u64 cellSpursAttributeSetNamePrefix();
    u64 cellSpursAttributeEnableSpuPrintfIfAvailable();
    u64 _cellSpursTasksetAttributeInitialize();
    u64 cellSpursLFQueueAttachLv2EventQueue();
    u64 cellSpursRequestIdleSpu();
    u64 cellSpursCreateTaskWithAttribute();
    u64 cellSpursGetInfo();
    u64 cellSpursInitializeWithAttribute2();
    u64 cellSpursWorkloadAttributeSetName();
    u64 cellSpursCreateTaskset2();
    u64 cellSpursEventFlagClear();
    u64 cellSpursCreateTaskset();
    u64 _cellSpursEventFlagInitialize();
    u64 cellSpursTasksetAttributeSetName();
    u64 cellSpursAttributeSetMemoryContainerForSpuThread();
    u64 cellSpursEventFlagAttachLv2EventQueue();
    u64 _cellSpursLFQueuePushBody();
    u64 _cellSpursAttributeInitialize();
    u64 cellSpursAttributeEnableSystemWorkload();
    u64 _cellSpursWorkloadFlagReceiver();
    u64 cellSpursAttributeSetSpuThreadGroupType();
    u64 cellSpursInitializeWithAttribute();
    u64 cellSpursInitialize();
    u64 _cellSpursTaskAttributeInitialize();
    u64 cellSpursAttachLv2EventQueue();
    u64 cellSpursCreateTask();
    u64 cellSpursAddWorkloadWithAttribute();
    u64 cellSpursCreateTasksetWithAttribute();
    u64 _cellSpursTasksetAttribute2Initialize();
    u64 cellSpursGetWorkloadFlag();
    u64 cellSpursSetExceptionEventHandler();
    u64 _cellSpursWorkloadAttributeInitialize();

private:
    MAKE_LOG_FUNCTION(log, cellSpurs);
};