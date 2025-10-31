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

#ifndef _AM_C_CHANNEL_H
#define _AM_C_CHANNEL_H

#include "amplitude_common.h"
#include "amplitude_entity.h"
#include "amplitude_listener.h"
#include "amplitude_room.h"

struct am_channel;

/**
 * @brief Represents a single channel of audio.
 *
 * Channels are used to play audio streams and manage their playback state.
 */
typedef struct am_channel am_channel;

/**
 * @brief Represents a handle to a channel.
 */
typedef am_channel* am_channel_handle;

/**
 * @brief Represents the unique ID of a channel.
 *
 * @note This is a 64-bit unsigned integer.
 */
typedef am_uint64 am_channel_id;

/**
 * @brief Enumerates the different states a channel can be in during playback.
 */
typedef enum am_channel_playback_state
{
    /**
     * @brief The channel is stopped and not rendering audio.
     */
    am_channel_playback_state_stopped = 0,

    /**
     * @brief The channel is playing audio.
     */
    am_channel_playback_state_playing = 1,

    /**
     * @brief The channel has just been played or resumed and is fading in to the `Playing` state.
     */
    am_channel_playback_state_fading_in = 2,

    /**
     * @brief The channel has just been stopped or paused and is fading out to the `Stopped` or `Paused` state.
     */
    am_channel_playback_state_fading_out = 3,

    /**
     * @brief The channel is updating the value of his linked switch state.
     */
    am_channel_playback_state_switching_state = 4,

    /**
     * @brief The channel is playing audio but in a paused state.
     */
    am_channel_playback_state_paused = 5,
} am_channel_playback_state;

/**
 * @brief Enumerates the events triggered by a channel during playback.
 */
typedef enum am_channel_event : am_uint8
{
    /**
     * @brief The playback of the channel has started.
     */
    am_channel_event_begin = 0,

    /**
     * @brief The playback of the channel has ended.
     */
    am_channel_event_end = 1,

    /**
     * @brief The playback of the channel has resumed after being paused.
     */
    am_channel_event_resume = 2,

    /**
     * @brief The playback of the channel has been paused.
     */
    am_channel_event_pause = 3,

    /**
     * @brief The playback of the channel has been stopped.
     */
    am_channel_event_stop = 4,

    /**
     * @brief The playback of the channel has looped.
     */
    am_channel_event_loop = 5
} am_channel_event;

/**
 * @brief Callback function for channel events.
 */
typedef void (*am_channel_event_callback)(am_channel_handle source, void* user_data);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if a channel is valid.
 *
 * @param[in] channel The channel to check.
 *
 * @return Whether the channel is valid.
 */
__api am_bool
am_channel_is_valid(am_channel_handle channel);

/**
 * @brief Gets the unique ID of the channel.
 *
 * @param[in] channel The channel to get the ID of.
 *
 * @return The unique ID of the channel.
 */
__api am_channel_id
am_channel_get_id(am_channel_handle channel);

/**
 * @brief Checks if the sound associated to the given channel is playing.
 *
 * @param[in] channel The channel to check.
 *
 * @return Whether the channel is currently playing.
 */
__api am_bool
am_channel_playing(am_channel_handle channel);

/**
 * @brief Stops a channel.
 *
 * Stops the channel from playing. A sound will stop on its own if it's not set
 * to loop. Looped audio must be explicitly stopped.
 *
 * @param[in] channel The channel to stop.
 */
__api void
am_channel_stop(am_channel_handle channel);

/**
 * @brief Stops a channel.
 *
 * Stops the channel from playing. A sound will stop on its own if it's not set
 * to loop. Looped audio must be explicitly stopped.
 *
 * @param[in] channel The channel to stop.
 * @param[in] duration The fade out duration before to stop the channel.
 */
__api void
am_channel_stop_timeout(am_channel_handle channel, am_time duration);

/**
 * @brief Pauses a channel.
 *
 * Pauses the channel. A paused channel may be resumed where it left off.
 *
 * @param[in] channel The channel to pause.
 */
__api void
am_channel_pause(am_channel_handle channel);

/**
 * @brief Pauses a channel.
 *
 * Pauses the channel. A paused channel may be resumed where it left off.
 *
 * @param[in] channel The channel to pause.
 * @param[in] duration The fade out duration before to pause the channel.
 */
__api void
am_channel_pause_timeout(am_channel_handle channel, am_time duration);

/**
 * @brief Resumes a paused channel.
 *
 * Resumes the channel. If the channel was paused it will continue where it
 * left off.
 *
 * @param[in] channel The channel to resume.
 */
__api void
am_channel_resume(am_channel_handle channel);

/**
 * @brief Resumes a paused channel.
 *
 * Resumes the channel. If the channel was paused it will continue where it
 * left off.
 *
 * @param[in] channel The channel to resume.
 * @param[in] duration The fade in duration after resuming the channel.
 */
__api void
am_channel_resume_timeout(am_channel_handle channel, am_time duration);

/**
 * @brief Gets the location of the given channel.
 *
 * If the audio on the channel is not set to be Positional, this method will
 * return an invalid location.
 *
 * @param[in] channel The channel to get the location of.
 *
 * @return The location of the given channel.
 */
__api am_vec3
am_channel_get_location(am_channel_handle channel);

/**
 * @brief Sets the location of the given channel.
 *
 * If the audio on the channel is not set to be Positional, this method
 * does nothing.
 *
 * @param[in] channel The channel to set the location of.
 * @param[in] location The new location of the channel.
 */
__api void
am_channel_set_location(am_channel_handle channel, am_vec3 location);

/**
 * @brief Gets the gain of the given channel.
 *
 * @param[in] channel The channel to get the gain of.
 *
 * @return The gain of the channel.
 */
__api am_float32
am_channel_get_gain(am_channel_handle channel);

/**
 * @brief Sets the gain on the given channel.
 *
 * @param[in] channel The channel to set the gain of.
 * @param[in] gain The new gain value.
 */
__api void
am_channel_set_gain(am_channel_handle channel, am_float32 gain);

/**
 * @brief Gets the playback state of the given channel.
 *
 * @param[in] channel The channel to get the playback state of.
 *
 * @return The playback state of the given channel.
 */
__api am_channel_playback_state
am_channel_get_playback_state(am_channel_handle channel);

/**
 * @brief Gets the entity associated with the given channel.
 *
 * If no entity is associated with the channel, this function returns NULL.
 * You should check if the returned entity is valid before using it.
 *
 * @param[in] channel The channel to get the entity from.
 *
 * @return The entity associated with the channel, or NULL if none.
 */
__api am_entity_handle
am_channel_get_entity(am_channel_handle channel);

/**
 * @brief Gets the listener associated with the given channel.
 *
 * If no listener is associated with the channel, this function returns NULL.
 * You should check if the returned listener is valid before using it.
 *
 * @param[in] channel The channel to get the listener from.
 *
 * @return The listener associated with the channel, or NULL if none.
 */
__api am_listener_handle
am_channel_get_listener(am_channel_handle channel);

/**
 * @brief Gets the room associated with the given channel.
 *
 * If no room is associated with the channel, this function returns NULL.
 * You should check if the returned room is valid before using it.
 *
 * @param[in] channel The channel to get the room from.
 *
 * @return The room associated with the channel, or NULL if none.
 */
__api am_room_handle
am_channel_get_room(am_channel_handle channel);

/**
 * @brief Adds an event listener to the given channel.
 *
 * @param[in] channel The channel on which to add the event listener.
 * @param[in] event The event to listen for.
 * @param[in] callback The callback function to be called when the event occurs.
 * @param[in] user_data The user data to be passed to the callback function.
 */
__api void
am_channel_on_event(am_channel_handle channel, am_channel_event event, am_channel_event_callback callback, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // _AM_C_CHANNEL_H
