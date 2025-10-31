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

#include <SparkyStudios/Audio/Amplitude/Amplitude.h>

#include <amplitude_file.h>
#include <amplitude_filesystem.h>

#include "amplitude_internals.h"

class CFile final : public SparkyStudios::Audio::Amplitude::File {
public:
  explicit CFile(am_file_vtable *v_table, am_voidptr user_data = nullptr)
      : File(), _v_table(v_table), _user_data(user_data) {
    if (_v_table->create)
      _v_table->create(_user_data);
  }

  ~CFile() override {
    if (_v_table->destroy)
      _v_table->destroy(_user_data);

    _v_table = nullptr;
    _user_data = nullptr;
  }

  [[nodiscard]] AmOsString GetPath() const override {
    return _v_table->get_path(_user_data);
  }

  [[nodiscard]] bool Eof() const override {
    return AM_BOOL_TO_BOOL(_v_table->eof(_user_data));
  }

  AmSize Read(AmUInt8Buffer dst, AmSize bytes) const override {
    return _v_table->read(_user_data, dst, bytes);
  }

  AmSize Write(AmConstUInt8Buffer src, AmSize bytes) override {
    return _v_table->write(_user_data, src, bytes);
  }

  [[nodiscard]] AmSize Length() const override {
    return _v_table->length(_user_data);
  }

  void Seek(AmInt64 offset, eFileSeekOrigin origin) override {
    _v_table->seek(_user_data, offset,
                   static_cast<am_file_seek_origin>(origin));
  }

  [[nodiscard]] AmSize Position() const override {
    return _v_table->position(_user_data);
  }

  [[nodiscard]] AmVoidPtr GetPtr() const override {
    return _v_table->get_ptr(_user_data);
  }

  [[nodiscard]] bool IsValid() const override {
    return AM_BOOL_TO_BOOL(_v_table->is_valid(_user_data));
  }

  void Close() override { _v_table->close(_user_data); }

private:
  am_file_vtable *_v_table;
  am_voidptr _user_data;
};

class CFileSystem final : public FileSystem {
public:
  explicit CFileSystem(am_filesystem_vtable *v_table,
                       am_voidptr user_data = nullptr)
      : FileSystem(), _v_table(v_table), _user_data(user_data) {
    if (_v_table->create)
      _v_table->create(_user_data);
  }

  ~CFileSystem() override {
    if (_v_table->destroy)
      _v_table->destroy(_user_data);

    _v_table = nullptr;
    _user_data = nullptr;
  }

  void SetBasePath(const AmOsString &basePath) override {
    _v_table->set_base_path(_user_data, basePath.c_str());
  }

  const AmOsString &GetBasePath() const override {
    _base_path_cache = _v_table->get_base_path(_user_data);
    return _base_path_cache;
  }

  [[nodiscard]] AmOsString ResolvePath(const AmOsString &path) const override {
    return _v_table->resolve_path(_user_data, path.c_str());
  }

  [[nodiscard]] bool Exists(const AmOsString &path) const override {
    return AM_BOOL_TO_BOOL(_v_table->exists(_user_data, path.c_str()));
  }

  [[nodiscard]] bool IsDirectory(const AmOsString &path) const override {
    return AM_BOOL_TO_BOOL(_v_table->is_directory(_user_data, path.c_str()));
  }

  [[nodiscard]] AmOsString
  Join(const std::vector<AmOsString> &parts) const override {
    const auto **c_parts = static_cast<const am_oschar **>(
        ammalloc(sizeof(am_oschar *) * parts.size()));
    for (size_t i = 0; i < parts.size(); i++)
      c_parts[i] = parts[i].c_str();

    const auto result = _v_table->join(_user_data, c_parts, parts.size());
    amfree(c_parts);

    return result;
  }

  [[nodiscard]] std::shared_ptr<File>
  OpenFile(const AmOsString &path, eFileOpenMode mode) const override {
    const auto file = _v_table->open_file(_user_data, path.c_str(),
                                          static_cast<am_file_open_mode>(mode));

    if (file.type == am_file_type_custom)
      return std::shared_ptr<CFile>(static_cast<CFile *>(file.handle));

    if (file.type == am_file_type_disk)
      return std::shared_ptr<DiskFile>(static_cast<DiskFile *>(file.handle));

    if (file.type == am_file_type_memory)
      return std::shared_ptr<MemoryFile>(
          static_cast<MemoryFile *>(file.handle));

    if (file.type == am_file_type_package_item)
      return std::shared_ptr<PackageItemFile>(
          static_cast<PackageItemFile *>(file.handle));

#if AM_PLATFORM_ANDROID
    if (file.type == am_file_type_android_asset)
      return std::shared_ptr<AssetManagerFile>(
          static_cast<AssetManagerFile *>(file.handle));
#elif AM_PLATFORM_IOS
    if (file.type == am_file_type_nsfile)
      return std::shared_ptr<NSFile>(static_cast<NSFile *>(file.handle));
#endif

    return nullptr;
  }

  void StartOpenFileSystem() override {
    _v_table->start_open_filesystem(_user_data);
  }

  bool TryFinalizeOpenFileSystem() override {
    return AM_BOOL_TO_BOOL(_v_table->try_finalize_open_filesystem(_user_data));
  }

  void StartCloseFileSystem() override {
    _v_table->start_close_filesystem(_user_data);
  }

  bool TryFinalizeCloseFileSystem() override {
    return AM_BOOL_TO_BOOL(_v_table->try_finalize_close_filesystem(_user_data));
  }

private:
  mutable AmOsString _base_path_cache;
  am_filesystem_vtable *_v_table;
  am_voidptr _user_data;
};

#ifdef __cplusplus
extern "C" {
#endif

am_file_config am_file_config_init_custom() {
  return {am_file_type_custom, nullptr, nullptr};
}

am_file_config am_file_config_init_disk() {
  return {am_file_type_disk, nullptr, nullptr};
}

am_file_config am_file_config_init_memory() {
  return {am_file_type_memory, nullptr, nullptr};
}

am_file_handle am_file_create(const am_file_config *config) {
  if (config->type == am_file_type_custom)
    return {am_file_type_custom, ampoolnew(eMemoryPoolKind_IO, CFile,
                                           config->v_table, config->user_data)};

  if (config->type == am_file_type_disk)
    return {am_file_type_disk, ampoolnew(eMemoryPoolKind_IO, DiskFile)};

  if (config->type == am_file_type_memory)
    return {am_file_type_memory, ampoolnew(eMemoryPoolKind_IO, MemoryFile)};

  return {am_file_type_unknown, nullptr};
}

void am_file_destroy(am_file_handle handle) {
  if (handle.type == am_file_type_custom) {
    ampooldelete(eMemoryPoolKind_IO, CFile,
                 static_cast<CFile *>(handle.handle));
  } else if (handle.type == am_file_type_disk) {
    ampooldelete(eMemoryPoolKind_IO, DiskFile,
                 static_cast<DiskFile *>(handle.handle));
  } else if (handle.type == am_file_type_memory) {
    ampooldelete(eMemoryPoolKind_IO, MemoryFile,
                 static_cast<MemoryFile *>(handle.handle));
  }
}

const am_oschar *am_file_get_path(am_file_handle handle) {
  return am_allocate_osstring(static_cast<File *>(handle.handle)->GetPath());
}

am_uint8 am_file_read8(am_file_handle handle) {
  return static_cast<File *>(handle.handle)->Read8();
}

am_uint16 am_file_read16(am_file_handle handle) {
  return static_cast<File *>(handle.handle)->Read16();
}

am_uint32 am_file_read32(am_file_handle handle) {
  return static_cast<File *>(handle.handle)->Read32();
}

am_uint64 am_file_read64(am_file_handle handle) {
  return static_cast<File *>(handle.handle)->Read64();
}

const char *am_file_read_string(am_file_handle handle) {
  return am_allocate_string(static_cast<File *>(handle.handle)->ReadString());
}

am_size am_file_write8(am_file_handle handle, am_uint8 value) {
  return static_cast<File *>(handle.handle)->Write8(value);
}

am_size am_file_write16(am_file_handle handle, am_uint16 value) {
  return static_cast<File *>(handle.handle)->Write16(value);
}

am_size am_file_write32(am_file_handle handle, am_uint32 value) {
  return static_cast<File *>(handle.handle)->Write32(value);
}

am_size am_file_write64(am_file_handle handle, am_uint64 value) {
  return static_cast<File *>(handle.handle)->Write64(value);
}

am_size am_file_write_string(am_file_handle handle, const char *str) {
  return static_cast<File *>(handle.handle)->WriteString(str);
}

am_bool am_file_eof(am_file_handle file) {
  return BOOL_TO_AM_BOOL(static_cast<File *>(file.handle)->Eof());
}

am_size am_file_read(am_file_handle file, am_uint8 *dst, am_size bytes) {
  return static_cast<File *>(file.handle)->Read(dst, bytes);
}

am_size am_file_write(am_file_handle file, const am_uint8 *buffer,
                      am_size bytes) {
  return static_cast<File *>(file.handle)->Write(buffer, bytes);
}

am_size am_file_length(am_file_handle file) {
  return static_cast<File *>(file.handle)->Length();
}

void am_file_seek(am_file_handle file, am_size offset,
                  am_file_seek_origin origin) {
  static_cast<File *>(file.handle)
      ->Seek(offset, static_cast<eFileSeekOrigin>(origin));
}

am_size am_file_position(am_file_handle file) {
  return static_cast<File *>(file.handle)->Position();
}

am_voidptr am_file_get_ptr(am_file_handle file) {
  return static_cast<File *>(file.handle)->GetPtr();
}

am_bool am_file_is_valid(am_file_handle handle) {
  return BOOL_TO_AM_BOOL(static_cast<File *>(handle.handle)->IsValid());
}

void am_file_close(am_file_handle file) {
  auto file_ptr = GET_SHARED_PTR(File, file.handle);

  if (file_ptr) {
    file_ptr->Close();
    REMOVE_SHARED_PTR(File, file.handle);
  }
}

am_filesystem_config am_filesystem_config_init_custom() {
  return {am_filesystem_type_custom, nullptr, nullptr};
}

am_filesystem_config am_filesystem_config_init_disk() {
  return {am_filesystem_type_disk, nullptr, nullptr};
}

am_filesystem_config am_filesystem_config_init_package() {
  return {am_filesystem_type_package, nullptr, nullptr};
}

#if AM_PLATFORM_ANDROID
am_filesystem_config am_filesystem_config_init_android() {
  return {am_filesystem_type_android, nullptr, nullptr};
}
#elif AM_PLATFORM_IOS
am_filesystem_config am_filesystem_config_init_ios() {
  return {am_filesystem_type_ios, nullptr, nullptr};
}
#endif

am_filesystem_handle am_filesystem_create(const am_filesystem_config *config) {
  if (config->type == am_filesystem_type_custom)
    return {am_filesystem_type_custom,
            ampoolnew(eMemoryPoolKind_IO, CFileSystem, config->v_table,
                      config->user_data)};

  if (config->type == am_filesystem_type_disk)
    return {am_filesystem_type_disk,
            ampoolnew(eMemoryPoolKind_IO, DiskFileSystem)};

  if (config->type == am_filesystem_type_package)
    return {am_filesystem_type_package,
            ampoolnew(eMemoryPoolKind_IO, PackageFileSystem)};

#if AM_PLATFORM_ANDROID
  if (config->type == am_filesystem_type_android)
    return {am_filesystem_type_android,
            ampoolnew(eMemoryPoolKind_IO, AssetManagerFileSystem)};
#elif AM_PLATFORM_IOS
  if (config->type == am_filesystem_type_ios)
    return {am_filesystem_type_ios,
            ampoolnew(eMemoryPoolKind_IO, NSFileSystem)};
#endif

  return {am_filesystem_type_unknown, nullptr};
}

void am_filesystem_destroy(am_filesystem_handle filesystem) {
  if (filesystem.type == am_filesystem_type_custom) {
    ampooldelete(eMemoryPoolKind_IO, CFileSystem,
                 static_cast<CFileSystem *>(filesystem.handle));
  } else if (filesystem.type == am_filesystem_type_disk) {
    ampooldelete(eMemoryPoolKind_IO, DiskFileSystem,
                 static_cast<DiskFileSystem *>(filesystem.handle));
  } else if (filesystem.type == am_filesystem_type_package) {
    ampooldelete(eMemoryPoolKind_IO, PackageFileSystem,
                 static_cast<PackageFileSystem *>(filesystem.handle));
  }
#if AM_PLATFORM_ANDROID
  else if (filesystem.type == am_filesystem_type_android) {
    ampooldelete(eMemoryPoolKind_IO, AssetManagerFileSystem,
                 static_cast<AssetManagerFileSystem *>(filesystem.handle));
  }
#elif AM_PLATFORM_IOS
  else if (filesystem.type == am_filesystem_type_ios) {
    ampooldelete(eMemoryPoolKind_IO, NSFileSystem,
                 static_cast<NSFileSystem *>(filesystem.handle));
  }
#endif
}

void am_filesystem_set_base_path(am_filesystem_handle filesystem,
                                 const am_oschar *base_path) {
  static_cast<FileSystem *>(filesystem.handle)->SetBasePath(base_path);
}

const am_oschar *am_filesystem_get_base_path(am_filesystem_handle filesystem) {
  return am_allocate_osstring(
      static_cast<FileSystem *>(filesystem.handle)->GetBasePath());
}

const am_oschar *am_filesystem_resolve_path(am_filesystem_handle filesystem,
                                            const am_oschar *path) {
  return am_allocate_osstring(
      static_cast<FileSystem *>(filesystem.handle)->ResolvePath(path));
}

am_bool am_filesystem_exists(am_filesystem_handle filesystem,
                             const am_oschar *path) {
  return BOOL_TO_AM_BOOL(
      static_cast<FileSystem *>(filesystem.handle)->Exists(path));
}

am_bool am_filesystem_is_directory(am_filesystem_handle filesystem,
                                   const am_oschar *path) {
  return BOOL_TO_AM_BOOL(
      static_cast<FileSystem *>(filesystem.handle)->IsDirectory(path));
}

const am_oschar *am_filesystem_join(am_filesystem_handle filesystem,
                                    const am_oschar **parts, am_size count) {
  std::vector<AmOsString> cpp_parts(count);
  for (am_size i = 0; i < count; i++)
    cpp_parts[i] = parts[i];

  return am_allocate_osstring(
      static_cast<FileSystem *>(filesystem.handle)->Join(cpp_parts));
}

am_file_handle am_filesystem_open_file(am_filesystem_handle filesystem,
                                       const am_oschar *path,
                                       am_file_open_mode mode) {
  const auto file = static_cast<FileSystem *>(filesystem.handle)
                        ->OpenFile(path, static_cast<eFileOpenMode>(mode));

  if (file) {
    // Store in SharedPtrManager instead of global map
    File *raw_ptr = STORE_SHARED_PTR(File, file);
    return {filesystem.type == am_filesystem_type_custom ? am_file_type_custom
                                                         : am_file_type_disk,
            raw_ptr};
  }

  return {am_file_type_unknown, nullptr};
}

void am_filesystem_start_open(am_filesystem_handle filesystem) {
  static_cast<FileSystem *>(filesystem.handle)->StartOpenFileSystem();
}

am_bool am_filesystem_try_finalize_open(am_filesystem_handle filesystem) {
  return BOOL_TO_AM_BOOL(static_cast<FileSystem *>(filesystem.handle)
                             ->TryFinalizeOpenFileSystem());
}

void am_filesystem_start_close(am_filesystem_handle filesystem) {
  static_cast<FileSystem *>(filesystem.handle)->StartCloseFileSystem();
}

am_bool am_filesystem_try_finalize_close(am_filesystem_handle filesystem) {
  return BOOL_TO_AM_BOOL(static_cast<FileSystem *>(filesystem.handle)
                             ->TryFinalizeCloseFileSystem());
}

void am_filesystem_package_set_filesystem(am_filesystem_handle filesystem,
                                          am_filesystem_config *internal) {
  if (filesystem.type != am_filesystem_type_package ||
      internal->type == am_filesystem_type_package)
    return;

  auto *fs = static_cast<PackageFileSystem *>(filesystem.handle);

  if (internal->type == am_filesystem_type_custom)
    fs->SetPlatformFileSystem<CFileSystem>(internal->v_table,
                                           internal->user_data);
  else if (internal->type == am_filesystem_type_disk)
    fs->SetPlatformFileSystem<DiskFileSystem>();
#if AM_PLATFORM_ANDROID
  else if (internal->type == am_filesystem_type_android)
    fs->SetPlatformFileSystem<AssetManagerFileSystem>();
#elif AM_PLATFORM_IOS
  else if (internal->type == am_filesystem_type_ios)
    fs->SetPlatformFileSystem<NSFileSystem>();
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif
