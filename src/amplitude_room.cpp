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

#include <amplitude_room.h>

#include "amplitude_internals.h"

// Conversion functions for enum types
inline eRoomWallMaterialType
to_cpp_material_type(am_room_wall_material_type type) {
  switch (type) {
  case am_room_wall_material_type_transparent:
    return eRoomWallMaterialType_Transparent;
  case am_room_wall_material_type_acoustic_tile:
    return eRoomWallMaterialType_AcousticTile;
  case am_room_wall_material_type_carpet_on_concrete:
    return eRoomWallMaterialType_CarpetOnConcrete;
  case am_room_wall_material_type_heavy_drapes:
    return eRoomWallMaterialType_HeavyDrapes;
  case am_room_wall_material_type_gypsum_board:
    return eRoomWallMaterialType_GypsumBoard;
  case am_room_wall_material_type_concrete_unpainted:
    return eRoomWallMaterialType_ConcreteUnpainted;
  case am_room_wall_material_type_wood:
    return eRoomWallMaterialType_Wood;
  case am_room_wall_material_type_brick_painted:
    return eRoomWallMaterialType_BrickPainted;
  case am_room_wall_material_type_foam_panel:
    return eRoomWallMaterialType_FoamPanel;
  case am_room_wall_material_type_glass:
    return eRoomWallMaterialType_Glass;
  case am_room_wall_material_type_plaster_smooth:
    return eRoomWallMaterialType_PlasterSmooth;
  case am_room_wall_material_type_metal:
    return eRoomWallMaterialType_Metal;
  case am_room_wall_material_type_marble:
    return eRoomWallMaterialType_Marble;
  case am_room_wall_material_type_water_surface:
    return eRoomWallMaterialType_WaterSurface;
  case am_room_wall_material_type_ice_surface:
    return eRoomWallMaterialType_IceSurface;
  case am_room_wall_material_type_custom:
  default:
    return eRoomWallMaterialType_Custom;
  }
}

inline am_room_wall_material_type
from_cpp_material_type(eRoomWallMaterialType type) {
  switch (type) {
  case eRoomWallMaterialType_Transparent:
    return am_room_wall_material_type_transparent;
  case eRoomWallMaterialType_AcousticTile:
    return am_room_wall_material_type_acoustic_tile;
  case eRoomWallMaterialType_CarpetOnConcrete:
    return am_room_wall_material_type_carpet_on_concrete;
  case eRoomWallMaterialType_HeavyDrapes:
    return am_room_wall_material_type_heavy_drapes;
  case eRoomWallMaterialType_GypsumBoard:
    return am_room_wall_material_type_gypsum_board;
  case eRoomWallMaterialType_ConcreteUnpainted:
    return am_room_wall_material_type_concrete_unpainted;
  case eRoomWallMaterialType_Wood:
    return am_room_wall_material_type_wood;
  case eRoomWallMaterialType_BrickPainted:
    return am_room_wall_material_type_brick_painted;
  case eRoomWallMaterialType_FoamPanel:
    return am_room_wall_material_type_foam_panel;
  case eRoomWallMaterialType_Glass:
    return am_room_wall_material_type_glass;
  case eRoomWallMaterialType_PlasterSmooth:
    return am_room_wall_material_type_plaster_smooth;
  case eRoomWallMaterialType_Metal:
    return am_room_wall_material_type_metal;
  case eRoomWallMaterialType_Marble:
    return am_room_wall_material_type_marble;
  case eRoomWallMaterialType_WaterSurface:
    return am_room_wall_material_type_water_surface;
  case eRoomWallMaterialType_IceSurface:
    return am_room_wall_material_type_ice_surface;
  case eRoomWallMaterialType_Custom:
  default:
    return am_room_wall_material_type_custom;
  }
}

inline eRoomWall to_cpp_wall(am_room_wall wall) {
  switch (wall) {
  case am_room_wall_left:
    return eRoomWall_Left;
  case am_room_wall_right:
    return eRoomWall_Right;
  case am_room_wall_floor:
    return eRoomWall_Floor;
  case am_room_wall_ceiling:
    return eRoomWall_Ceiling;
  case am_room_wall_front:
    return eRoomWall_Front;
  case am_room_wall_back:
    return eRoomWall_Back;
  case am_room_wall_invalid:
  default:
    return eRoomWall_Invalid;
  }
}

inline RoomWallMaterial to_cpp(const am_room_wall_material &material) {
  RoomWallMaterial cpp_material(to_cpp_material_type(material.type));
  std::memcpy(cpp_material.m_absorptionCoefficients,
              material.absorption_coefficients,
              sizeof(cpp_material.m_absorptionCoefficients));
  return cpp_material;
}

inline am_room_wall_material from_cpp(const RoomWallMaterial &material) {
  am_room_wall_material c_material;
  c_material.type = from_cpp_material_type(material.m_type);
  std::memcpy(c_material.absorption_coefficients,
              material.m_absorptionCoefficients,
              sizeof(c_material.absorption_coefficients));
  return c_material;
}

extern "C" {
am_room_wall_material am_room_wall_material_create() {
  const RoomWallMaterial cpp_material;
  return from_cpp(cpp_material);
}

am_room_wall_material
am_room_wall_material_create_with_type(am_room_wall_material_type type) {
  const RoomWallMaterial cpp_material(to_cpp_material_type(type));
  return from_cpp(cpp_material);
}

am_bool am_room_is_valid(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  return BOOL_TO_AM_BOOL(c.Valid());
}

am_room_id am_room_get_id(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  return c.GetId();
}

void am_room_set_location(am_room_handle room, am_vec3 location) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetLocation(to_cpp(location));
}

am_vec3 am_room_get_location(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto &vec = c.GetLocation();
  return from_cpp(vec);
}

void am_room_set_orientation(am_room_handle room, am_quaternion orientation) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetOrientation(Orientation(to_cpp(orientation)));
}

am_quaternion am_room_get_orientation(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto &orientation = c.GetOrientation();
  return from_cpp(orientation.GetQuaternion());
}

am_vec3 am_room_get_direction(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto vec = c.GetDirection();
  return from_cpp(vec);
}

am_vec3 am_room_get_up(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto vec = c.GetUp();
  return from_cpp(vec);
}

void am_room_set_dimensions(am_room_handle room, am_vec3 dimensions) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetDimensions(to_cpp(dimensions));
}

void am_room_set_shape(am_room_handle room, am_box_shape_handle shape) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  // Cast the opaque handle to BoxShape pointer and dereference it
  const BoxShape *box_shape = reinterpret_cast<const BoxShape *>(shape);
  if (box_shape != nullptr) {
    c.SetShape(*box_shape);
  }
}

am_box_shape_handle am_room_get_shape(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto &shape = c.GetShape();
  // Return a pointer to the shape - this is safe as long as the room remains
  // valid
  return reinterpret_cast<am_box_shape_handle>(const_cast<BoxShape *>(&shape));
}

void am_room_set_wall_material(am_room_handle room, am_room_wall wall,
                               am_room_wall_material material) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetWallMaterial(to_cpp_wall(wall), to_cpp(material));
}

void am_room_set_all_wall_materials(am_room_handle room,
                                    am_room_wall_material material) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetAllWallMaterials(to_cpp(material));
}

void am_room_set_wall_materials(am_room_handle room,
                                am_room_wall_material left_wall_material,
                                am_room_wall_material right_wall_material,
                                am_room_wall_material floor_material,
                                am_room_wall_material ceiling_material,
                                am_room_wall_material front_wall_material,
                                am_room_wall_material back_wall_material) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetWallMaterials(to_cpp(left_wall_material), to_cpp(right_wall_material),
                     to_cpp(floor_material), to_cpp(ceiling_material),
                     to_cpp(front_wall_material), to_cpp(back_wall_material));
}

am_room_wall_material am_room_get_wall_material(am_room_handle room,
                                                am_room_wall wall) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto &material = c.GetWallMaterial(to_cpp_wall(wall));
  return from_cpp(material);
}

void am_room_set_gain(am_room_handle room, am_float32 gain) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  c.SetGain(gain);
}

am_float32 am_room_get_gain(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  return c.GetGain();
}

am_float32 am_room_get_volume(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  return c.GetVolume();
}

am_vec3 am_room_get_dimensions(am_room_handle room) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  const auto vec = c.GetDimensions();
  return from_cpp(vec);
}

am_float32 am_room_get_surface_area(am_room_handle room, am_room_wall wall) {
  const Room c(reinterpret_cast<RoomInternalState *>(room));
  return c.GetSurfaceArea(to_cpp_wall(wall));
}
}
