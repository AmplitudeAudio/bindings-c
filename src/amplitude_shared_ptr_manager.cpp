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

#include "amplitude_shared_ptr_manager.h"

// Static member definitions
std::unique_ptr<SharedPtrManager> SharedPtrManager::_instance = nullptr;
std::once_flag SharedPtrManager::_init_flag;

SharedPtrManager &SharedPtrManager::Instance() {
  std::call_once(_init_flag, []() {
    _instance = std::unique_ptr<SharedPtrManager>(new SharedPtrManager());
  });

  return *_instance;
}

bool SharedPtrManager::Contains(void *raw_ptr) const {
  if (!raw_ptr)
    return false;

  std::shared_lock lock(_mutex);
  return _storage.find(raw_ptr) != _storage.end();
}

void SharedPtrManager::Clear() {
  std::unique_lock lock(_mutex);
  _storage.clear();
}

size_t SharedPtrManager::GetStoredCount() const {
  std::shared_lock lock(_mutex);
  return _storage.size();
}
