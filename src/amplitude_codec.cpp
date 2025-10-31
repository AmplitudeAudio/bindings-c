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

#include <amplitude_codec.h>

#include "amplitude_internals.h"

using namespace SparkyStudios::Audio::Amplitude;

// Helper functions for format conversion
static eAudioSampleFormat to_cpp_sample_format(am_audio_sample_format format) {
  switch (format) {
  case am_audio_sample_format_float32:
    return eAudioSampleFormat_Float32;
  case am_audio_sample_format_int16:
    return eAudioSampleFormat_Int16;
  case am_audio_sample_format_unknown:
  default:
    return eAudioSampleFormat_Unknown;
  }
}

static am_audio_sample_format
from_cpp_sample_format(eAudioSampleFormat format) {
  switch (format) {
  case eAudioSampleFormat_Float32:
    return am_audio_sample_format_float32;
  case eAudioSampleFormat_Int16:
    return am_audio_sample_format_int16;
  case eAudioSampleFormat_Unknown:
  default:
    return am_audio_sample_format_unknown;
  }
}

static SoundFormat to_cpp_sound_format(const am_sound_format &format) {
  SoundFormat result;
  result.SetAll(format.sample_rate, format.num_channels, format.bits_per_sample,
                format.frames_count, format.frame_size,
                to_cpp_sample_format(format.sample_type));
  return result;
}

static am_sound_format from_cpp_sound_format(const SoundFormat &format) {
  am_sound_format result;
  result.sample_rate = format.GetSampleRate();
  result.num_channels = format.GetNumChannels();
  result.bits_per_sample = format.GetBitsPerSample();
  result.frames_count = format.GetFramesCount();
  result.frame_size = format.GetFrameSize();
  result.sample_type = from_cpp_sample_format(format.GetSampleType());
  return result;
}

class CCodec final : public Codec {
public:
  class CDecoder final : public Decoder {
  public:
    CDecoder(const Codec *codec, am_codec_decoder_vtable *v_table,
             am_voidptr user_data = nullptr)
        : Decoder(codec), _v_table(v_table), _user_data(user_data) {
      if (_v_table && _v_table->create)
        _v_table->create(_user_data);
    }

    ~CDecoder() override {
      if (_v_table && _v_table->destroy)
        _v_table->destroy(_user_data);

      _v_table = nullptr;
      _user_data = nullptr;
    }

    bool Open(std::shared_ptr<File> file) override {
      if (!_v_table || !_v_table->open || !file)
        return false;

      am_file_handle handle = {am_file_type_unknown, file.get()};
      bool result = AM_BOOL_TO_BOOL(_v_table->open(_user_data, handle));

      // Update format after opening
      if (result && _v_table->get_format) {
        am_sound_format format;
        _v_table->get_format(_user_data, &format);
        m_format = to_cpp_sound_format(format);
      }

      return result;
    }

    bool Close() override {
      if (!_v_table || !_v_table->close)
        return false;

      return AM_BOOL_TO_BOOL(_v_table->close(_user_data));
    }

    AmUInt64 Load(AudioBuffer *out) override {
      if (!_v_table || !_v_table->load || !out)
        return 0;

      return _v_table->load(_user_data, out->GetData().GetBuffer());
    }

    AmUInt64 Stream(AudioBuffer *out, AmUInt64 bufferOffset,
                    AmUInt64 seekOffset, AmUInt64 length) override {
      if (!_v_table || !_v_table->stream || !out)
        return 0;

      return _v_table->stream(_user_data, out->GetData().GetBuffer(),
                              bufferOffset, seekOffset, length);
    }

    bool Seek(AmUInt64 offset) override {
      if (!_v_table || !_v_table->seek)
        return false;

      return AM_BOOL_TO_BOOL(_v_table->seek(_user_data, offset));
    }

  public:
    am_codec_decoder_vtable *_v_table;
    am_voidptr _user_data;
  };

  class CEncoder final : public Encoder {
  public:
    explicit CEncoder(const Codec *codec, am_codec_encoder_vtable *v_table,
                      am_voidptr user_data = nullptr)
        : Encoder(codec), _v_table(v_table), _user_data(user_data) {
      if (_v_table && _v_table->create)
        _v_table->create(_user_data);
    }

    ~CEncoder() override {
      if (_v_table && _v_table->destroy)
        _v_table->destroy(_user_data);

      _v_table = nullptr;
      _user_data = nullptr;
    }

    bool Open(std::shared_ptr<File> file) override {
      if (!_v_table || !_v_table->open || !file)
        return false;

      am_file_handle handle = {am_file_type_unknown, file.get()};
      return AM_BOOL_TO_BOOL(_v_table->open(_user_data, handle));
    }

    bool Close() override {
      if (!_v_table || !_v_table->close)
        return false;

      return AM_BOOL_TO_BOOL(_v_table->close(_user_data));
    }

    void SetFormat(const SoundFormat &format) override {
      Encoder::SetFormat(format);

      if (_v_table && _v_table->set_format) {
        am_sound_format c_format = from_cpp_sound_format(format);
        _v_table->set_format(_user_data, &c_format);
      }
    }

    AmUInt64 Write(AudioBuffer *in, AmUInt64 offset, AmUInt64 length) override {
      if (!_v_table || !_v_table->write || !in)
        return 0;

      return _v_table->write(_user_data, in->GetData().GetBuffer(), offset,
                             length);
    }

  public:
    am_codec_encoder_vtable *_v_table;
    am_voidptr _user_data;
  };

  explicit CCodec(const am_codec_config &config)
      : Codec(config.name), _config(config) {
    if (_config.v_table && _config.v_table->on_register)
      _config.v_table->on_register(_config.user_data);
  }

  ~CCodec() override {
    if (_config.v_table && _config.v_table->on_unregister)
      _config.v_table->on_unregister(_config.user_data);
  }

  std::shared_ptr<Decoder> CreateDecoder() override {
    if (!_config.decoder.v_table)
      return nullptr;

    return ampoolshared(eMemoryPoolKind_Codec, CDecoder, this,
                        _config.decoder.v_table, _config.decoder.user_data);
  }

  std::shared_ptr<Encoder> CreateEncoder() override {
    if (!_config.encoder.v_table)
      return nullptr;

    return ampoolshared(eMemoryPoolKind_Codec, CEncoder, this,
                        _config.encoder.v_table, _config.encoder.user_data);
  }

  [[nodiscard]] bool CanHandleFile(std::shared_ptr<File> file) const override {
    if (!_config.v_table || !_config.v_table->on_can_handle_file || !file)
      return false;

    am_file_handle handle = {am_file_type_unknown, file.get()};
    return AM_BOOL_TO_BOOL(
        _config.v_table->on_can_handle_file(_config.user_data, handle));
  }

private:
  am_codec_config _config;
};

extern "C" {

am_codec_config am_codec_config_init(const char *name) {
  am_codec_config config;

  config.name = name;
  config.user_data = nullptr;
  config.v_table = nullptr;
  config.decoder.v_table = nullptr;
  config.decoder.user_data = nullptr;
  config.encoder.v_table = nullptr;
  config.encoder.user_data = nullptr;

  return config;
}

void am_codec_register(const am_codec_config *config) {
  if (!config)
    return;

  Codec::Register(ampoolshared(eMemoryPoolKind_Codec, CCodec, *config));
}

void am_codec_unregister(const char *name) {
  if (!name)
    return;

  auto codec = Codec::Find(name);
  if (codec)
    Codec::Unregister(codec);
}

am_codec_handle am_codec_find(const char *name) {
  if (!name)
    return nullptr;

  auto codec = Codec::Find(name);
  return reinterpret_cast<am_codec_handle>(codec.get());
}

am_codec_handle am_codec_find_for_file(am_file_handle file) {
  if (!file.handle)
    return nullptr;

  auto file_ptr = static_cast<File *>(file.handle);
  auto shared_file =
      std::shared_ptr<File>(file_ptr, [](File *) {}); // Non-owning shared_ptr
  auto codec = Codec::FindForFile(shared_file);
  return reinterpret_cast<am_codec_handle>(codec.get());
}

am_bool am_codec_can_handle_file(am_codec_handle codec, am_file_handle file) {
  if (!codec || !file.handle)
    return AM_FALSE;

  auto file_ptr = static_cast<File *>(file.handle);
  auto shared_file =
      std::shared_ptr<File>(file_ptr, [](File *) {}); // Non-owning shared_ptr
  return BOOL_TO_AM_BOOL(
      reinterpret_cast<Codec *>(codec)->CanHandleFile(shared_file));
}

const char *am_codec_get_name(am_codec_handle codec) {
  if (!codec)
    return nullptr;

  const auto &name = reinterpret_cast<Codec *>(codec)->GetName();
  return am_allocate_string(name);
}

// Decoder functions

am_codec_decoder_handle am_codec_decoder_create(const char *codec_name) {
  if (!codec_name)
    return nullptr;

  auto codec = Codec::Find(codec_name);
  if (!codec)
    return nullptr;

  auto decoder = codec->CreateDecoder();
  if (!decoder)
    return nullptr;

  return reinterpret_cast<am_codec_decoder_handle>(
      STORE_SHARED_PTR(Codec::Decoder, decoder));
}

am_codec_decoder_handle
am_codec_decoder_create_from_codec(am_codec_handle codec) {
  if (!codec)
    return nullptr;

  auto decoder = reinterpret_cast<Codec *>(codec)->CreateDecoder();
  if (!decoder)
    return nullptr;

  return reinterpret_cast<am_codec_decoder_handle>(
      STORE_SHARED_PTR(Codec::Decoder, decoder));
}

void am_codec_decoder_destroy(am_codec_decoder_handle handle) {
  if (!handle)
    return;

  REMOVE_SHARED_PTR(Codec::Decoder, handle);
}

am_bool am_codec_decoder_open(am_codec_decoder_handle handle,
                              am_file_handle file) {
  if (!handle || !file.handle)
    return AM_FALSE;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return AM_FALSE;

  auto file_ptr = static_cast<File *>(file.handle);
  auto shared_file =
      std::shared_ptr<File>(file_ptr, [](File *) {}); // Non-owning shared_ptr
  return BOOL_TO_AM_BOOL(decoder->Open(shared_file));
}

am_bool am_codec_decoder_close(am_codec_decoder_handle handle) {
  if (!handle)
    return AM_FALSE;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return AM_FALSE;

  return BOOL_TO_AM_BOOL(decoder->Close());
}

am_bool am_codec_decoder_get_format(am_codec_decoder_handle handle,
                                    am_sound_format *format) {
  if (!handle || !format)
    return AM_FALSE;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return AM_FALSE;

  *format = from_cpp_sound_format(decoder->GetFormat());
  return AM_TRUE;
}

am_uint64 am_codec_decoder_load(am_codec_decoder_handle handle,
                                am_voidptr out) {
  if (!handle || !out)
    return 0;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return 0;

  // For raw buffer operations, we need to pass the buffer directly to the C
  // callback The AudioBuffer approach doesn't work with external raw buffers,
  // so we bypass it
  auto c_decoder = static_cast<CCodec::CDecoder *>(decoder.get());
  if (c_decoder && c_decoder->_v_table && c_decoder->_v_table->load) {
    return c_decoder->_v_table->load(c_decoder->_user_data, out);
  }

  return 0;
}

am_uint64 am_codec_decoder_stream(am_codec_decoder_handle handle,
                                  am_voidptr out, am_uint64 buffer_offset,
                                  am_uint64 seek_offset, am_uint64 length) {
  if (!handle || !out)
    return 0;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return 0;

  // For raw buffer operations, we need to pass the buffer directly to the C
  // callback
  auto c_decoder = static_cast<CCodec::CDecoder *>(decoder.get());
  if (c_decoder && c_decoder->_v_table && c_decoder->_v_table->stream) {
    return c_decoder->_v_table->stream(c_decoder->_user_data, out,
                                       buffer_offset, seek_offset, length);
  }

  return 0;
}

am_bool am_codec_decoder_seek(am_codec_decoder_handle handle,
                              am_uint64 offset) {
  if (!handle)
    return AM_FALSE;

  auto decoder = GET_SHARED_PTR(Codec::Decoder, handle);
  if (!decoder)
    return AM_FALSE;

  return BOOL_TO_AM_BOOL(decoder->Seek(offset));
}

// Encoder functions

am_codec_encoder_handle am_codec_encoder_create(const char *codec_name) {
  if (!codec_name)
    return nullptr;

  auto codec = Codec::Find(codec_name);
  if (!codec)
    return nullptr;

  auto encoder = codec->CreateEncoder();
  if (!encoder)
    return nullptr;

  return reinterpret_cast<am_codec_encoder_handle>(
      STORE_SHARED_PTR(Codec::Encoder, encoder));
}

am_codec_encoder_handle
am_codec_encoder_create_from_codec(am_codec_handle codec) {
  if (!codec)
    return nullptr;

  auto encoder = reinterpret_cast<Codec *>(codec)->CreateEncoder();
  if (!encoder)
    return nullptr;

  return reinterpret_cast<am_codec_encoder_handle>(
      STORE_SHARED_PTR(Codec::Encoder, encoder));
}

void am_codec_encoder_destroy(am_codec_encoder_handle handle) {
  if (!handle)
    return;

  REMOVE_SHARED_PTR(Codec::Encoder, handle);
}

am_bool am_codec_encoder_open(am_codec_encoder_handle handle,
                              am_file_handle file) {
  if (!handle || !file.handle)
    return AM_FALSE;

  auto encoder = GET_SHARED_PTR(Codec::Encoder, handle);
  if (!encoder)
    return AM_FALSE;

  auto file_ptr = static_cast<File *>(file.handle);
  auto shared_file =
      std::shared_ptr<File>(file_ptr, [](File *) {}); // Non-owning shared_ptr
  return BOOL_TO_AM_BOOL(encoder->Open(shared_file));
}

am_bool am_codec_encoder_close(am_codec_encoder_handle handle) {
  if (!handle)
    return AM_FALSE;

  auto encoder = GET_SHARED_PTR(Codec::Encoder, handle);
  if (!encoder)
    return AM_FALSE;

  return BOOL_TO_AM_BOOL(encoder->Close());
}

void am_codec_encoder_set_format(am_codec_encoder_handle handle,
                                 const am_sound_format *format) {
  if (!handle || !format)
    return;

  auto encoder = GET_SHARED_PTR(Codec::Encoder, handle);
  if (!encoder)
    return;

  SoundFormat cpp_format = to_cpp_sound_format(*format);
  encoder->SetFormat(cpp_format);
}

am_uint64 am_codec_encoder_write(am_codec_encoder_handle handle, am_voidptr in,
                                 am_uint64 offset, am_uint64 length) {
  if (!handle || !in)
    return 0;

  auto encoder = GET_SHARED_PTR(Codec::Encoder, handle);
  if (!encoder)
    return 0;

  // For raw buffer operations, we need to pass the buffer directly to the C
  // callback
  auto c_encoder = static_cast<CCodec::CEncoder *>(encoder.get());
  if (c_encoder && c_encoder->_v_table && c_encoder->_v_table->write) {
    return c_encoder->_v_table->write(c_encoder->_user_data, in, offset,
                                      length);
  }

  return 0;
}

// Utility functions

am_sound_format am_sound_format_init(void) {
  am_sound_format format;
  format.sample_rate = 44100;
  format.num_channels = 2;
  format.bits_per_sample = 32;
  format.frames_count = 0;
  format.frame_size = sizeof(float) * 2;
  format.sample_type = am_audio_sample_format_float32;
  return format;
}

void am_sound_format_set_all(am_sound_format *format, am_uint32 sample_rate,
                             am_uint16 num_channels, am_uint32 bits_per_sample,
                             am_uint64 frames_count, am_uint32 frame_size,
                             am_audio_sample_format sample_type) {
  if (!format)
    return;

  format->sample_rate = sample_rate;
  format->num_channels = num_channels;
  format->bits_per_sample = bits_per_sample;
  format->frames_count = frames_count;
  format->frame_size = frame_size;
  format->sample_type = sample_type;
}

} // extern "C"
