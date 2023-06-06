
/*
 * Copyright (c) 2022 Ruben Rodriguez
 *
 * This file is part of Out Run.
 * Out Run is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Out Run is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Out Run.  If not, see <https://www.gnu.org/licenses/>.
 */



/*
 * Interface file of the module Audio
 */

#pragma once

#ifndef AUDIO_H
#define AUDIO_H


#include <iostream>
#include <mutex>
#include <memory>
#include <SFML/Audio.hpp>
#include "../Input/Input.h"

struct Input;


/**
 * Soundtracks of the game
 */
enum class Soundtrack : int {
    OPTIONS,
    MAGICAL_SOUND_SHOWER,
    PASSING_BREEZE,
    SPLASH_WAVE,
    WAY_TO_MY_LOVE,
    CRUISING_LINE,
    OUTRIDE_A_CRISIS,
    SPRINTER,
    HARD_ROAD,
    WINNING_RUN,
    LAST_WAVE,
    GAME_OVER,
    VEHICLE_SELECTION,
    CIRCUIT_SELECTION,
    LOADING,
    CREDITS,
    __COUNT
};



/**
 * Sound effects (sfx) of the game
 */
enum class Sfx : int {
    MENU_LOGO_DISPLAY,
    MENU_SELECTION_MOVE,
    MENU_SELECTION_CHOOSE,
    MENU_SELECTION_CONFIRM,
    MENU_SELECTION_BACK,
    MENU_SELECTION_WRONG,
    MENU_SELECTION_MUSIC,
    MENU_PANEL_DISPLAY,
    MENU_PANEL_HIDE,

    RACE_SEMAPHORE_PREPARE,
    RACE_SEMAPHORE_START,
    RACE_START_ORIGINAL_MODE,
    RACE_START_CONTINUOUS_MODE,
    RACE_START_TIME_TRIAL_MODE,

    RACE_END_FIRST_ORIGINAL_MODE,
    RACE_END_SECOND_ORIGINAL_MODE,
    RACE_END_THIRD_ORIGINAL_MODE,

    RACE_END_FIRST_CONTINUOUS_MODE,
    RACE_END_SECOND_CONTINUOUS_MODE,

    FERRARI_ENGINE_START,
    FERRARI_ENGINE_RUN,
    FERRARI_ENGINE_BRAKE,
    FERRARI_ENGINE_SKIDDING,
    FERRARI_ENGINE_DRIFT,
    FERRARI_ENGINE_ROAD_OUTSIDE,
    FERRARI_ENGINE_UP_GEAR,
    FERRARI_ENGINE_DOWN_GEAR,
    FERRARI_CRASH,

    SHOWMAN_RACE_START,
    SHOWMAN_CONGRATULATIONS,

    BLOND_WOMAN_FIRST_SHOUT,
    BLOND_WOMAN_SECOND_SHOUT,
    BLOND_WOMAN_THIRD_SHOUT,
    BLOND_WOMAN_TEN_SECONDS,
    BLOND_WOMAN_DIE,
    BLONDE_WOMAN_HURRY_UP,
    BLONDE_WOMAN_CHOOSE_CAR,

    SPECTATORS_FIRST_SHOUT,
    SPECTATORS_SECOND_SHOUT,
    SPECTATORS_THIRD_SHOUT,
    SPECTATORS_FOURTH_SHOUT,
    SPECTATORS_FIFTH_SHOUT,
    SPECTATORS_SIXTH_SHOUT,
    SPECTATORS_SEVENTH_SHOUT,
    SPECTATORS_EIGHTH_SHOUT,
    SPECTATORS_CONGRATULATIONS,

    TRAFFIC_FIRST_ENGINE,
    TRAFFIC_SECOND_ENGINE,
    TRAFFIC_THIRD_ENGINE,
    TRAFFIC_FIRST_CLAXON,
    TRAFFIC_SECOND_CLAXON,
    TRAFFIC_THIRD_CLAXON,
    TRAFFIC_FOURTH_CLAXON,

    CHECKPOINT_ALARM,
    CHECKPOINT_VOICE_FIRST,
    CHECKPOINT_VOICE_SECOND,

    SCORE_BONUS,

    HALL_FAME,
    WAVE,
    WIND,
    GAME_OVER,
    COUNTDOWN,

    BIOMES_LOADED_WELL,
    __COUNT
};



/**
 * Static class that controls the reproduction of all the sounds of the game
 */
class Audio {

    private:

        // Volume sound multiplier factor
        static constexpr const float VOLUME_MULTIPLIER = 0.8f;

        // Max true volume val
        static constexpr const float VOLUME_LOG_EXP = 1.0f;

        // Static instance of the class
        static Audio instance;

        // Array that stores all the soundtracks of the game
        std::array<std::unique_ptr<sf::Music>, (int)Soundtrack::__COUNT> themes;

        // Array that stores all the sound effects of the game
        std::array<std::unique_ptr<sf::Music>, (int)Sfx::__COUNT> effects;

        // Values of volume of soundtrack and sfx in logarithmic scale
        float musicVolumePct, sfxVolumePct;

        // Values of volume of soundtrack and sfx from values (1 to 100)
        float getMusicValue, getSFXValue;



        /**
         * Load a soundtrack from a file
         * @param sd is the index of the soundtrack to load
         * @param filename is the name of the file from where the soundtrack must be loaded
         */
        static void load(const Soundtrack sd, const std::string &filename);



        /**
         * Load a sound effect from a file
         * @param sfx is the index of the sound effect to load
         * @param filename is the name of the file from where the sound effect must be loaded
         */
        static void load(const Sfx sfx, const std::string &filename);

    public:



        // Number of soundtracks of the game
        static constexpr const float NUM_SOUNDTRACKS = 9;



        /**
         * Default constructor
         */
        Audio();



        /**
         * Load all the soundtracks and sound effects
         * @param input is the module of the game where the audio is loaded
         */
        static void loadAll(Input& input);



        /**
         * Set the default configuration to reproduce the audio
         */
        static void setDefaultinput();



        /**
         * Play a soundtrack of the game
         * @param sd is the index of the soundtrack in the playlist
         * @param loop is a flag to control if the soundtrack must be played infinitely or not
         */
        static void play(const Soundtrack sd, bool loop = true);



        /**
         * Play a sound effect of the game
         * @param sfx is the index of the sound effect in the playlist
         * @param loop is a flag to control if the sound effect must be played infinitely or not
         */
        static void play(const Sfx sfx, bool loop = false);



        /**
         * Play a soundtrack of the game
         * @param index is the index of the soundtrack in the playlist
         * @param loop is a flag to control if the soundtrack must be played infinitely or not
         */
        static void play(const int index, bool loop);



        /**
         * Check if a soundtrack is being played or not
         * @param sd is the index of the soundtrack in the playlist
         * @return true if the soundtrack is being played. Otherwise returns false
         */
        static bool isPlaying(const Soundtrack sd);



        /**
         * Check if a sound effect is being played or not
         * @param sfx is the index of the sound effect in the playlist
         * @return true if the sound effect is being played. Otherwise returns false
         */
        static bool isPlaying(const Sfx sfx);



        /**
         * Check if a soundtrack is being played or not
         * @param index is the index of the soundtrack in the playlist
         * @return true if the soundtrack is being played. Otherwise returns false
         */
        static bool isPlaying(const int index);



        /**
         * Stop a soundtrack of the game that is being played
         * @param sd is the index of the soundtrack in the playlist
         */
        static void stop(const Soundtrack sd);



        /**
         * Stop a sfx of the game that is being played
         * @param sfx is the index of the sfx in the playlist
         */
        static void stop(const Sfx sfx);



        /**
         * Stop a soundtrack of the game that is being played
         * @param index is the index of the soundtrack in the playlist
         */
        static void stop(const int index);



        /**
         * Pause a soundtrack of the game that is being played
         * @param sd is the index of the soundtrack in the playlist
         */
        static void pause(const Soundtrack sd);



        /**
         * Pause a sfx of the game that is being played
         * @param sfx is the index of the sfx in the playlist
         */
        static void pause(const Sfx sfx);



        /**
         * Pause a soundtrack of the game that is being played
         * @param index is the index of the soundtrack in the playlist
         */
        static void pause(const int index);



        /**
         * Check all the sfx that can be played during a game round and if there is
         * anyone being played is pause
         */
        static void pauseSfxForReanudate();



        /**
         * Check all the sfx that are paused and play them
         * anyone being played is pause
         */
        static void reanudateSfxPaused();



        /**
         * Stop all the soundtracks of the game
         */
        static void stopMusic();



        /**
         * Stop all the sound effects of the game
         */
        static void stopSfx();



        /**
         * Module the new value in scale (0..1) using logarithmic scale to compute the volume
         * @param value is the level volume in scale (1..100)
         * @return the volume value in logarithmic scale
         */
        static float logFunc(const float value);



        /**
         * Set the volumes of the soundtracks and sfx of the game in scale (0..100)
         * @param musicVolumePct is the volume of the soundtrack in scale (0..1)
         * @param sfxVolumePct is the volume of the soundtrack in scale (0..1)
         */
        static void setVolume(const float musicVolumePct, const float sfxVolumePct);



        /**
         * Set the volume of the soundtracks in scale (0..100)
         * @param musicVolumePct is the volume of the soundtrack in scale (0..1)
         */
        static void setVolumeMusic(const float musicVolumePct);



        /**
         * Set the volumes of the sfx of the game in scale (0..100)
         * @param sfxVolumePct is the volume of the soundtrack in scale (0..1)
         */
        static void setVolumeSfx(const float sfxVolumePct);



        /**
         * Get the duration of a soundtrack
         * @param sd is the index of the soundtrack in the playlist
         */
        static sf::Time getDurationMusic(const Soundtrack sd);



        /**
         * Get the duration of a sfx
         * @param sfx is the index of the sound effect in the playlist
         */
        static sf::Time getDurationSfx(const Sfx sfx);



        /**
         * Get the soundtrack volume
         */
        static float getMusicVolume();


        /**
         * Get the sfx volume
         */
        static float getSfxVolume();

};

#endif // AUDIO_H
