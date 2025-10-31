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

#include <amplitude_entity.h>

#include "amplitude_internals.h"

extern "C" {
am_bool am_entity_is_valid(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return BOOL_TO_AM_BOOL(c.Valid());
}

am_entity_id am_entity_get_id(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetId();
}

am_vec3 am_entity_get_velocity(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  const auto &vec = c.GetVelocity();
  return from_cpp(vec);
}

void am_entity_set_location(am_entity_handle entity, am_vec3 location) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetLocation(to_cpp(location));
}

am_vec3 am_entity_get_location(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  const auto &vec = c.GetLocation();
  return from_cpp(vec);
}

void am_entity_set_orientation(am_entity_handle entity,
                               am_quaternion orientation) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetOrientation(Orientation(to_cpp(orientation)));
}

am_vec3 am_entity_get_direction(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  const auto vec = c.GetDirection();
  return from_cpp(vec);
}

am_vec3 am_entity_get_up(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  const auto vec = c.GetUp();
  return from_cpp(vec);
}

am_quaternion am_entity_get_orientation(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  const auto &orientation = c.GetOrientation();
  return from_cpp(orientation.GetQuaternion());
}

void am_entity_set_obstruction(am_entity_handle entity,
                               am_float32 obstruction) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetObstruction(obstruction);
}

void am_entity_set_occlusion(am_entity_handle entity, am_float32 occlusion) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetOcclusion(occlusion);
}

void am_entity_set_directivity(am_entity_handle entity, am_float32 directivity,
                               am_float32 sharpness) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetDirectivity(directivity, sharpness);
}

am_float32 am_entity_get_obstruction(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetObstruction();
}

am_float32 am_entity_get_occlusion(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetOcclusion();
}

am_float32 am_entity_get_directivity(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetDirectivity();
}

am_float32 am_entity_get_directivity_sharpness(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetDirectivitySharpness();
}

void am_entity_set_environment_factor(am_entity_handle entity,
                                      am_environment_id environment_id,
                                      am_float32 factor) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  c.SetEnvironmentFactor(environment_id, factor);
}

am_float32 am_entity_get_environment_factor(am_entity_handle entity,
                                            am_environment_id environment_id) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetEnvironmentFactor(environment_id);
}

am_uint64 am_entity_get_active_channel_count(am_entity_handle entity) {
  const Entity c(reinterpret_cast<EntityInternalState *>(entity));
  return c.GetActiveChannelCount();
}
}
