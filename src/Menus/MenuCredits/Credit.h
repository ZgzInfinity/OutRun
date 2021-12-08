
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

#ifndef CREDIT_H
#define CREDIT_H

#include <iostream>
#include <array>


enum class Game_Detail : int {
    GAME_NAME,
    DESIGNED_BY,
    AUTHOR,
    YEAR,
    COMPANY,
    LICENSE,
    __COUNT
};

enum class Game_Acknowlegment : int {
    RIGHTS_FIRST,
    RIGHTS_SECOND,
    RIGHTS_THIRD,
    __COUNT
};


enum class Soundtrack_Credit : int {
    SUPER_HANG_ON,
    CRUIS_USA,
    CRUIS_WORLD,
    OUT_RUN,
    WORLD_CUP_ITALIA,
    __COUNT
};


enum class Soundtrack_Enterprise : int {
    SUPER_HANG_ON,
    CRUIS_USA,
    CRUIS_WORLD,
    OUT_RUN,
    WORLD_CUP_ITALIA,
    __COUNT
};


enum class Sfx_Credit : int {
    SUPER_HANG_ON,
    CRUIS_USA,
    CRUIS_WORLD,
    MARIO_KART_GBA,
    MARIO_KART_WII,
    OUT_RUN,
    GOLDEN_AXE,
    __COUNT
};

enum class Sfx_Enterprise : int {
    SUPER_HANG_ON,
    CRUIS_USA,
    CRUIS_WORLD,
    OUT_RUN,
    MARIO_KART_GBA,
    MARIO_KART_WII,
    GOLDEN_AXE,
    __COUNT
};


enum class Sprite_Enterprise : int {
    SPRITE_FIRST,
    SPRITE_SECOND,
    __COUNT
};

enum class Acknowledgments : int {
    OUTRUN_ARCADE,
    OUTRUN_SEGA,
    __COUNT
};

class Credit {

    private:

        static Credit instance;

        std::array<std::string, (int)Game_Detail::__COUNT> gameDetails;
        std::array<std::string, (int)Game_Acknowlegment::__COUNT> gameAcknowlegments;
        std::array<std::string, (int)Soundtrack_Credit::__COUNT> soundtrackCredits;
        std::array<std::string, (int)Sfx_Credit::__COUNT> soundEffectsCredits;
        std::array<std::string, (int)Soundtrack_Enterprise::__COUNT> soundtrackEnterrpisesCredits;
        std::array<std::string, (int)Sfx_Enterprise::__COUNT> soundEffectsEnterprisesCredits;
        std::array<std::string, (int)Sprite_Enterprise::__COUNT> spritesEnterprisesCredits;
        std::array<std::string, (int)Acknowledgments::__COUNT> acknowledgments;

        static void load(const Game_Detail gC, const std::string &name);
        static void load(const Game_Acknowlegment gA, const std::string &name);
        static void load(const Soundtrack_Credit sdC, const std::string &name);
        static void load(const Sfx_Credit sfxC, const std::string &name);
        static void load(const Soundtrack_Enterprise sdE, const std::string &name);
        static void load(const Sfx_Enterprise sfxE, const std::string &name);
        static void load(const Sprite_Enterprise spE, const std::string &name);
        static void load(const Acknowledgments ack, const std::string &name);


    public:

        Credit();

        static void loadAll();

        static std::string getGameDetail(const int i);

        static std::string getGameAcknowlegment(const int i);

        static std::string getSoundtrackCredit(const int i);

        static std::string getSfxCredit(const int i);

        static std::string getSoundtrackEnterpriseCredit(const int i);

        static std::string getSfxEnterpriseCredit(const int i);

        static std::string getSpriteEnterpriseCredit(const int i);

        static std::string getAcknowledgments(const int i);

};

#endif // CREDIT_H
