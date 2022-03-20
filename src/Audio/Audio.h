
/*
 * Copyright (c) 2021 Andres Gavin
 * Copyright (c) 2021 Ruben Rodriguez
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

#pragma once


#ifndef AUDIO_H
#define AUDIO_H


#include <iostream>
#include <mutex>
#include <memory>
#include <SFML/Audio.hpp>
#include "../Input/Input.h"

struct Input;

enum class Soundtrack : int {
    OPTIONS,
    MAGICAL_SOUND_SHOWER,
    PASSING_BREEZE,
    SPLASH_WAVE,
    LAST_WAVE,
    GAME_OVER,
    VEHICLE_SELECTION,
    LOADING,
    CREDITS,
    __COUNT
};



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
    RACE_START,
    RACE_END_FIRST,
    RACE_END_SECOND,
    RACE_END_THIRD,

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
    CHECKPOINT_VOICE,

    SCORE_BONUS,

    HALL_FAME,
    WAVE,
    WIND,
    GAME_OVER,
    COUNTDOWN,

    BIOMES_LOADED_WELL,
    __COUNT
};

class Audio {

    private:

        static constexpr const float VOLUME_MULTIPLIER = 0.8f;
        static constexpr const float VOLUME_LOG_EXP = 1.0f;  // max true volume val.

        static Audio instance;
        std::array<std::unique_ptr<sf::Music>, (int)Soundtrack::__COUNT> themes;
        std::array<std::unique_ptr<sf::Music>, (int)Sfx::__COUNT> effects;

        static void load(const Soundtrack sd, const std::string &filename);
        static void load(const Sfx sfx, const std::string &filename);

        float musicVolumePct, sfxVolumePct;
        float getMusicValue, getSFXValue;

    public:

        static constexpr const float NUM_SOUNDTRACKS = 3;

        Audio();

        static void loadAll(Input& input);

        static void setDefaultinput();

        static void play(const Soundtrack sd, bool loop = true);

        static void play(const Sfx sfx, bool loop = false);

        static void play(const int index, bool loop);

        static bool isPlaying(const Soundtrack sd);

        static bool isPlaying(const Sfx sfx);

        static bool isPlaying(const int index);

        static void stop(const Sfx sfx);

        static void stop(const Soundtrack sd);

        static void stop(const int index);

        static void pause(const Sfx sfx);

        static void pause(const Soundtrack sd);

        static void pause(const int index);

        static void pauseSfxForReanudate();

        static void reanudateSfxPaused();

        static void stopSfx();

        static void stopMusic();

        static float logFunc(const float value);

        static void setVolume(const float musicVolumePct, const float sfxVolumePct);

        static void setVolumeMusic(const float musicVolumePct);

        static void setVolumeSfx(const float sfxVolumePct);

        static sf::Time getDurationMusic(const Soundtrack sd);

        static sf::Time getDurationSfx(const Sfx sfx);

        static float getMusicVolume();

        static float getSfxVolume();

};

#endif // AUDIO_H
