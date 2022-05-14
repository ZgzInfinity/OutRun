
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
 * Interface file of the module Credit
 */

#pragma once

#ifndef CREDIT_H
#define CREDIT_H

#include <iostream>
#include <array>



/**
 * Credits that represent the technical
 * information of the game
 */
enum class Game_Detail : int {
    GAME_NAME,
    DESIGNED_BY,
    AUTHOR,
    YEAR,
    COMPANY,
    LICENSE,
    __COUNT
};



/**
 * Credits that represent the rights of the game
 */
enum class Game_Acknowlegment : int {
    RIGHTS_FIRST,
    RIGHTS_SECOND,
    RIGHTS_THIRD,
    __COUNT
};



/**
 * Credits that represent the soundtracks of the game for the first ad
 */
enum class Soundtrack_Credit : int {
    SUPER_HANG_ON,
    OUT_RUN,
    OUT_RUN_3D,
    __COUNT
};



/**
 * Credits that represent the soundtracks of the game for the second ad
 */
enum class Soundtrack_Credit_Two : int {
    WORLD_CUP_ITALIA,
    MARIO_KART_GBA,
    MARIO_KART_TOUR,
    __COUNT
};



/**
 * Credits that represent the develop enterprise
 * whose game soundtracks have been used for the first ad
 */
enum class Soundtrack_Enterprise : int {
    SUPER_HANG_ON,
    OUT_RUN,
    OUT_RUN_3D,
    __COUNT
};



/**
 * Credits that represent the develop enterprise
 * whose game soundtracks have been used for the second ad
 */
enum class Soundtrack_Enterprise_Two : int {
    WORLD_CUP_ITALIA,
    MARIO_KART_GBA,
    MARIO_KART_TOUR,
    __COUNT
};



/**
 * Credits that represent the sfx of the game for the first ad
 */
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



/**
 * Credits that represent the develop enterprise
 * whose game sfx have been used
 */
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



/**
 * Credits that represent the develop enterprise
 * whose game sprites have been used
 */
enum class Sprite_Enterprise : int {
    SPRITE_FIRST,
    SPRITE_SECOND,
    __COUNT
};



/**
 * Credits that represent the acknowledgments of the game
 */
enum class Acknowledgments : int {
    OUTRUN_ARCADE,
    OUTRUN_SEGA,
    __COUNT
};



/**
 * Represent a credit information that is displayed
 * in the credit animation
 */
class Credit {

    private:

        // Static instance of the class
        static Credit instance;

        // Array with all the game detail credits
        std::array<std::string, (int)Game_Detail::__COUNT> gameDetails;

        // Array with all the right credits of the game
        std::array<std::string, (int)Game_Acknowlegment::__COUNT> gameAcknowlegments;

        // Array with all the soundtrack credits of the game used in the first ad
        std::array<std::string, (int)Soundtrack_Credit::__COUNT> soundtrackCredits;

        // Array with all the soundtrack credits of the game used in the second ad
        std::array<std::string, (int)Soundtrack_Credit::__COUNT> soundtrackCreditsTwo;

        // Array with all the sfx credits of the game used
        std::array<std::string, (int)Sfx_Credit::__COUNT> soundEffectsCredits;

        // Array with all the enterprise whose game soundtracks have been used for the first as
        std::array<std::string, (int)Soundtrack_Enterprise::__COUNT> soundtrackEnterpisesCredits;

        // Array with all the enterprise whose game soundtracks have been used for the second as
        std::array<std::string, (int)Soundtrack_Enterprise::__COUNT> soundtrackEnterpisesCreditsTwo;

        // Array with all the enterprise whose game sfx have been used
        std::array<std::string, (int)Sfx_Enterprise::__COUNT> soundEffectsEnterprisesCredits;

        // Array with all the enterprise whose game sprites have been used
        std::array<std::string, (int)Sprite_Enterprise::__COUNT> spritesEnterprisesCredits;

        // Array with all the acknowledgments to the games in which this one has been inspired
        std::array<std::string, (int)Acknowledgments::__COUNT> acknowledgments;



        /**
         * Load a game detail (technical information of the game
         * @param gC is the index position of the vector where the game detail is going to be stored
         * @param name is the content o the technical information credit
         */
        static void load(const Game_Detail gC, const std::string &name);



        /**
         * Load right game credit
         * @param gA is the index position of the vector where the game right is going to be stored
         * @param name is the content o the right game information credit
         */
        static void load(const Game_Acknowlegment gA, const std::string &name);



        /**
         * Load a soundtrack credit into the first ad
         * @param sdC is the index position of the vector where the soundtrack is going to be stored
         * @param name is the content of the soundtrack credit used in the first ad
         */
        static void load(const Soundtrack_Credit sdC, const std::string &name);



        /**
         * Load a soundtrack credit into the second ad
         * @param sdC is the index position of the vector where the soundtrack is going to be stored
         * @param name is the content of the soundtrack credit used in the second ad
         */
        static void load(const Soundtrack_Credit_Two sdC, const std::string &name);



        /**
         * Load a sfx credit into the first ad
         * @param sfxC is the index position of the vector where the sfx is going to be stored
         * @param name is the content of the sfx credit
         */
        static void load(const Sfx_Credit sfxC, const std::string &name);



        /**
         * Load a enterprise credit with the enterprise name whose game soundtrack has been used in this game
         * shown in the first ad
         * @param sdE is the index position of the vector where the enterprise credit name is going to be stored
         * @param name is the content of the enterprise name whose game soundtrack has been used in this game
         * shown in the first ad
         */
        static void load(const Soundtrack_Enterprise sdE, const std::string &name);



        /**
         * Load a enterprise credit with the enterprise name whose game soundtrack has been used in this game
         * shown in the second ad
         * @param sdE is the index position of the vector where the enterprise credit name is going to be stored
         * @param name is the content of the enterprise name whose game soundtrack has been used in this game
         * shown in the second ad
         */
        static void load(const Soundtrack_Enterprise_Two sdE, const std::string &name);



        /**
         * Load a enterprise credit with the enterprise name whose game sfx has been used
         * @param sfxE is the index position of the vector where the enterprise credit name is going to be stored
         * @param name is the content of the enterprise name whose game sfx has been used
         */
        static void load(const Sfx_Enterprise sfxE, const std::string &name);



        /**
         * Load a sprite enterprise credit
         * @param spE is the index position of the vector where the enterprise credit name is going to be stored
         * @param name is the content of the sprite enterprise credit
         */
        static void load(const Sprite_Enterprise spE, const std::string &name);



        /**
         * Load acknowledge of a game used to develop this one
         * @param ack is the index position of the vector where the acknowledge is going to be stored
         * @param name is the content of acknowledge of a game used to create this one
         */
        static void load(const Acknowledgments ack, const std::string &name);


    public:



        /**
         * Default constructor
         */
        Credit();



        /**
         * Load all the credits of the game
         */
        static void loadAll();



        /**
         * Get a game detail credit
         * @param i is the index position in the vector of the game detail credit to be obtained
         */
        static std::string getGameDetail(const int i);



        /**
         * Get a right game credit
         * @param i is the index position in the vector of the right game credit to be obtained
         */
        static std::string getGameAcknowlegment(const int i);



        /**
         * Get a soundtrack credit displayed in the first ad
         * @param i is the index position in the vector of the soundtrack credit to be obtained
         */
        static std::string getSoundtrackCredit(const int i);



        /**
         * Get a soundtrack credit displayed in the second ad
         * @param i is the index position in the vector of the soundtrack credit to be obtained
         */
        static std::string getSoundtrackCreditTwo(const int i);



        /**
         * Get an enterprise name credit whose game soundtrack has been used in this game of the first ad
         * @param i is the index position in the vector of the enterprise name to be obtained of the first ad
         */
        static std::string getSoundtrackEnterpriseCredit(const int i);



        /**
         * Get an enterprise name credit whose game soundtrack has been used in this game of the second ad
         * @param i is the index position in the vector of the enterprise name to be obtained of the second ad
         */
        static std::string getSoundtrackEnterpriseCreditTwo(const int i);



        /**
         * Get a sfx credit
         * @param i is the index position in the vector of the sfx credit to be obtained
         */
        static std::string getSfxCredit(const int i);



        /**
         * Get an enterprise name credit whose game sfx has been used in this game
         * @param i is the index position in the vector of the enterprise name to be obtained
         */
        static std::string getSfxEnterpriseCredit(const int i);



        /**
         * Get an enterprise name credit whose sprites has been used in this game
         * @param i is the index position in the vector of the enterprise name to be obtained
         */
        static std::string getSpriteEnterpriseCredit(const int i);



        /**
         * Get an acknowledge of a game used to create this one
         * @param i is the index position in the vector of the acknowledge name to be obtained
         */
        static std::string getAcknowledgments(const int i);

};

#endif // CREDIT_H
