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

#ifndef _AM_C_ENVIRONMENT_H
#define _AM_C_ENVIRONMENT_H

#include "amplitude_common.h"

struct am_environment;
struct am_entity;
struct am_zone;
struct am_effect;

/**
 * @brief An Environment is a zone where every spatialized audio playing inside it gets
 * applied a specific effect.
 */
typedef struct am_environment am_environment;

/**
 * @brief Represents a handle to an environment.
 */
typedef am_environment* am_environment_handle;

/**
 * @brief Represents the unique ID of an environment.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_environment_id;

/**
 * @brief Represents a handle to an entity.
 */
typedef struct am_entity* am_entity_handle;

/**
 * @brief Represents a handle to a zone.
 */
typedef struct am_zone* am_zone_handle;

/**
 * @brief Represents a handle to an effect.
 */
typedef struct am_effect* am_effect_handle;

/**
 * @brief Represents the unique ID of an effect.
 */
typedef am_uint64 am_effect_id;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks whether an environment has been initialized.
 *
 * @param[in] environment The environment to check.
 *
 * @return Whether the environment is valid.
 */
__api am_bool
am_environment_is_valid(am_environment_handle environment);

/**
 * @brief Gets the unique ID of an environment.
 *
 * @param[in] environment The environment to get the ID of.
 *
 * @return The environment's unique ID.
 */
__api am_environment_id
am_environment_get_id(am_environment_handle environment);

/**
 * @brief Sets the location of an environment.
 *
 * @param[in] environment The environment to set the location of.
 * @param[in] location The new location.
 */
__api void
am_environment_set_location(am_environment_handle environment, am_vec3 location);

/**
 * @brief Gets the current location of an environment.
 *
 * @param[in] environment The environment to get the location of.
 *
 * @return The current location of the environment.
 */
__api am_vec3
am_environment_get_location(am_environment_handle environment);

/**
 * @brief Sets the orientation of an environment.
 *
 * @param[in] environment The environment to set the orientation of.
 * @param[in] orientation The new orientation.
 */
__api void
am_environment_set_orientation(am_environment_handle environment, am_quaternion orientation);

/**
 * @brief Gets the current orientation of an environment.
 *
 * @param[in] environment The environment to get the orientation of.
 *
 * @return The current orientation of the environment.
 */
__api am_quaternion
am_environment_get_orientation(am_environment_handle environment);

/**
 * @brief Gets the direction vector of an environment.
 *
 * @param[in] environment The environment to get the direction of.
 *
 * @return The direction vector.
 */
__api am_vec3
am_environment_get_direction(am_environment_handle environment);

/**
 * @brief Gets the up vector of an environment.
 *
 * @param[in] environment The environment to get the up vector of.
 *
 * @return The up vector.
 */
__api am_vec3
am_environment_get_up(am_environment_handle environment);

/**
 * @brief Gets the environment factor for the given location.
 *
 * @param[in] environment The environment to compute the factor for.
 * @param[in] location The location for which to compute the environment factor.
 *
 * @return The environment factor.
 */
__api am_float32
am_environment_get_factor_for_location(am_environment_handle environment, am_vec3 location);

/**
 * @brief Gets the environment factor for the given entity.
 *
 * @param[in] environment The environment to compute the factor for.
 * @param[in] entity The entity for which to compute the environment factor.
 *
 * @return The environment factor.
 */
__api am_float32
am_environment_get_factor_for_entity(am_environment_handle environment, am_entity_handle entity);

/**
 * @brief Sets the effect applied in the environment by ID.
 *
 * @param[in] environment The environment to set the effect for.
 * @param[in] effect_id The ID of the effect to apply in the environment.
 */
__api void
am_environment_set_effect_by_id(am_environment_handle environment, am_effect_id effect_id);

/**
 * @brief Sets the effect applied in the environment by name.
 *
 * @param[in] environment The environment to set the effect for.
 * @param[in] effect_name The name of the effect to apply in the environment.
 */
__api void
am_environment_set_effect_by_name(am_environment_handle environment, const char* effect_name);

/**
 * @brief Sets the effect applied in the environment.
 *
 * @param[in] environment The environment to set the effect for.
 * @param[in] effect The effect to apply in the environment.
 */
__api void
am_environment_set_effect(am_environment_handle environment, am_effect_handle effect);

/**
 * @brief Gets the effect linked to an environment.
 *
 * @param[in] environment The environment to get the effect from.
 *
 * @return A handle to the effect instance.
 */
__api am_effect_handle
am_environment_get_effect(am_environment_handle environment);

/**
 * @brief Sets the zone for an environment.
 *
 * @param[in] environment The environment to set the zone for.
 * @param[in] zone The environment's zone.
 */
__api void
am_environment_set_zone(am_environment_handle environment, am_zone_handle zone);

/**
 * @brief Gets the zone linked to an environment.
 *
 * @param[in] environment The environment to get the zone from.
 *
 * @return A handle to the zone instance.
 */
__api am_zone_handle
am_environment_get_zone(am_environment_handle environment);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_ENVIRONMENT_H
