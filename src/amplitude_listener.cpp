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

#include <amplitude_listener.h>

#include "amplitude_internals.h"

extern "C" {
am_bool am_listener_is_valid(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  return BOOL_TO_AM_BOOL(c.Valid());
}

am_listener_id am_listener_get_id(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  return c.GetId();
}

am_vec3 am_listener_get_velocity(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto &vec = c.GetVelocity();
  return from_cpp(vec);
}

am_vec3 am_listener_get_location(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto &vec = c.GetLocation();
  return from_cpp(vec);
}

void am_listener_set_location(am_listener_handle listener, am_vec3 location) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  c.SetLocation(to_cpp(location));
}

am_vec3 am_listener_get_direction(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto vec = c.GetDirection();
  return from_cpp(vec);
}

am_vec3 am_listener_get_up(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto vec = c.GetUp();
  return from_cpp(vec);
}

void am_listener_set_orientation(am_listener_handle listener,
                                 am_quaternion orientation) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  c.SetOrientation(Orientation(to_cpp(orientation)));
}

am_quaternion am_listener_get_orientation(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto &orientation = c.GetOrientation();
  return from_cpp(orientation.GetQuaternion());
}

void am_listener_set_directivity(am_listener_handle listener,
                                 am_float32 directivity, am_float32 sharpness) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  c.SetDirectivity(directivity, sharpness);
}

am_float32 am_listener_get_directivity(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  return c.GetDirectivity();
}

am_float32 am_listener_get_directivity_sharpness(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  return c.GetDirectivitySharpness();
}

am_mat4 am_listener_get_inverse_matrix(am_listener_handle listener) {
  const Listener c(reinterpret_cast<ListenerInternalState *>(listener));
  const auto &mat = c.GetInverseMatrix();
  return from_cpp(mat);
}
}
