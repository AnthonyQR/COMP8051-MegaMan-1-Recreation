//
// Created by antho on 2026-03-29.
//

#ifndef TUTORIAL1_AUDIOEVENTQUEUE_H
#define TUTORIAL1_AUDIOEVENTQUEUE_H
#include <memory>
#include <queue>
#include <string>

#include "manager/AudioManager.h"

class AudioEvent {
public:
    explicit AudioEvent(const std::string &name) : name(name) {}

    void execute() const {
        AudioManager::playSfx(name);
    }

private:
    std::string name;
};

class AudioEventQueue {
public:
    void push(std::unique_ptr<AudioEvent> event) {
        events.push(std::move(event));
    }

    void process() {
        while (!events.empty()) {
            events.front()->execute();
            events.pop();
        }
    }

private:
    std::queue<std::unique_ptr<AudioEvent>> events;
};

#endif //TUTORIAL1_AUDIOEVENTQUEUE_H