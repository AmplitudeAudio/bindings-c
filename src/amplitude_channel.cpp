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

#include <amplitude_channel.h>

#include "amplitude_internals.h"

extern "C" {
am_bool am_channel_is_valid(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  return BOOL_TO_AM_BOOL(c.Valid());
}

am_channel_id am_channel_get_id(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  return c.GetId();
}

am_bool am_channel_playing(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  return BOOL_TO_AM_BOOL(c.Playing());
}

void am_channel_stop(am_channel_handle channel) {
  am_channel_stop_timeout(channel, kMinFadeDuration);
}

void am_channel_stop_timeout(am_channel_handle channel, am_time duration) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.Stop(duration);
}

void am_channel_pause(am_channel_handle channel) {
  am_channel_pause_timeout(channel, kMinFadeDuration);
}

void am_channel_pause_timeout(am_channel_handle channel, am_time duration) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.Pause(duration);
}

void am_channel_resume(am_channel_handle channel) {
  am_channel_resume_timeout(channel, kMinFadeDuration);
}

void am_channel_resume_timeout(am_channel_handle channel, am_time duration) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.Resume(duration);
}

am_vec3 am_channel_get_location(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  const auto &vec = c.GetLocation();
  return from_cpp(vec);
}

void am_channel_set_location(am_channel_handle channel, am_vec3 location) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.SetLocation(to_cpp(location));
}

am_float32 am_channel_get_gain(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  return c.GetGain();
}

void am_channel_set_gain(am_channel_handle channel, am_float32 gain) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.SetGain(gain);
}

am_channel_playback_state
am_channel_get_playback_state(am_channel_handle channel) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  return static_cast<am_channel_playback_state>(c.GetPlaybackState());
}

void am_channel_on_event(am_channel_handle channel, am_channel_event event,
                         am_channel_event_callback callback, void *user_data) {
  const Channel c(reinterpret_cast<ChannelInternalState *>(channel));
  c.On(
      static_cast<eChannelEvent>(event),
      [callback](ChannelEventInfo info) {
        callback(reinterpret_cast<am_channel_handle>(info.m_source),
                 info.m_userData);
      },
      user_data);
}
}
