This project aims to authentically recreate the “Cutman” stage from the NES game “Mega Man 1” with a custom game engine.

The engine uses a combination of C++ and SDL, and uses the lecture material from COMP 8051 as a base. This a 2D engine with a graphics API automatically chosen by SDL based on the user’s system. 
The engine utilizes a pure Entity Component System (ECS) where:
- Entities are created (ex. Player, Enemies, Projectiles, Items, etc.)
- Components are added to entities (ex. Transform, Velocity, Gravity, Tags. etc.)
- Systems act on entities based on their components (ex. A movement system acts on entities with the transform & velocity components)

50+ components and 30+ systems were created for this game.

A deeper explanation of the engine's design & systems are included in the repository.
