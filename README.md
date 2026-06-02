This project aims to authentically recreate the “Cutman” stage from the NES game “Mega Man 1” with a custom game engine.

The engine uses a combination of C++ and SDL, and uses the lecture material from COMP 8051 as a base. This a 2D engine with a graphics API automatically chosen by SDL based on the user’s system. 
The engine utilizes a pure Entity Component System (ECS) where:
- Entities are created (ex. Player, Enemies, Projectiles, Items, etc.)
- Components are added to entities (ex. Transform, Velocity, Gravity, Tags. etc.)
- Systems act on entities based on their components (ex. A movement system acts on entities with the transform & velocity components)

50+ components and 30+ systems were created for this game.

A deeper explanation of the engine's design & systems are included in the repository.

Controls:
- Arrow Keys: Movement
- Z: Jump
- X: Shoot
- Enter: Start / Pause

Special Thanks:
- Charles Tapp, for teaching us & giving us the opportunity to create the projects for this course
- Mister Mike, who created the spritesheets that was used for this project
    - https://www.spriters-resource.com/profile/mistermike/
- Chris Rovell, who ripped the music that was used for this project
    - https://www.zophar.net/music/nintendo-nes-nsf/mega-man
- Nai255, who ripped the SFX that was used for this project
    - https://sounds.spriters-resource.com/profile/nai255/
