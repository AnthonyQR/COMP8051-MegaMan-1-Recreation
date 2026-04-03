//
// Created by antho on 2026-03-23.
//

#ifndef TUTORIAL1_ONDESTROYEVENT_H
#define TUTORIAL1_ONDESTROYEVENT_H

#include "World.h"

class OnDestroyEvent {
public:
    static void onDestroy(const DestroyedEvent& e, World& world);
};

#endif //TUTORIAL1_ONDESTROYEVENT_H