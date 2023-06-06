
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
 * Implementation file of the module Audio
 */

#include "Audio.h"


// Declare the static instance of the class
Audio Audio::instance;



/**
 * Load a soundtrack from a file
 * @param sd is the index of the soundtrack to load
 * @param filename is the name of the file from where the soundtrack must be loaded
 */
void Audio::load(const Soundtrack sd, const std::string &filename) {
    std::unique_ptr<sf::Music> soundtrack = std::make_unique<sf::Music>();
    soundtrack->openFromFile(filename);
    instance.themes[(int)sd] = move(soundtrack);
}



/**
 * Load a sound effect from a file
 * @param sfx is the index of the sound effect to load
 * @param filename is the name of the file from where the sound effect must be loaded
 */
void Audio::load(const Sfx Sfx, const std::string &filename) {
    std::unique_ptr<sf::Music> effect = std::make_unique<sf::Music>();
    effect->openFromFile(filename);
    instance.effects[(int)Sfx] = move(effect);
}



/**
 * Default constructor
 */
Audio::Audio() {}



/**
 * Load all the soundtracks and sound effects
 * @param input is the module of the game where the audio is loaded
 */
void Audio::loadAll(Input& input){

    /*
     * Load all the soundtracks from the files
     */

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
    instance.load(Soundtrack::CIRCUIT_SELECTION, "Resources/Soundtracks/Circuit_Selection.ogg");
    instance.load(Soundtrack::LOADING, "Resources/Soundtracks/Loading.ogg");
    instance.load(Soundtrack::CREDITS, "Resources/Soundtracks/Credits.ogg");

    /*
     * Load all the sound effects from the files
     */

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
    instance.load(Sfx::RACE_START_ORIGINAL_MODE, "Resources/SoundEffects/Race_Start_Original_Mode.ogg");
    instance.load(Sfx::RACE_START_CONTINUOUS_MODE, "Resources/SoundEffects/Race_Start_Continuous_Mode.ogg");
    instance.load(Sfx::RACE_START_SURVIVAL_MODE, "Resources/SoundEffects/Race_Start_Survival_Mode.ogg");

    instance.load(Sfx::RACE_END_FIRST_ORIGINAL_MODE, "Resources/SoundEffects/Race_End_First_Original_Mode.ogg");
    instance.load(Sfx::RACE_END_SECOND_ORIGINAL_MODE, "Resources/SoundEffects/Race_End_Second_Original_Mode.ogg");
    instance.load(Sfx::RACE_END_THIRD_ORIGINAL_MODE, "Resources/SoundEffects/Race_End_Third_Original_Mode.ogg");

    instance.load(Sfx::RACE_END_FIRST_CONTINUOUS_MODE, "Resources/SoundEffects/Race_End_First_Continuous_Mode.ogg");
    instance.load(Sfx::RACE_END_SECOND_CONTINUOUS_MODE, "Resources/SoundEffects/Race_End_Second_Continuous_Mode.ogg");

    instance.load(Sfx::RACE_END_FIRST_SURVIVAL_MODE, "Resources/SoundEffects/Race_End_First_Survival_Mode.ogg");
    instance.load(Sfx::RACE_END_SECOND_SURVIVAL_MODE, "Resources/SoundEffects/Race_End_Second_Survival_Mode.ogg");

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
    instance.load(Sfx::CHECKPOINT_VOICE_FIRST, "Resources/SoundEffects/Checkpoint_Voice_First.ogg");
    instance.load(Sfx::CHECKPOINT_VOICE_SECOND, "Resources/SoundEffects/Checkpoint_Voice_Second.ogg");
    instance.load(Sfx::CHECKPOINT_VOICE_THIRD, "Resources/SoundEffects/Checkpoint_Voice_Third.ogg");

    instance.load(Sfx::SCORE_BONUS, "Resources/SoundEffects/Score_Bonus.ogg");

    instance.load(Sfx::HALL_FAME, "Resources/SoundEffects/Hall_Fame.ogg");
    instance.load(Sfx::WAVE, "Resources/SoundEffects/Wave.ogg");
    instance.load(Sfx::WIND, "Resources/SoundEffects/Wind.ogg");
    instance.load(Sfx::GAME_OVER, "Resources/SoundEffects/Game_Over.ogg");
    instance.load(Sfx::COUNTDOWN, "Resources/SoundEffects/Countdown.ogg");

    instance.load(Sfx::BIOMES_LOADED_WELL, "Resources/SoundEffects/Biomes_Loaded_Well.ogg");

    // Check first of all if there is a configuration file for the game
    if (input.exitsSettings){
        // Get the values and store them
        if ((input.volumeMusic % 5 == 0 && input.volumeMusic >= 0 && input.volumeMusic <= 100) &&
            (input.volumeEffects % 5 == 0 && input.volumeEffects >= 0 && input.volumeEffects <= 100))
        {
            Audio::setVolume(0.01f * input.volumeMusic, 0.01f * input.volumeEffects);
        }
    }
    else
        // There is not a settings file and establish a default configuration
        Audio::setDefaultinput();
}



/**
 * Set the default configuration to reproduce the audio
 */
void Audio::setDefaultinput(){
    // Set default values to the volumes in both scales
    instance.musicVolumePct = logFunc(1.f) * 100.0;
    instance.sfxVolumePct = logFunc(1.f) * 100.0;
    instance.getMusicValue = 1.f;
    instance.getSFXValue = 1.f;
}



/**
 * Play a soundtrack of the game
 * @param sd is the index of the soundtrack in the playlist
 * @param loop is a flag to control if the soundtrack must be played infinitely or not
 */
void Audio::play(const Soundtrack sd, bool loop) {
    // Stop the soundtrack in order to not be played multiple times
    stop(sd);
    instance.themes[(int)sd]->play();
    instance.themes[(int)sd]->setLoop(loop);
    instance.themes[(int)sd]->setVolume(instance.musicVolumePct);
}



/**
 * Play a sound effect of the game
 * @param sfx is the index of the sound effect in the playlist
 * @param loop is a flag to control if the sound effect must be played infinitely or not
 */
void Audio::play(const Sfx sfx, bool loop) {
    // Check if the sfx is current being played to not reproduce twice
    if (instance.effects[(int)sfx]->getStatus() != sf::SoundSource::Paused)
        stop(sfx);

    instance.effects[(int)sfx]->play();
    instance.effects[(int)sfx]->setLoop(loop);
    instance.effects[(int)sfx]->setVolume(instance.sfxVolumePct);
}



/**
 * Play a soundtrack of the game
 * @param index is the index of the soundtrack in the playlist
 * @param loop is a flag to control if the soundtrack must be played infinitely or not
 */
void Audio::play(const int index, bool loop) {
    // Check if the soundtrack is current being played to not reproduce twice
    if (instance.themes[index]->getStatus() != sf::SoundSource::Paused)
        stop(index);

    instance.themes[index]->play();
    instance.themes[index]->setLoop(loop);
    instance.themes[index]->setVolume(instance.musicVolumePct);
}



/**
 * Check if a soundtrack is being played or not
 * @param sd is the index of the soundtrack in the playlist
 * @return true if the soundtrack is being played. Otherwise returns false
 */
bool Audio::isPlaying(const Soundtrack sd) {
    bool playing;
    playing = instance.themes[(int)sd]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}



/**
 * Check if a sound effect is being played or not
 * @param sfx is the index of the sound effect in the playlist
 * @return true if the sound effect is being played. Otherwise returns false
 */
bool Audio::isPlaying(const Sfx Sfx) {
    bool playing;
    playing = instance.effects[(int)Sfx]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}



/**
 * Check if a soundtrack is being played or not
 * @param index is the index of the soundtrack in the playlist
 * @return true if the soundtrack is being played. Otherwise returns false
 */
bool Audio::isPlaying(const int index){
    bool playing;
    playing = instance.themes[index]->getStatus() == sf::SoundSource::Status::Playing;
    return playing;
}



/**
 * Stop a soundtrack of the game that is being played
 * @param sd is the index of the soundtrack in the playlist
 */
void Audio::stop(const Soundtrack sd) {
    instance.themes[(int)sd]->stop();
}



/**
 * Stop a sfx of the game that is being played
 * @param sfx is the index of the sfx in the playlist
 */
void Audio::stop(const Sfx Sfx) {
    instance.effects[(int)Sfx]->stop();
}



/**
 * Stop a soundtrack of the game that is being played
 * @param index is the index of the soundtrack in the playlist
 */
void Audio::stop(const int index) {
    instance.themes[index]->stop();
}



/**
 * Pause a soundtrack of the game that is being played
 * @param sd is the index of the soundtrack in the playlist
 */
void Audio::pause(const Soundtrack sd) {
    if (instance.themes[(int)sd]->getStatus() == sf::SoundSource::Status::Playing){
        instance.themes[(int)sd]->pause();
    }
}



/**
 * Pause a sfx of the game that is being played
 * @param sfx is the index of the sfx in the playlist
 */
void Audio::pause(const Sfx Sfx) {
    if (instance.effects[(int)Sfx]->getStatus() == sf::SoundSource::Status::Playing){
        instance.effects[(int)Sfx]->pause();
    }
}



/**
 * Pause a soundtrack of the game that is being played
 * @param index is the index of the soundtrack in the playlist
 */
void Audio::pause(const int index) {
    if (instance.themes[index]->getStatus() == sf::SoundSource::Status::Playing){
        instance.themes[index]->pause();
    }
}



/**
 * Check all the sfx that can be played during a game round and if there is
 * anyone being played is pause
 */
void Audio::pauseSfxForReanudate(){
    // Loop of all the playable sound effects in a game round
    for (int sfxInt = (int)Sfx::RACE_SEMAPHORE_PREPARE; sfxInt < (int)Sfx::__COUNT; sfxInt++){
        Sfx sfx = static_cast<Sfx>(sfxInt);
        // If it is being played it is paused
        if (isPlaying(sfx))
            pause(sfx);
    }
}



/**
 * Check all the sfx that are paused and play them
 * anyone being played is pause
 */
void Audio::reanudateSfxPaused(){
    // Iterate the sound effects and play which are paused
    for (int i = 0; i < (int)Sfx::__COUNT; i++){
        if (instance.effects[i]->getStatus() == sf::SoundSource::Status::Paused){
            Sfx sfx = static_cast<Sfx>(i);
            play(sfx, false);
        }
    }
}



/**
 * Stop all the soundtracks of the game
 */
void Audio::stopMusic() {
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++) {
        instance.themes[i]->stop();
    }
}



/**
 * Stop all the sound effects of the game
 */
void Audio::stopSfx() {
    for (int i = 0; i < (int)Sfx::__COUNT; i++){
        instance.effects[i]->stop();
    }
}



/**
 * Module the new value in scale (0..1) using logarithmic scale to compute the volume
 * @param value is the level volume in scale (1..100)
 * @return the volume value in logarithmic scale
 */
float Audio::logFunc(const float value) {
    float ret = -log10f(powf(1 - value * 0.9f, VOLUME_LOG_EXP));
    if (ret > 1.0f) {
        ret = 1.0f;
    }
    return ret;
}



/**
 * Set the volumes of the soundtracks and sfx of the game in scale (0..100)
 * @param musicVolumePct is the volume of the soundtrack in scale (0..1)
 * @param sfxVolumePct is the volume of the soundtrack in scale (0..1)
 */
void Audio::setVolume(const float musicVolumePct, const float sfxVolumePct) {
    // Get the current values
    instance.getMusicValue = musicVolumePct;
    instance.getSFXValue = sfxVolumePct;

    // Compute the values using logarithmic scale to convert from scale (0..1) to (0..100)
    instance.musicVolumePct = logFunc(musicVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    instance.sfxVolumePct = logFunc(sfxVolumePct) * 100.0f * VOLUME_MULTIPLIER;

    // Set the soundtracks with the new volume value
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++) {
        instance.themes[i]->setVolume(instance.musicVolumePct);
    }

    // Set the sound effects with the new volume value
    for (int i = 0; i < (int)Sfx::__COUNT; i++) {
        instance.effects[i]->setVolume(instance.sfxVolumePct);
    }
}



/**
 * Set the volume of the soundtracks in scale (0..100)
 * @param musicVolumePct is the volume of the soundtrack in scale (0..1)
 */
void Audio::setVolumeMusic(const float musicVolumePct) {
    // Get the current value of the soundtracks volume
    instance.getMusicValue = musicVolumePct;

    /*
     * Set the new value of the volume in scale (0.100) using logarithmic scale
     * and set the soundtracks with it
     */

    instance.musicVolumePct = logFunc(musicVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    for (int i = 0; i < (int)Soundtrack::__COUNT; i++)
        instance.themes[i]->setVolume(instance.musicVolumePct);
}



/**
 * Set the volumes of the sfx of the game in scale (0..100)
 * @param sfxVolumePct is the volume of the soundtrack in scale (0..1)
 */
void Audio::setVolumeSfx(const float sfxVolumePct) {
    // Get the current value of the sfx volume
    instance.getSFXValue = sfxVolumePct;

    /*
     * Set the new value of the volume in scale (0.100) using logarithmic scale
     * and set the sound effects with it
     */

    instance.sfxVolumePct = logFunc(sfxVolumePct) * 100.0f * VOLUME_MULTIPLIER;
    for (int i = 0; i < (int)Sfx::__COUNT; i++)
        instance.effects[i]->setVolume(instance.sfxVolumePct);
}



/**
 * Get the duration of a soundtrack
 * @param sd is the index of the soundtrack in the playlist
 */
sf::Time Audio::getDurationMusic(const Soundtrack sd){
    return instance.themes[(int)sd]->getDuration();
}



/**
 * Get the duration of a sfx
 * @param sfx is the index of the sound effect in the playlist
 */
sf::Time Audio::getDurationSfx(const Sfx sfx){
    return instance.effects[(int)sfx]->getDuration();
}


/**
 * Get the soundtrack volume
 */
float Audio::getMusicVolume() {
    return instance.getMusicValue;
}



/**
 * Get the sfx volume
 */
float Audio::getSfxVolume() {
    return instance.getSFXValue;
}
