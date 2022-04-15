
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


#include "Audio.h"


Audio Audio::instance;

Audio::Audio() {}

void Audio::loadAll(Input& input){

    instance.load(Soundtrack::OPTIONS, "Resources/Soundtracks/Options.ogg");
    instance.load(Soundtrack::MAGICAL_SOUND_SHOWER, "Resources/Soundtracks/Magical_Sound_Shower.ogg");
    instance.load(Soundtrack::PASSING_BREEZE, "Resources/Soundtracks/Passing_Breeze.ogg");
    instance.load(Soundtrack::SPLASH_WAVE, "Resources/Soundtracks/Splash_Wave.ogg");
    instance.load(Soundtrack::WAY_TO_MY_LOVE, "Resources/Soundtracks/Way_To_My_Love.ogg");
    instance.load(Soundtrack::CRUISING_LINE, "Resources/Soundtracks/Cruising_Line.ogg");
    instance.load(Soundtrack::OUTRIDE_A_CRISIS, "Resources/Soundtracks/Outride_A_Crisis.ogg");
    instance.load(Soundtrack::SPRINTER, "Resources/Soundtracks/Sprinter.ogg");
    instance.load(Soundtrack::HARD_ROAD, "Resources/Soundtracks/Hard_Road.ogg");
    instance.load(Soundtrack::WINNING_RUN, "Resources/Soundtracks/Winning_Run.ogg");
    instance.load(Soundtrack::LAST_WAVE, "Resources/Soundtracks/Last_Wave.ogg");

    instance.load(Soundtrack::GAME_OVER, "Resources/Soundtracks/Game_Over.ogg");
    instance.load(Soundtrack::VEHICLE_SELECTION, "Resources/Soundtracks/Vehicle_Selection.ogg");
    instance.load(Soundtrack::LOADING, "Resources/Soundtracks/Loading.ogg");
    instance.load(Soundtrack::CREDITS, "Resources/Soundtracks/Credits.ogg");

    instance.load(Sfx::MENU_LOGO_DISPLAY, "Resources/SoundEffects/Logo_Display.ogg");
    instance.load(Sfx::MENU_SELECTION_MOVE, "Resources/SoundEffects/Menu_Selection_Move.ogg");
    instance.load(Sfx::MENU_SELECTION_CHOOSE, "Resources/SoundEffects/Menu_Selection_Choose.ogg");
    instance.load(Sfx::MENU_SELECTION_CONFIRM, "Resources/SoundEffects/Menu_Selection_Confirm.ogg");
    instance.load(Sfx::MENU_SELECTION_BACK, "Resources/SoundEffects/Menu_Selection_Back.ogg");
    instance.load(Sfx::MENU_SELECTION_WRONG, "Resources/SoundEffects/Menu_Selection_Wrong.ogg");
    instance.load(Sfx::MENU_SELECTION_MUSIC, "Resources/SoundEffects/Menu_Selection_Music.ogg");
    instance.load(Sfx::MENU_PANEL_DISPLAY, "Resources/SoundEffects/Menu_Panel_Display.ogg");
    instance.load(Sfx::MENU_PANEL_HIDE, "Resources/SoundEffects/Menu_Panel_Hide.ogg");

    instance.load(Sfx::RACE_SEMAPHORE_PREPARE, "Resources/SoundEffects/Race_Semaphore_Prepare.ogg");
    instance.load(Sfx::RACE_SEMAPHORE_START, "Resources/SoundEffects/Race_Semaphore_Start.ogg");
    instance.load(Sfx::RACE_START, "Resources/SoundEffects/Race_Start.ogg");
    instance.load(Sfx::RACE_END_FIRST, "Resources/SoundEffects/Race_End_First.ogg");
    instance.load(Sfx::RACE_END_SECOND, "Resources/SoundEffects/Race_End_Second.ogg");
    instance.load(Sfx::RACE_END_THIRD, "Resources/SoundEffects/Race_End_Third.ogg");



    instance.load(Sfx::FERRARI_ENGINE_START, "Resources/SoundEffects/Ferrari_Engine_Start.ogg");
    instance.load(Sfx::FERRARI_ENGINE_RUN, "Resources/SoundEffects/Ferrari_Engine_Run.ogg");
    instance.load(Sfx::FERRARI_ENGINE_BRAKE, "Resources/SoundEffects/Ferrari_Engine_Brake.ogg");
    instance.load(Sfx::FERRARI_ENGINE_SKIDDING, "Resources/SoundEffects/Ferrari_Engine_Skidding.ogg");
    instance.load(Sfx::FERRARI_ENGINE_DRIFT, "Resources/SoundEffects/Ferrari_Engine_Drift.ogg");
    instance.load(Sfx::FERRARI_ENGINE_ROAD_OUTSIDE, "Resources/SoundEffects/Ferrari_Engine_Outside.ogg");
    instance.load(Sfx::FERRARI_ENGINE_UP_GEAR, "Resources/SoundEffects/Ferrari_Engine_Up_Gear.ogg");
    instance.load(Sfx::FERRARI_ENGINE_DOWN_GEAR, "Resources/SoundEffects/Ferrari_Engine_Down_Gear.ogg");
    instance.load(Sfx::FERRARI_CRASH, "Resources/SoundEffects/Ferrari_Crash.ogg");

    instance.load(Sfx::SHOWMAN_RACE_START, "Resources/SoundEffects/Showman_Race_Start.ogg");
    instance.load(Sfx::SHOWMAN_CONGRATULATIONS, "Resources/SoundEffects/Showman_Congratulations.ogg");

    instance.load(Sfx::BLOND_WOMAN_FIRST_SHOUT, "Resources/SoundEffects/Blond_Woman_First_Shout.ogg");
    instance.load(Sfx::BLOND_WOMAN_SECOND_SHOUT, "Resources/SoundEffects/Blond_Woman_Second_Shout.ogg");
    instance.load(Sfx::BLOND_WOMAN_THIRD_SHOUT, "Resources/SoundEffects/Blond_Woman_Third_Shout.ogg");
    instance.load(Sfx::BLOND_WOMAN_TEN_SECONDS, "Resources/SoundEffects/Blond_Woman_Ten_Seconds.ogg");
    instance.load(Sfx::BLOND_WOMAN_DIE, "Resources/SoundEffects/Blond_Woman_Die.ogg");
    instance.load(Sfx::BLONDE_WOMAN_HURRY_UP, "Resources/SoundEffects/Blond_Woman_Hurry_Up.ogg");
    instance.load(Sfx::BLONDE_WOMAN_CHOOSE_CAR, "Resources/SoundEffects/Blond_Woman_Choose_Car.ogg");

    instance.load(Sfx::SPECTATORS_FIRST_SHOUT, "Resources/SoundEffects/Spectators_First_Shout.ogg");
    instance.load(Sfx::SPECTATORS_SECOND_SHOUT, "Resources/SoundEffects/Spectators_Second_Shout.ogg");
    instance.load(Sfx::SPECTATORS_THIRD_SHOUT, "Resources/SoundEffects/Spectators_Third_Shout.ogg");
    instance.load(Sfx::SPECTATORS_FOURTH_SHOUT, "Resources/SoundEffects/Spectators_Fourth_Shout.ogg");
    instance.load(Sfx::SPECTATORS_FIFTH_SHOUT, "Resources/SoundEffects/Spectators_Fifth_Shout.ogg");
    instance.load(Sfx::SPECTATORS_SIXTH_SHOUT, "Resources/SoundEffects/Spectators_Sixth_Shout.ogg");
    instance.load(Sfx::SPECTATORS_SEVENTH_SHOUT, "Resources/SoundEffects/Spectators_Seventh_Shout.ogg");
    instance.load(Sfx::SPECTATORS_EIGHTH_SHOUT, "Resources/SoundEffects/Spectators_Eighth_Shout.ogg");

    instance.load(Sfx::SPECTATORS_CONGRATULATIONS, "Resources/SoundEffects/Spectators_Congratulations.ogg");

    instance.load(Sfx::TRAFFIC_FIRST_ENGINE, "Resources/SoundEffects/Traffic_First_Engine.ogg");
    instance.load(Sfx::TRAFFIC_SECOND_ENGINE, "Resources/SoundEffects/Traffic_Second_Engine.ogg");
    instance.load(Sfx::TRAFFIC_THIRD_ENGINE, "Resources/SoundEffects/Traffic_Third_Engine.ogg");
    instance.load(Sfx::TRAFFIC_FIRST_CLAXON, "Resources/SoundEffects/Traffic_First_Claxon.ogg");
    instance.load(Sfx::TRAFFIC_SECOND_CLAXON, "Resources/SoundEffects/Traffic_Second_Claxon.ogg");
    instance.load(Sfx::TRAFFIC_THIRD_CLAXON, "Resources/SoundEffects/Traffic_Third_Claxon.ogg");
    instance.load(Sfx::TRAFFIC_FOURTH_CLAXON, "Resources/SoundEffects/Traffic_Fourth_Claxon.ogg");

    instance.load(Sfx::CHECKPOINT_ALARM, "Resources/SoundEffects/Checkpoint_Alarm.ogg");
    instance.load(Sfx::CHECKPOINT_VOICE, "Resources/SoundEffects/Checkpoint_Voice.ogg");

    instance.load(Sfx::SCORE_BONUS, "Resources/SoundEffects/Score_Bonus.ogg");

    instance.load(Sfx::HALL_FAME, "Resources/SoundEffects/Hall_Fame.ogg");
    instance.load(Sfx::WAVE, "Resources/SoundEffects/Wave.ogg");
    instance.load(Sfx::WIND, "Resources/SoundEffects/Wind.ogg");
    instance.load(Sfx::GAME_OVER, "Resources/SoundEffects/Game_Over.ogg");
    instance.load(Sfx::COUNTDOWN, "Resources/SoundEffects/Countdown.ogg");

    instance.load(Sfx::BIOMES_LOADED_WELL, "Resources/SoundEffects/Biomes_Loaded_Well.ogg");

    if (input.exitsSettings){
        if ((input.volumeMusic % 5 == 0 && input.volumeMusic >= 0 && input.volumeMusic <= 100) &&
            (input.volumeEffects % 5 == 0 && input.volumeEffects >= 0 && input.volumeEffects <= 100))
        {
            Audio::setVolume(0.01f * input.volumeMusic, 0.01f * input.volumeEffects);
        }
    }
    else {
        Audio::setDefaultinput();
    }
}

void Audio::load(const Soundtrack sd, const std::string &filename) {
    std::unique_ptr<sf::Music> soundtrack = std::make_unique<sf::Music>();
    soundtrack->openFromFile(filename);
    instance.themes[(int)sd] = move(soundtrack);
}


void Audio::load(const Sfx Sfx, const std::string &filename) {
    std::unique_ptr<sf::Music> effect = std::make_unique<sf::Music>();
    effect->openFromFile(filename);
    instance.effects[(int)Sfx] = move(effect);
}


void Audio::setDefaultinput(){
    instance.musicVolumePct = logFunc(1.f) * 100.0;
    instance.sfxVolumePct = logFunc(1.f) * 100.0;
    instance.getMusicValue = 1.f;
    instance.getSFXValue = 1.f;
}



void Audio::play(const Soundtrack sd, bool loop) {
    stop(sd);
    instance.themes[(int)sd]->play();
    instance.themes[(int)sd]->setLoop(loop);
    instance.themes[(int)sd]->setVolume(instance.musicVolumePct);
}

void Audio::play(const Sfx sfx, bool loop) {
    if (instance.effects[(int)sfx]->getStatus() != sf::SoundSource::Paused)
        stop(sfx);


    instance.effects[(int)sfx]->play();
    instance.effects[(int)sfx]->setLoop(loop);
    instance.effects[(int)sfx]->setVolume(instance.sfxVolumePct);
}


void Audio::play(const int index, bool loop) {
    if (instance.themes[index]->getStatus() != sf::SoundSource::Paused)
        stop(index);

    instance.themes[index]->play();
    instance.themes[index]->setLoop(loop);
    instance.themes[index]->setVolume(instance.musicVolumePct);
}

bool Audio::isPlaying(const Soundtrack sd) {
    bool playing;
    playing = instance.themes[(int)sd]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}

bool Audio::isPlaying(const Sfx Sfx) {
    bool playing;
    playing = instance.effects[(int)Sfx]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}

bool Audio::isPlaying(const int index){
    bool playing;
    playing = instance.themes[index]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}


void Audio::pause(const Soundtrack sd) {
    if (instance.themes[(int)sd]->getStatus() == sf::SoundSource::Status::Playing){
        instance.themes[(int)sd]->pause();
    }
}

void Audio::pause(const Sfx Sfx) {
    if (instance.effects[(int)Sfx]->getStatus() == sf::SoundSource::Status::Playing){
        instance.effects[(int)Sfx]->pause();
    }
}

void Audio::pause(const int index) {
    if (instance.themes[index]->getStatus() == sf::SoundSource::Status::Playing){
        instance.themes[index]->pause();
    }
}

void Audio::pauseSfxForReanudate(){
    for (int sfxInt = (int)Sfx::RACE_SEMAPHORE_PREPARE; sfxInt < (int)Sfx::__COUNT; sfxInt++){
        Sfx sfx = static_cast<Sfx>(sfxInt);
        if (isPlaying(sfx))
            pause(sfx);
    }
}

void Audio::reanudateSfxPaused(){
    for (int i = 0; i < (int)Sfx::__COUNT; i++){
        if (instance.effects[i]->getStatus() == sf::SoundSource::Status::Paused){
            Sfx sfx = static_cast<Sfx>(i);
            play(sfx, false);
        }
    }
}

void Audio::stop(const Sfx Sfx) {
    instance.effects[(int)Sfx]->stop();
}


void Audio::stop(const Soundtrack sd) {
    instance.themes[(int)sd]->stop();
}

void Audio::stop(const int index) {
    instance.themes[index]->stop();
}

void Audio::stopSfx() {
    for (int i = 0; i < (int)Sfx::__COUNT; i++){
        instance.effects[i]->stop();
    }
}

void Audio::stopMusic() {
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++) {
        instance.themes[i]->stop();
    }
}


float Audio::logFunc(const float value) {
    float ret = -log10f(powf(1 - value * 0.9f, VOLUME_LOG_EXP));
    if (ret > 1.0f) {
        ret = 1.0f;
    }
    return ret;
}

// set volume as percent 0-1
void Audio::setVolume(const float musicVolumePct, const float sfxVolumePct) {
    instance.getMusicValue = musicVolumePct;
    instance.getSFXValue = sfxVolumePct;
    instance.musicVolumePct = logFunc(musicVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    instance.sfxVolumePct = logFunc(sfxVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++) {
        instance.themes[i]->setVolume(instance.musicVolumePct);
    }
    for (int i = 0; i < (int)Sfx::__COUNT; i++) {
        instance.effects[i]->setVolume(instance.sfxVolumePct);
    }
}


// set volume as percent 0-1
void Audio::setVolumeMusic(const float musicVolumePct) {
    instance.getMusicValue = musicVolumePct;
    instance.musicVolumePct = logFunc(musicVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++) {
        instance.themes[i]->setVolume(instance.musicVolumePct);
    }
}


// set volume as percent 0-1
void Audio::setVolumeSfx(const float sfxVolumePct) {
    instance.getSFXValue = sfxVolumePct;
    instance.sfxVolumePct = logFunc(sfxVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    for (int i = 0; i < (int)Sfx::__COUNT; i++) {
        instance.effects[i]->setVolume(instance.sfxVolumePct);
    }
}

sf::Time Audio::getDurationMusic(const Soundtrack sd){
    return instance.themes[(int)sd]->getDuration();
}

sf::Time Audio::getDurationSfx(const Sfx sfx){
    return instance.effects[(int)sfx]->getDuration();
}



float Audio::getMusicVolume() {
    // return instance.musicVolumePct / (100.0f * VOLUME_MULTIPLIER);
    return instance.getMusicValue;
}

float Audio::getSfxVolume() {
    // instance.sfxVolumePct / (100.0f * VOLUME_MULTIPLIER);
    return instance.getSFXValue;
}
