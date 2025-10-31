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

#ifndef _AM_C_ROOM_H
#define _AM_C_ROOM_H

#include "amplitude_common.h"

struct am_room;
struct am_box_shape;

/**
 * @brief Defines the material type of Room walls.
 *
 * Use this enum when you want to use predefined absorption coefficients for a wall.
 * Note that the predefined coefficients are only for reference and may not be accurate
 * for your specific use case.
 */
typedef enum am_room_wall_material_type
{
    am_room_wall_material_type_transparent = 0,
    am_room_wall_material_type_acoustic_tile,
    am_room_wall_material_type_carpet_on_concrete,
    am_room_wall_material_type_heavy_drapes,
    am_room_wall_material_type_gypsum_board,
    am_room_wall_material_type_concrete_unpainted,
    am_room_wall_material_type_wood,
    am_room_wall_material_type_brick_painted,
    am_room_wall_material_type_foam_panel,
    am_room_wall_material_type_glass,
    am_room_wall_material_type_plaster_smooth,
    am_room_wall_material_type_metal,
    am_room_wall_material_type_marble,
    am_room_wall_material_type_water_surface,
    am_room_wall_material_type_ice_surface,
    am_room_wall_material_type_custom
} am_room_wall_material_type;

/**
 * @brief Lists the walls in a Room.
 */
typedef enum am_room_wall
{
    /**
     * @brief The left wall.
     */
    am_room_wall_left = 0,

    /**
     * @brief The right wall.
     */
    am_room_wall_right,

    /**
     * @brief The bottom wall (floor).
     */
    am_room_wall_floor,
    am_room_wall_bottom = am_room_wall_floor,

    /**
     * @brief The top wall (ceiling).
     */
    am_room_wall_ceiling,
    am_room_wall_top = am_room_wall_ceiling,

    /**
     * @brief The front wall.
     */
    am_room_wall_front,

    /**
     * @brief The back wall.
     */
    am_room_wall_back,

    /**
     * @brief An invalid wall. Also used to mark the maximum value of the enum.
     */
    am_room_wall_invalid
} am_room_wall;

/**
 * @brief Represents the material of a Room wall.
 */
typedef struct am_room_wall_material
{
    /**
     * @brief The type of the material.
     */
    am_room_wall_material_type type;

    /**
     * @brief The absorption coefficients of the material.
     */
    am_float32 absorption_coefficients[9];
} am_room_wall_material;

/**
 * @brief Represents a physical space where sound waves can propagate.
 *
 * Any sound source within the room will be affected by the room's properties,
 * and get applied early reflections and reverberation effects.
 */
typedef struct am_room am_room;

/**
 * @brief Represents a handle to a room.
 */
typedef am_room* am_room_handle;

/**
 * @brief Represents the unique ID of a room.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_room_id;

/**
 * @brief Represents a handle to a box shape.
 */
typedef struct am_box_shape* am_box_shape_handle;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a room wall material with default settings.
 *
 * @return A new room wall material with custom type.
 */
__api am_room_wall_material
am_room_wall_material_create();

/**
 * @brief Creates a room wall material with a predefined type.
 *
 * @param[in] type The type of the material.
 *
 * @return A new room wall material with the specified type.
 */
__api am_room_wall_material
am_room_wall_material_create_with_type(am_room_wall_material_type type);

/**
 * @brief Checks whether a room has been initialized.
 *
 * @param[in] room The room to check.
 *
 * @return Whether the room has been initialized with a valid state.
 */
__api am_bool
am_room_is_valid(am_room_handle room);

/**
 * @brief Gets the unique ID of a room.
 *
 * @param[in] room The room to get the ID of.
 *
 * @return The room's unique ID.
 */
__api am_room_id
am_room_get_id(am_room_handle room);

/**
 * @brief Sets the location of a room.
 *
 * @param[in] room The room to set the location of.
 * @param[in] location The new location.
 */
__api void
am_room_set_location(am_room_handle room, am_vec3 location);

/**
 * @brief Gets the current location of a room.
 *
 * @param[in] room The room to get the location of.
 *
 * @return The current location of the room.
 */
__api am_vec3
am_room_get_location(am_room_handle room);

/**
 * @brief Sets the orientation of a room.
 *
 * @param[in] room The room to set the orientation of.
 * @param[in] orientation The new orientation.
 */
__api void
am_room_set_orientation(am_room_handle room, am_quaternion orientation);

/**
 * @brief Gets the current orientation of a room.
 *
 * @param[in] room The room to get the orientation of.
 *
 * @return The current orientation of the room.
 */
__api am_quaternion
am_room_get_orientation(am_room_handle room);

/**
 * @brief Gets the direction vector of a room.
 *
 * @param[in] room The room to get the direction of.
 *
 * @return The direction (forward) vector.
 */
__api am_vec3
am_room_get_direction(am_room_handle room);

/**
 * @brief Gets the up vector of a room.
 *
 * @param[in] room The room to get the up vector of.
 *
 * @return The up vector.
 */
__api am_vec3
am_room_get_up(am_room_handle room);

/**
 * @brief Sets the shape's dimensions of a room.
 *
 * @param[in] room The room to set the dimensions of.
 * @param[in] dimensions The new dimensions.
 */
__api void
am_room_set_dimensions(am_room_handle room, am_vec3 dimensions);

/**
 * @brief Sets the shape representing a room.
 *
 * @param[in] room The room to set the shape of.
 * @param[in] shape The new shape.
 */
__api void
am_room_set_shape(am_room_handle room, am_box_shape_handle shape);

/**
 * @brief Gets the shape representing a room.
 *
 * @param[in] room The room to get the shape of.
 *
 * @return The room shape.
 */
__api am_box_shape_handle
am_room_get_shape(am_room_handle room);

/**
 * @brief Sets the material for a specific wall of a room.
 *
 * @param[in] room The room to set the wall material of.
 * @param[in] wall The wall to set the material for.
 * @param[in] material The new material.
 */
__api void
am_room_set_wall_material(am_room_handle room, am_room_wall wall, am_room_wall_material material);

/**
 * @brief Sets the material for all walls of a room.
 *
 * @param[in] room The room to set the wall materials of.
 * @param[in] material The new material.
 */
__api void
am_room_set_all_wall_materials(am_room_handle room, am_room_wall_material material);

/**
 * @brief Sets the material for each wall of a room.
 *
 * @param[in] room The room to set the wall materials of.
 * @param[in] left_wall_material The material for the left wall.
 * @param[in] right_wall_material The material for the right wall.
 * @param[in] floor_material The material for the floor.
 * @param[in] ceiling_material The material for the ceiling.
 * @param[in] front_wall_material The material for the front wall.
 * @param[in] back_wall_material The material for the back wall.
 */
__api void
am_room_set_wall_materials(
    am_room_handle room,
    am_room_wall_material left_wall_material,
    am_room_wall_material right_wall_material,
    am_room_wall_material floor_material,
    am_room_wall_material ceiling_material,
    am_room_wall_material front_wall_material,
    am_room_wall_material back_wall_material);

/**
 * @brief Gets the material for a specific wall of a room.
 *
 * @param[in] room The room to get the wall material of.
 * @param[in] wall The wall to get the material for.
 *
 * @return The material of the specified wall.
 */
__api am_room_wall_material
am_room_get_wall_material(am_room_handle room, am_room_wall wall);

/**
 * @brief Sets the room effects gain.
 *
 * @param[in] room The room to set the gain of.
 * @param[in] gain The gain applied to early reflections and reverberations effects.
 */
__api void
am_room_set_gain(am_room_handle room, am_float32 gain);

/**
 * @brief Gets the room effects gain.
 *
 * @param[in] room The room to get the gain of.
 *
 * @return The room effects gain.
 */
__api am_float32
am_room_get_gain(am_room_handle room);

/**
 * @brief Gets the volume of a room in m3.
 *
 * @param[in] room The room to get the volume of.
 *
 * @return The volume of the room's shape.
 */
__api am_float32
am_room_get_volume(am_room_handle room);

/**
 * @brief Gets the dimensions of a room in meters.
 *
 * @param[in] room The room to get the dimensions of.
 *
 * @return The dimensions of the room's shape.
 */
__api am_vec3
am_room_get_dimensions(am_room_handle room);

/**
 * @brief Gets the surface area for a specific wall of a room.
 *
 * @param[in] room The room to get the surface area of.
 * @param[in] wall The wall to get the surface area for.
 *
 * @return The surface area of the specified wall.
 */
__api am_float32
am_room_get_surface_area(am_room_handle room, am_room_wall wall);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_ROOM_H
