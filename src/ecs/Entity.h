//
// Created by antho on 2026-01-21.
//

#ifndef TUTORIAL1_ENTITY_H
#define TUTORIAL1_ENTITY_H
#include <array>
#include <bitset>
#include <cstddef>

#include "ComponentType.h"

// Create a const that is evaluated at compile time
// 32 components fits perfectly in a 32-bit integer
constexpr std::size_t MAX_COMPONENTS = 128;

// bitset[0] = 1 // Entity has component 0
// Bit operations are fast, and work well for checking each frame
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

// Fixed size array, where each index is a component
// The index corresponds to the ComponentTypeID
// We use a fixed number of components, so there is no dynamic allocation for the array (efficient)
using ComponentArray = std::array<void*, MAX_COMPONENTS>;


class Entity {
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};
public:
    bool isActive() const {
        return active;
    }
    void destroy() {
        active = false;
    }

    // Templates need to be implemented in header
    // entity.hasComponent<Position>() -> returns true or false
    template <typename T>
    bool hasComponent() {
        return componentBitSet[getComponentTypeID<T>()];
    }

    // Template that can receive multiple arguments
    // entity.addComponent<Position>(4, 5)
    // entity.addComponent<Health>(100)
    // ... allows for multiple arguments
    template <typename T, typename... mArgs>
    T& addComponent(mArgs&&...args) {
        // Implements perfect forwarding
        // Everything is not treated as an lvalue
        // This is more efficient with moves and temporaries
        T* component(new T{std::forward<mArgs>(args)...});

        // Add the new component to the array
        componentArray[getComponentTypeID<T>()] = component;
        // Update the bitset to true
        componentBitSet[getComponentTypeID<T>()] = true;
        return *component;
    }

    template <typename T>
    T& getComponent() {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

    template <typename T>
    void deactivateComponent() {
        componentBitSet[getComponentTypeID<T>()] = false;
    }
};

#endif //TUTORIAL1_ENTITY_H
