// Copyright (c) 2025-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "amplitude_internals.h"

#include <amplitude_memory.h>

class CMemoryAllocator : public MemoryAllocator
{
public:
    CMemoryAllocator(const am_memory_allocator_vtable* v_table)
        : _v_table(v_table)
    {
    }

    ~CMemoryAllocator() = default;

    AmVoidPtr Malloc(eMemoryPoolKind pool, AmSize size) override
    {
        return _v_table->malloc(static_cast<am_memory_pool_kind>(pool), size);
    }

    AmVoidPtr Realloc(eMemoryPoolKind pool, AmVoidPtr ptr, AmSize size) override
    {
        return _v_table->realloc(static_cast<am_memory_pool_kind>(pool), ptr, size);
    }

    AmVoidPtr Malign(eMemoryPoolKind pool, AmSize size, AmUInt32 alignment) override
    {
        return _v_table->malign(static_cast<am_memory_pool_kind>(pool), size, alignment);
    }

    AmVoidPtr Realign(eMemoryPoolKind pool, AmVoidPtr ptr, AmSize size, AmUInt32 alignment) override
    {
        return _v_table->realign(static_cast<am_memory_pool_kind>(pool), ptr, size, alignment);
    }

    void Free(eMemoryPoolKind pool, AmVoidPtr ptr) override
    {
        _v_table->free(static_cast<am_memory_pool_kind>(pool), ptr);
    }

    AmSize SizeOf(eMemoryPoolKind pool, AmVoidPtr ptr) override
    {
        return _v_table->size_of(static_cast<am_memory_pool_kind>(pool), ptr);
    }

private:
    const am_memory_allocator_vtable* _v_table;
};

extern "C" {
am_memory_allocator_vtable am_memory_allocator_vtable_init()
{
    return { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
}

void am_memory_manager_initialize(const am_memory_allocator_vtable* config)
{
    MemoryManager::Initialize(std::make_unique<CMemoryAllocator>(config));
}

void am_memory_manager_deinitialize()
{
    MemoryManager::Deinitialize();
}

 am_bool am_memory_manager_is_initialized()
{
    return BOOL_TO_AM_BOOL(MemoryManager::IsInitialized());
}

 am_voidptr am_memory_manager_malloc(am_memory_pool_kind pool, am_size size, const char* file, am_uint32 line)
{
    return amMemory->Malloc(static_cast<eMemoryPoolKind>(pool), size, file, line);
}

 am_voidptr
am_memory_manager_malign(am_memory_pool_kind pool, am_size size, am_uint32 alignment, const char* file, am_uint32 line)
{
    return amMemory->Malign(static_cast<eMemoryPoolKind>(pool), size, alignment, file, line);
}

 am_voidptr
am_memory_manager_realloc(am_memory_pool_kind pool, am_voidptr address, am_size size, const char* file, am_uint32 line)
{
    return amMemory->Realloc(static_cast<eMemoryPoolKind>(pool), address, size, file, line);
}

 am_voidptr
am_memory_manager_realign(am_memory_pool_kind pool, am_voidptr address, am_size size, am_uint32 alignment, const char* file, am_uint32 line)
{
    return amMemory->Realign(static_cast<eMemoryPoolKind>(pool), address, size, alignment, file, line);
}

 void am_memory_manager_free(am_memory_pool_kind pool, am_voidptr address)
{
    amMemory->Free(static_cast<eMemoryPoolKind>(pool), address);
}

am_size am_memory_manager_total_reserved_memory_size()
{
    return amMemory->TotalReservedMemorySize();
}

am_size am_memory_manager_size_of(am_memory_pool_kind pool, const am_voidptr address)
{
    return amMemory->SizeOf(static_cast<eMemoryPoolKind>(pool), address);
}

void am_memory_free_str(const char* str)
{
    am_free_string(str);
}

#ifndef AM_NO_MEMORY_STATS
const char* am_memory_manager_get_memory_pool_name(am_memory_pool_kind pool)
{
    return am_allocate_string(MemoryManager::GetMemoryPoolName(static_cast<eMemoryPoolKind>(pool)).c_str());
}

am_memory_pool_stats_handle am_memory_manager_get_stats(am_memory_pool_kind pool)
{
    return nullptr;
}

const char* am_memory_manager_inspect_memory_leaks()
{
    return am_allocate_string(amMemory->InspectMemoryLeaks().c_str());
}
#endif
}
