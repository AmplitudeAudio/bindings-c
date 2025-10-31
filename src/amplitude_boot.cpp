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

#include <atomic>

#include <amplitude_boot.h>

#include "amplitude_internals.h"
#include "amplitude_shared_ptr_manager.h"

// Global initialization state
static std::atomic<bool> g_initialized{false};

extern "C" {

void am_boot() {
  // Use compare-and-swap to ensure thread-safe initialization
  bool expected = false;
  if (!g_initialized.compare_exchange_strong(expected, true)) {
    // Already initialized
    return;
  }

  // Initialize SharedPtrManager (singleton auto-initializes on first access)
  SharedPtrManager::Instance();
}

void am_shutdown() {
  // Use compare-and-swap to ensure thread-safe shutdown
  bool expected = true;
  if (!g_initialized.compare_exchange_strong(expected, false)) {
    // Not initialized or already shut down
    return;
  }

  // Clear all shared pointers managed by the system
  SharedPtrManager::Instance().Clear();
}

am_bool am_is_initialized() { return BOOL_TO_AM_BOOL(g_initialized.load()); }

} // extern "C"
