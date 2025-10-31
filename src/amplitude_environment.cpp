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

#include <amplitude_environment.h>

#include "amplitude_internals.h"

extern "C" {
am_bool am_environment_is_valid(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  return BOOL_TO_AM_BOOL(c.Valid());
}

am_environment_id am_environment_get_id(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  return c.GetId();
}

void am_environment_set_location(am_environment_handle environment,
                                 am_vec3 location) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  c.SetLocation(to_cpp(location));
}

am_vec3 am_environment_get_location(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const auto &vec = c.GetLocation();
  return from_cpp(vec);
}

void am_environment_set_orientation(am_environment_handle environment,
                                    am_quaternion orientation) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  c.SetOrientation(Orientation(to_cpp(orientation)));
}

am_quaternion
am_environment_get_orientation(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const auto &orientation = c.GetOrientation();
  return from_cpp(orientation.GetQuaternion());
}

am_vec3 am_environment_get_direction(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const auto vec = c.GetDirection();
  return from_cpp(vec);
}

am_vec3 am_environment_get_up(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const auto vec = c.GetUp();
  return from_cpp(vec);
}

am_float32
am_environment_get_factor_for_location(am_environment_handle environment,
                                       am_vec3 location) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  return c.GetFactor(to_cpp(location));
}

am_float32
am_environment_get_factor_for_entity(am_environment_handle environment,
                                     am_entity_handle entity) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const Entity e(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetFactor(e);
}

void am_environment_set_effect_by_id(am_environment_handle environment,
                                     am_effect_id effect_id) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  c.SetEffect(effect_id);
}

void am_environment_set_effect_by_name(am_environment_handle environment,
                                       const char *effect_name) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  c.SetEffect(AmString(effect_name));
}

void am_environment_set_effect(am_environment_handle environment,
                               am_effect_handle effect) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const Effect *e = reinterpret_cast<const Effect *>(effect);
  c.SetEffect(e);
}

am_effect_handle am_environment_get_effect(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  const Effect *effect = c.GetEffect();
  return reinterpret_cast<am_effect_handle>(const_cast<Effect *>(effect));
}

void am_environment_set_zone(am_environment_handle environment,
                             am_zone_handle zone) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));

  auto zone_ptr = GET_SHARED_PTR(Zone, zone);

  if (zone_ptr)
    c.SetZone(zone_ptr);
}

am_zone_handle am_environment_get_zone(am_environment_handle environment) {
  const Environment c(
      reinterpret_cast<EnvironmentInternalState *>(environment));
  auto zone = c.GetZone();

  if (zone)
    return reinterpret_cast<am_zone_handle>(STORE_SHARED_PTR(Zone, zone));

  return nullptr;
}
}
