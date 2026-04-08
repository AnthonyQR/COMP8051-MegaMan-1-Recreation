//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_AUDIOMANAGER_H
#define TUTORIAL1_AUDIOMANAGER_H

#include <string>
#include <unordered_map>
#include "SDL3_mixer/SDL_mixer.h"

class AudioManager {
    struct clip {
        MIX_Audio* sound;
        int priority;
    };

    MIX_Mixer *mixer = nullptr;
    MIX_Track *musicTrack = nullptr;
    static MIX_Track *sfxTrack;
    static std::unordered_map<std::string, clip> audio;
    static std::string currentSfx;

public:
    AudioManager();

    void loadAudio(const std::string& name, const char* path, int priority) const;

    void playMusic (const std::string& name) const;
    void stopMusic() const;

    static void playSfx(const std::string& name);
    static void changeSfxVolume (const float volume);

    static void onSfxFinish(void *userdata, MIX_Track *track);
};

#endif //TUTORIAL1_AUDIOMANAGER_H