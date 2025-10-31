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

#ifndef _AM_C_ENTITY_H
#define _AM_C_ENTITY_H

#include "amplitude_common.h"
#include "amplitude_environment.h"

struct am_entity;

/**
 * @brief Represents a spatially positioned object in the game.
 *
 * Entities are used to link sound to an object in the game, sharing spatial data
 * to linked sound sources.
 */
typedef struct am_entity am_entity;

/**
 * @brief Represents a handle to a entity.
 */
typedef am_entity* am_entity_handle;

/**
 * @brief Represents the unique ID of a entity.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_entity_id;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if an entity is valid.
 *
 * @param[in] entity The entity to check.
 *
 * @return Whether the entity is valid.
 */
__api am_bool
am_entity_is_valid(am_entity_handle entity);

/**
 * @brief Gets the unique ID of the entity.
 *
 * @param[in] entity The entity to get the ID of.
 *
 * @return The unique ID of the entity.
 */
__api am_entity_id
am_entity_get_id(am_entity_handle entity);

/**
 * @brief Gets the velocity of the entity.
 *
 * @param[in] entity The entity to get the velocity of.
 *
 * @return The velocity of the entity.
 */
__api am_vec3
am_entity_get_velocity(am_entity_handle entity);

/**
 * @brief Sets the location of the entity.
 *
 * @param[in] entity The entity to set the location of.
 * @param[in] location The location to set.
 */
__api void
am_entity_set_location(am_entity_handle entity, am_vec3 location);

/**
 * @brief Gets the location of the entity.
 *
 * @param[in] entity The entity to get the location of.
 *
 * @return The location of the entity.
 */
__api am_vec3
am_entity_get_location(am_entity_handle entity);

/**
 * @brief Sets the orientation of the entity.
 *
 * @param[in] entity The entity to set the orientation of.
 * @param[in] orientation The orientation to set.
 */
__api void
am_entity_set_orientation(am_entity_handle entity, am_quaternion orientation);

/**
 * @brief Gets the forward vector of the entity.
 *
 * @param[in] entity The entity to get the direction of.
 *
 * @return The direction of the entity.
 */
__api am_vec3
am_entity_get_direction(am_entity_handle entity);

/**
 * @brief Gets the up vector of the entity.
 *
 * @param[in] entity The entity to get the up vector of.
 *
 * @return The up vector of the entity.
 */
__api am_vec3
am_entity_get_up(am_entity_handle entity);

/**
 * @brief Gets the orientation of the entity.
 *
 * @param[in] entity The entity to get the orientation of.
 *
 * @return The orientation of the entity.
 */
__api am_quaternion
am_entity_get_orientation(am_entity_handle entity);

/**
 * @brief Sets the obstruction of the entity.
 *
 * @param[in] entity The entity to set the obstruction of.
 * @param[in] obstruction The obstruction amount.
 */
__api void
am_entity_set_obstruction(am_entity_handle entity, am_float32 obstruction);

/**
 * @brief Sets the occlusion of the entity.
 *
 * @param[in] entity The entity to set the occlusion of.
 * @param[in] occlusion The occlusion amount.
 */
__api void
am_entity_set_occlusion(am_entity_handle entity, am_float32 occlusion);

/**
 * @brief Sets the directivity of the entity.
 *
 * @param[in] entity The entity to set the directivity of.
 * @param[in] directivity The directivity of the sound source, in the range [0, 1].
 * @param[in] sharpness The directivity sharpness of the sound source, in the range [1, +INF].
 * Increasing this value increases the directivity towards the front of the source.
 */
__api void
am_entity_set_directivity(am_entity_handle entity, am_float32 directivity, am_float32 sharpness);

/**
 * @brief Gets the obstruction of the entity.
 *
 * @param[in] entity The entity to get the obstruction of.
 *
 * @return The obstruction amount.
 */
__api am_float32
am_entity_get_obstruction(am_entity_handle entity);

/**
 * @brief Gets the occlusion of the entity.
 *
 * @param[in] entity The entity to get the occlusion of.
 *
 * @return The occlusion amount.
 */
__api am_float32
am_entity_get_occlusion(am_entity_handle entity);

/**
 * @brief Gets the directivity of the entity.
 *
 * @param[in] entity The entity to get the directivity of.
 *
 * @return The directivity of the sound source.
 */
__api am_float32
am_entity_get_directivity(am_entity_handle entity);

/**
 * @brief Gets the directivity sharpness of the entity.
 *
 * @param[in] entity The entity to get the directivity sharpness of.
 *
 * @return The directivity sharpness of the sound source.
 */
__api am_float32
am_entity_get_directivity_sharpness(am_entity_handle entity);

/**
 * @brief Sets the environment factor of the entity.
 *
 * @param[in] entity The entity to set the environment factor of.
 * @param[in] environment_id The environment ID to set the factor for.
 * @param[in] factor The environment factor to set.
 */
__api void
am_entity_set_environment_factor(am_entity_handle entity, am_environment_id environment_id, am_float32 factor);

/**
 * @brief Gets the environment factor of the entity.
 *
 * @param[in] entity The entity to get the environment factor of.
 * @param[in] environment_id The environment ID to get the factor for.
 *
 * @return The environment factor of the sound source.
 */
__api am_float32
am_entity_get_environment_factor(am_entity_handle entity, am_environment_id environment_id);

/**
 * @brief Gets the active channel count of the entity.
 *
 * @param[in] entity The entity to get the active channel count of.
 *
 * @return The active channel count of the sound source.
 */
__api am_uint64
am_entity_get_active_channel_count(am_entity_handle entity);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_ENTITY_H
