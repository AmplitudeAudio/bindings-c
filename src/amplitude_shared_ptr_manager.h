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

#pragma once

#ifndef _AM_SHARED_PTR_MANAGER_H
#define _AM_SHARED_PTR_MANAGER_H

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <typeindex>
#include <unordered_map>

/**
 * @brief Manages shared_ptr objects across the C API boundary.
 *
 * This singleton class provides type-safe storage and retrieval of shared_ptr objects,
 * allowing C API functions to work with C++ shared_ptr objects while maintaining
 * proper reference counting and memory safety.
 */
class SharedPtrManager
{
private:
    /**
     * @brief Type-erased storage entry for shared_ptr objects.
     */
    struct SharedPtrEntry
    {
        std::shared_ptr<void> ptr;
        std::type_index type;

        template<typename T> SharedPtrEntry(std::shared_ptr<T> p) : ptr(std::static_pointer_cast<void>(p)), type(std::type_index(typeid(T)))
        {}
    };

    /**
     * @brief Storage for all shared_ptr objects.
     */
    std::unordered_map<void*, SharedPtrEntry> _storage;

    /**
     * @brief Mutex for thread-safe access to storage.
     */
    mutable std::shared_mutex _mutex;

    /**
     * @brief Singleton instance.
     */
    static std::unique_ptr<SharedPtrManager> _instance;

    /**
     * @brief Initialization flag for thread-safe singleton creation.
     */
    static std::once_flag _init_flag;

    /**
     * @brief Private constructor for singleton pattern.
     */
    SharedPtrManager() = default;

public:
    /**
     * @brief Deleted copy constructor.
     */
    SharedPtrManager(const SharedPtrManager&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    SharedPtrManager& operator = (const SharedPtrManager&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    SharedPtrManager(SharedPtrManager&&) = delete;

    /**
     * @brief Deleted move assignment operator.
     */
    SharedPtrManager& operator = (SharedPtrManager&&) = delete;

    /**
     * @brief Destructor.
     */
    ~SharedPtrManager() = default;

    /**
     * @brief Get the singleton instance.
     *
     * @return Reference to the SharedPtrManager instance.
     */
    static SharedPtrManager& Instance();

    /**
     * @brief Store a shared_ptr and return a raw pointer handle.
     *
     * @tparam T The type of object stored in the shared_ptr.
     * @param[in] ptr The shared_ptr to store.
     *
     * @return Raw pointer handle, or nullptr if the shared_ptr is null.
     */
    template<typename T> T* Store(std::shared_ptr<T> ptr);

    /**
     * @brief Retrieve a shared_ptr from a raw pointer handle.
     *
     * @tparam T The type of object to retrieve.
     * @param[in] raw_ptr The raw pointer handle.
     *
     * @return The shared_ptr, or nullptr if not found or type mismatch.
     */
    template<typename T> std::shared_ptr<T> Get(T * raw_ptr);

    /**
     * @brief Remove a shared_ptr from storage.
     *
     * @tparam T The type of object to remove.
     * @param[in] raw_ptr The raw pointer handle.
     *
     * @return True if successfully removed, false if not found.
     */
    template<typename T> bool Remove(T * raw_ptr);

    /**
     * @brief Check if a handle exists in storage.
     *
     * @param[in] raw_ptr The raw pointer handle.
     *
     * @return True if the handle exists, false otherwise.
     */
    bool Contains(void* raw_ptr) const;

    /**
     * @brief Clear all stored shared_ptr objects.
     *
     * This should typically only be called during shutdown.
     */
    void Clear();

    /**
     * @brief Get the number of currently stored shared_ptr objects.
     *
     * @return The count of stored objects.
     */
    size_t GetStoredCount() const;

    /**
     * @brief Check if a handle has the expected type.
     *
     * @tparam T The expected type.
     * @param[in] raw_ptr The raw pointer handle.
     *
     * @return True if the handle exists and has the correct type.
     */
    template<typename T> bool HasType(T * raw_ptr) const;
};

// Template implementations

template<typename T> T*
SharedPtrManager::Store(std::shared_ptr<T> ptr)
{
    if (!ptr)
        return nullptr;

    T* raw_ptr = ptr.get();
    std::unique_lock lock(_mutex);

    // Check if already exists
    auto it = _storage.find(raw_ptr);
    if (it != _storage.end())
    {
        // Verify type matches
        if (it->second.type == std::type_index(typeid(T)))
        {
            return raw_ptr; // Already stored with correct type
        }
        else
        {
            // Type mismatch - this is an error condition
            return nullptr;
        }
    }

    _storage.emplace(raw_ptr, SharedPtrEntry(ptr));
    return raw_ptr;
}

template<typename T> std::shared_ptr<T>
SharedPtrManager::Get(T* raw_ptr)
{
    if (!raw_ptr)
        return nullptr;

    std::shared_lock lock(_mutex);
    auto it = _storage.find(raw_ptr);

    if (it != _storage.end() && it->second.type == std::type_index(typeid(T)))
    {
        return std::static_pointer_cast<T>(it->second.ptr);
    }

    return nullptr;
}

template<typename T> bool
SharedPtrManager::Remove(T* raw_ptr)
{
    if (!raw_ptr)
        return false;

    std::unique_lock lock(_mutex);
    auto it = _storage.find(raw_ptr);

    if (it != _storage.end() && it->second.type == std::type_index(typeid(T)))
    {
        _storage.erase(it);
        return true;
    }

    return false;
}

template<typename T> bool
SharedPtrManager::HasType(T* raw_ptr) const
{
    if (!raw_ptr)
        return false;

    std::shared_lock lock(_mutex);
    auto it = _storage.find(raw_ptr);

    return (it != _storage.end() && it->second.type == std::type_index(typeid(T)));
}

#endif // _AM_SHARED_PTR_MANAGER_H
