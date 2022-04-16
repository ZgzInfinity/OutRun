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


#include "Credit.h"


Credit Credit::instance;

Credit::Credit() {}

void Credit::loadAll(){

    instance.load(Game_Detail::GAME_NAME, "OUT RUN");
    instance.load(Game_Detail::DESIGNED_BY, "DESGINED BY");
    instance.load(Game_Detail::AUTHOR, "ZGZINFINITY");
    instance.load(Game_Detail::YEAR, "2022");
    instance.load(Game_Detail::COMPANY, "ZGZ INFINITGAMES ENTERTAINMENT. INC");
    instance.load(Game_Detail::LICENSE, "UNDER LICENSE GPL-3.0");

    instance.load(Game_Acknowlegment::RIGHTS_FIRST, "ALL RIGHTS TO THE MUSIC , SFX AND SPRITES");
    instance.load(Game_Acknowlegment::RIGHTS_SECOND, "BELONG TO THE DEVELOPING ENTERPRISES OF THE");
    instance.load(Game_Acknowlegment::RIGHTS_THIRD, "VIDEOGAMES IN WHICH OUT RUN HAS BEEN INSPIRED");

    instance.load(Soundtrack_Credit::SUPER_HANG_ON, "SUPER HANG ON - 1989 - SEGA MEGADRIVE");
    instance.load(Soundtrack_Credit::OUT_RUN, "OUT RUN - 1986 - ARCADE VERSION");
    instance.load(Soundtrack_Credit::OUT_RUN_3D, "3D OUT RUN - 2015 - NINTENDO 3DS");

    instance.load(Soundtrack_Enterprise::SUPER_HANG_ON, "SEGA AM2 AND SEGA");
    instance.load(Soundtrack_Enterprise::OUT_RUN, "SEGA AM2, SEGA, M2, NEC CORPORATION, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");
    instance.load(Soundtrack_Enterprise::OUT_RUN_3D, "SEGA AND eSHOP");

    instance.load(Soundtrack_Credit_Two::WORLD_CUP_ITALIA, "WORLD CUP ITALIA 90 - 1989 - SEGA MEGADRIVE");
    instance.load(Soundtrack_Credit_Two::MARIO_KART_GBA, "MARIO KART SUPER CIRCUIT - 2001 - GAME BOY ADVANCED");
    instance.load(Soundtrack_Credit_Two::MARIO_KART_TOUR, "MARIO KART TOUR - 2019 - ANDROID AND IOS");

    instance.load(Soundtrack_Enterprise_Two::WORLD_CUP_ITALIA, "MASTERTRONIC AND SEGA");
    instance.load(Soundtrack_Enterprise_Two::MARIO_KART_GBA, "INTELLIGENT SYSTEMS");
    instance.load(Soundtrack_Enterprise_Two::MARIO_KART_TOUR, "DeNA, NINTENDO EAD AND NINTENDO EPD");

    instance.load(Sfx_Credit::SUPER_HANG_ON, "SUPER HANG ON - 1989 - SEGA MEGADRIVE");
    instance.load(Sfx_Credit::CRUIS_USA, "CRUIS'N USA - 1994 - N64");
    instance.load(Sfx_Credit::CRUIS_WORLD, "CRUIS'N WORLD - 1996 - N64");
    instance.load(Sfx_Credit::OUT_RUN, "OUT RUN - 1986 - ARCADE VERSION");
    instance.load(Sfx_Credit::MARIO_KART_GBA, "MARIO KART SUPER CIRCUIT - 2001 - GAME BOY ADVANCED");
    instance.load(Sfx_Credit::MARIO_KART_WII, "MARIO KART WII - 2008 -WII");
    instance.load(Sfx_Credit::GOLDEN_AXE, "GOLDEN AXE - 1989 - SEGA MEGADRIVE");

    instance.load(Sfx_Enterprise::SUPER_HANG_ON, "SEGA AM2 AND SEGA");
    instance.load(Sfx_Enterprise::CRUIS_USA, "MIDWAY GAMES AND WNS INDUSTRIES");
    instance.load(Sfx_Enterprise::CRUIS_WORLD, "MIDWAY GAMES AND EUROCOM");
    instance.load(Sfx_Enterprise::OUT_RUN, "SEGA AM2, SEGA, M2, NEC CORPORATION, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");
    instance.load(Sfx_Enterprise::MARIO_KART_GBA, "INTELLIGENT SYSTEMS");
    instance.load(Sfx_Enterprise::MARIO_KART_WII, "NINTENDO");
    instance.load(Sfx_Enterprise::GOLDEN_AXE, "SEGA, BANDAI, VIRGIN INTERACTIVE, SEGA WOW, TELENET JAPAN, ACCLAIM STUDIOS LONDON AND SEGA MOBILE");

    instance.load(Sprite_Enterprise::SPRITE_FIRST, "ALL SPRITES USED IN THIS GAME HAVE BEEN");
    instance.load(Sprite_Enterprise::SPRITE_SECOND, "DOWNLOADED AND MODIFIED FROM SPRITERESOURCE");

    instance.load(Acknowledgments::OUTRUN_ARCADE, "OUT RUN ARCADE - DESIGNED BY YU SUZUKI IN 1986");
    instance.load(Acknowledgments::OUTRUN_SEGA, "OUT RUN SEGA MEGADRIVE VERSION - RELEASED IN 1991");
}

void Credit::load(const Game_Detail gD, const std::string &name){
    instance.gameDetails[(int)gD] = name;
}

void Credit::load(const Game_Acknowlegment gA, const std::string &name){
    instance.gameAcknowlegments[(int)gA] = name;
}

void Credit::load(const Soundtrack_Credit sdC, const std::string &name){
    instance.soundtrackCredits[(int)sdC] = name;
}

void Credit::load(const Soundtrack_Credit_Two sdC, const std::string &name){
    instance.soundtrackCreditsTwo[(int)sdC] = name;
}

void Credit::load(const Sfx_Credit sfxC, const std::string &name){
    instance.soundEffectsCredits[(int)sfxC] = name;
}

void Credit::load(const Soundtrack_Enterprise sdE, const std::string &name){
    instance.soundtrackEnterpisesCredits[(int)sdE] = name;
}

void Credit::load(const Soundtrack_Enterprise_Two sdE, const std::string &name){
    instance.soundtrackEnterpisesCreditsTwo[(int)sdE] = name;
}

void Credit::load(const Sfx_Enterprise sfxE, const std::string &name){
    instance.soundEffectsEnterprisesCredits[(int)sfxE] = name;
}

void Credit::load(const Sprite_Enterprise spE, const std::string &name){
    instance.spritesEnterprisesCredits[(int)spE] = name;
}

void Credit::load(const Acknowledgments ack, const std::string &name){
    instance.acknowledgments[(int)ack] = name;
}

std::string Credit::getGameDetail(const int i){
    return instance.gameDetails[i];
}

std::string Credit::getGameAcknowlegment(const int i){
    return instance.gameAcknowlegments[i];
}

std::string Credit::getSoundtrackCredit(const int i){
    return instance.soundtrackCredits[i];
}

std::string Credit::getSfxCredit(const int i){
    return instance.soundEffectsCredits[i];
}

std::string Credit::getSoundtrackEnterpriseCredit(const int i){
    return instance.soundtrackEnterpisesCredits[i];
}

std::string Credit::getSfxEnterpriseCredit(const int i){
    return instance.soundEffectsEnterprisesCredits[i];
}

std::string Credit::getSoundtrackCreditTwo(const int i){
    return instance.soundtrackCreditsTwo[i];
}

std::string Credit::getSoundtrackEnterpriseCreditTwo(const int i){
    return instance.soundtrackEnterpisesCreditsTwo[i];
}

std::string Credit::getSpriteEnterpriseCredit(const int i){
    return instance.spritesEnterprisesCredits[i];
}

std::string Credit::getAcknowledgments(const int i){
    return instance.acknowledgments[i];
}
