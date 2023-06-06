
/*
 * Copyright (c) 2023 Ruben Rodriguez
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
 * Implementation file of the module Credit
 */


#include "Credit.h"

// Declare the static instance of the class
Credit Credit::instance;



/**
 * Default constructor
 */
Credit::Credit() {}



/**
 * Load a game detail (technical information of the game
 * @param gC is the index position of the vector where the game detail is going to be stored
 * @param name is the content o the technical information credit
 */
void Credit::load(const Game_Detail gD, const std::string &name){
    instance.gameDetails[(int)gD] = name;
}



/**
 * Load right game credit
 * @param gA is the index position of the vector where the game right is going to be stored
 * @param name is the content o the right game information credit
 */
void Credit::load(const Game_Acknowlegment gA, const std::string &name){
    instance.gameAcknowlegments[(int)gA] = name;
}



/**
 * Load a soundtrack credit into the first ad
 * @param sdC is the index position of the vector where the soundtrack is going to be stored
 * @param name is the content of the soundtrack credit used in the first ad
 */
void Credit::load(const Soundtrack_Credit sdC, const std::string &name){
    instance.soundtrackCredits[(int)sdC] = name;
}



/**
 * Load a soundtrack credit into the second ad
 * @param sdC is the index position of the vector where the soundtrack is going to be stored
 * @param name is the content of the soundtrack credit used in the second ad
 */
void Credit::load(const Soundtrack_Credit_Two sdC, const std::string &name){
    instance.soundtrackCreditsTwo[(int)sdC] = name;
}



/**
 * Load a sfx credit into the first ad
 * @param sfxC is the index position of the vector where the sfx is going to be stored
 * @param name is the content of the sfx credit
 */
void Credit::load(const Sfx_Credit sfxC, const std::string &name){
    instance.soundEffectsCredits[(int)sfxC] = name;
}



/**
 * Load a enterprise credit with the enterprise name whose game soundtrack has been used in this game
 * shown in the first ad
 * @param sdE is the index position of the vector where the enterprise credit name is going to be stored
 * @param name is the content of the enterprise name whose game soundtrack has been used in this game
 * shown in the first ad
 */
void Credit::load(const Soundtrack_Enterprise sdE, const std::string &name){
    instance.soundtrackEnterpisesCredits[(int)sdE] = name;
}



/**
 * Load a enterprise credit with the enterprise name whose game soundtrack has been used in this game
 * shown in the second ad
 * @param sdE is the index position of the vector where the enterprise credit name is going to be stored
 * @param name is the content of the enterprise name whose game soundtrack has been used in this game
 * shown in the second ad
 */
void Credit::load(const Soundtrack_Enterprise_Two sdE, const std::string &name){
    instance.soundtrackEnterpisesCreditsTwo[(int)sdE] = name;
}



/**
 * Load a enterprise credit with the enterprise name whose game sfx has been used
 * @param sfxE is the index position of the vector where the enterprise credit name is going to be stored
 * @param name is the content of the enterprise name whose game sfx has been used
 */
void Credit::load(const Sfx_Enterprise sfxE, const std::string &name){
    instance.soundEffectsEnterprisesCredits[(int)sfxE] = name;
}



/**
 * Load a sprite enterprise credit
 * @param spE is the index position of the vector where the enterprise credit name is going to be stored
 * @param name is the content of the sprite enterprise credit
 */
void Credit::load(const Sprite_Enterprise spE, const std::string &name){
    instance.spritesEnterprisesCredits[(int)spE] = name;
}



/**
 * Load acknowledge of a game used to develop this one
 * @param ack is the index position of the vector where the acknowledge is going to be stored
 * @param name is the content of acknowledge of a game used to create this one
 */
void Credit::load(const Acknowledgments ack, const std::string &name){
    instance.acknowledgments[(int)ack] = name;
}



/**
 * Load all the credits of the game
 */
void Credit::loadAll(){

    // Load the technical information credits
    instance.load(Game_Detail::GAME_NAME, "OUT RUN");
    instance.load(Game_Detail::DESIGNED_BY, "DESGINED BY");
    instance.load(Game_Detail::AUTHOR, "ZGZINFINITY");
    instance.load(Game_Detail::YEAR, "2023");
    instance.load(Game_Detail::COMPANY, "ZGZ INFINITGAMES ENTERTAINMENT. INC");
    instance.load(Game_Detail::LICENSE, "UNDER LICENSE GPL-3.0");

    // Load all the right credits of the game
    instance.load(Game_Acknowlegment::RIGHTS_FIRST, "ALL RIGHTS TO THE MUSIC , SFX AND SPRITES");
    instance.load(Game_Acknowlegment::RIGHTS_SECOND, "BELONG TO THE DEVELOPING ENTERPRISES OF THE");
    instance.load(Game_Acknowlegment::RIGHTS_THIRD, "VIDEOGAMES IN WHICH OUT RUN HAS BEEN INSPIRED");

    // Load the soundtracks of the game displayed in the first ad
    instance.load(Soundtrack_Credit::SUPER_HANG_ON, "SUPER HANG ON - 1989 - SEGA MEGADRIVE");
    instance.load(Soundtrack_Credit::OUT_RUN, "OUT RUN - 1986 - ARCADE VERSION");
    instance.load(Soundtrack_Credit::OUT_RUN_3D, "3D OUT RUN - 2015 - NINTENDO 3DS");

    // Load the enterprises whose game soundtracks have been used in the first ad
    instance.load(Soundtrack_Enterprise::SUPER_HANG_ON, "SEGA AM2 AND SEGA");
    instance.load(Soundtrack_Enterprise::OUT_RUN, "SEGA AM2, SEGA, M2, NEC CORPORATION, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");
    instance.load(Soundtrack_Enterprise::OUT_RUN_3D, "SEGA AND eSHOP");

    // Load the soundtracks of the game displayed in the second ad
    instance.load(Soundtrack_Credit_Two::WORLD_CUP_ITALIA, "WORLD CUP ITALIA 90 - 1989 - SEGA MEGADRIVE");
    instance.load(Soundtrack_Credit_Two::MARIO_KART_GBA, "MARIO KART SUPER CIRCUIT - 2001 - GAME BOY ADVANCED");
    instance.load(Soundtrack_Credit_Two::MARIO_KART_TOUR, "MARIO KART TOUR - 2019 - ANDROID AND IOS");

    // Load the enterprises whose game soundtracks have been used in the second ad
    instance.load(Soundtrack_Enterprise_Two::WORLD_CUP_ITALIA, "MASTERTRONIC AND SEGA");
    instance.load(Soundtrack_Enterprise_Two::MARIO_KART_GBA, "INTELLIGENT SYSTEMS");
    instance.load(Soundtrack_Enterprise_Two::MARIO_KART_TOUR, "DeNA, NINTENDO EAD AND NINTENDO EPD");

    // Load the sfx of the games used
    instance.load(Sfx_Credit::SUPER_HANG_ON, "SUPER HANG ON - 1989 - SEGA MEGADRIVE");
    instance.load(Sfx_Credit::CRUIS_USA, "CRUIS'N USA - 1994 - N64");
    instance.load(Sfx_Credit::CRUIS_WORLD, "CRUIS'N WORLD - 1996 - N64");
    instance.load(Sfx_Credit::OUT_RUN, "OUT RUN - 1986 - ARCADE VERSION");
    instance.load(Sfx_Credit::MARIO_KART_GBA, "MARIO KART SUPER CIRCUIT - 2001 - GAME BOY ADVANCED");
    instance.load(Sfx_Credit::MARIO_KART_WII, "MARIO KART WII - 2008 -WII");
    instance.load(Sfx_Credit::GOLDEN_AXE, "GOLDEN AXE - 1989 - SEGA MEGADRIVE");

    // Load the enterprises whose game sfx have been used
    instance.load(Sfx_Enterprise::SUPER_HANG_ON, "SEGA AM2 AND SEGA");
    instance.load(Sfx_Enterprise::CRUIS_USA, "MIDWAY GAMES AND WNS INDUSTRIES");
    instance.load(Sfx_Enterprise::CRUIS_WORLD, "MIDWAY GAMES AND EUROCOM");
    instance.load(Sfx_Enterprise::OUT_RUN, "SEGA AM2, SEGA, M2, NEC CORPORATION, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");
    instance.load(Sfx_Enterprise::MARIO_KART_GBA, "INTELLIGENT SYSTEMS");
    instance.load(Sfx_Enterprise::MARIO_KART_WII, "NINTENDO");
    instance.load(Sfx_Enterprise::GOLDEN_AXE, "SEGA, BANDAI, VIRGIN INTERACTIVE, SEGA WOW, TELENET JAPAN, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");

    // Load the sprites of the games used
    instance.load(Sprite_Enterprise::SPRITE_FIRST, "ALL SPRITES USED IN THIS GAME HAVE BEEN");
    instance.load(Sprite_Enterprise::SPRITE_SECOND, "DOWNLOADED AND MODIFIED FROM SPRITERESOURCE");

    // Set all the acknowledgments to the games in which this one has been inspired
    instance.load(Acknowledgments::OUTRUN_ARCADE, "OUT RUN ARCADE - DESIGNED BY YU SUZUKI IN 1986");
    instance.load(Acknowledgments::OUTRUN_SEGA, "OUT RUN SEGA MEGADRIVE VERSION - RELEASED IN 1991");
}



/**
 * Get a game detail credit
 * @param i is the index position in the vector of the game detail credit to be obtained
 */
std::string Credit::getGameDetail(const int i){
    return instance.gameDetails[i];
}



/**
 * Get a right game credit
 * @param i is the index position in the vector of the right game credit to be obtained
 */
std::string Credit::getGameAcknowlegment(const int i){
    return instance.gameAcknowlegments[i];
}



/**
 * Get a soundtrack credit displayed in the first ad
 * @param i is the index position in the vector of the soundtrack credit to be obtained
 */
std::string Credit::getSoundtrackCredit(const int i){
    return instance.soundtrackCredits[i];
}



/**
 * Get a soundtrack credit displayed in the second ad
 * @param i is the index position in the vector of the soundtrack credit to be obtained
 */
std::string Credit::getSoundtrackCreditTwo(const int i){
    return instance.soundtrackCreditsTwo[i];
}



/**
 * Get an enterprise name credit whose game soundtrack has been used in this game of the first ad
 * @param i is the index position in the vector of the enterprise name to be obtained of the first ad
 */
std::string Credit::getSoundtrackEnterpriseCredit(const int i){
    return instance.soundtrackEnterpisesCredits[i];
}



/**
 * Get an enterprise name credit whose game soundtrack has been used in this game of the second ad
 * @param i is the index position in the vector of the enterprise name to be obtained of the second ad
 */
std::string Credit::getSoundtrackEnterpriseCreditTwo(const int i){
    return instance.soundtrackEnterpisesCreditsTwo[i];
}



/**
 * Get a sfx credit
 * @param i is the index position in the vector of the sfx credit to be obtained
 */
std::string Credit::getSfxCredit(const int i){
    return instance.soundEffectsCredits[i];
}



/**
 * Get an enterprise name credit whose game sfx has been used in this game
 * @param i is the index position in the vector of the enterprise name to be obtained
 */
std::string Credit::getSfxEnterpriseCredit(const int i){
    return instance.soundEffectsEnterprisesCredits[i];
}



/**
 * Get an enterprise name credit whose sprites has been used in this game
 * @param i is the index position in the vector of the enterprise name to be obtained
 */
std::string Credit::getSpriteEnterpriseCredit(const int i){
    return instance.spritesEnterprisesCredits[i];
}



/**
 * Get an acknowledge of a game used to create this one
 * @param i is the index position in the vector of the acknowledge name to be obtained
 */
std::string Credit::getAcknowledgments(const int i){
    return instance.acknowledgments[i];
}
