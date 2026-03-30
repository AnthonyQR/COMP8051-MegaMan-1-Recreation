//
// Created by antho on 2026-03-29.
//

#include "AudioManager.h"

#include <iostream>
#include <ostream>

MIX_Track* AudioManager::sfxTrack;
std::string AudioManager::currentSfx;
std::unordered_map<std::string, AudioManager::clip> AudioManager::audio;

AudioManager::AudioManager() {
    if (MIX_Init() == 0) {
        std::cout << "MIX_Init() failed" << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cout << "MIX_CreateMixerDevice() failed" << std::endl;
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);
    MIX_SetTrackGain(musicTrack, 0.1f);
    MIX_SetTrackGain(sfxTrack, 0.3f);
}

void AudioManager::loadAudio(const std::string& name, const char* path, int priority) const {
    if (audio.contains(path)) {
        return;
    }

    auto audioPtr = MIX_LoadAudio(mixer, path, true);
    if (!audioPtr) {
        std::cout << "MIX_LoadAudio() failed" << std::endl;
        return;
    }
    clip newClip{audioPtr, priority};
    audio.emplace(name, newClip);
}

void AudioManager::playMusic(const std::string& name) const {
    if (MIX_SetTrackAudio(musicTrack, audio[name].sound) == 0) {
        std::cout << "MIX_SetTrackAudio() failed" << std::endl;
        return;
    }
    SDL_PropertiesID props = MIX_GetTrackProperties(musicTrack);
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(musicTrack, props); // -1 means loop endlessly
    std::cout << "Playing music: " << name << std::endl;
}

void AudioManager::stopMusic() const {
    MIX_StopTrack(musicTrack, 0);
    std::cout << "Stopped music: " << musicTrack << std::endl;
}

void AudioManager::playSfx(const std::string &name) {
    // Check for priority first & return early if priority is too low
    if (audio.contains(currentSfx)) {
        if (audio[currentSfx].priority > audio[name].priority) return;
    }

    if (MIX_SetTrackAudio(sfxTrack, audio[name].sound) == 0) {
        std::cout << "MIX_SetTrackAudio() failed" << std::endl;
        return;
    }

    MIX_PlayTrack(sfxTrack, 0); // 0 means play once
    MIX_SetTrackStoppedCallback(sfxTrack, onSfxFinish, nullptr); // Execute function when sfx stops playing
    currentSfx = name;
    std::cout << "Playing sfx: " << name << std::endl;
}

void AudioManager::onSfxFinish(void *userdata, MIX_Track *track) {
    currentSfx = ""; // Set current sfx as empty
}
