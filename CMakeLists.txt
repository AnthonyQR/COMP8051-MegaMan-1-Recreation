cmake_minimum_required(VERSION 4.1)
project(Megaman)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths

if (WIN32)
    set(SDL3_PATH "C:/Users/antho/OneDrive/Desktop/GameArchitecture/sdl3")
    set(SDL3_IMAGE_PATH "C:/Users/antho/OneDrive/Desktop/GameArchitecture/sdl3_image")
    set(SDL3_MIXER_PATH "C:/Users/antho/OneDrive/Desktop/GameArchitecture/sdl3_mixer 3.1.0")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${SDL3_MIXER_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
            ${SDL3_MIXER_PATH}/lib
    )
endif()

add_executable(Megaman WIN32 src/main.cpp
        src/Game.cpp
        src/TextureManager.cpp
        src/Map.cpp
        src/GameObject.cpp
        src/utils/Vector2D.cpp
        src/utils/Vector2DTest.cpp
        src/utils/Collision.cpp
        src/ecs/system/ERS/CollisionSystem.cpp
        src/vendor/tinyxml2.cpp
        src/manager/AssetManager.cpp
        src/scene/Scene.cpp
        src/ecs/system/ERS/EventResponseSystem.cpp
        src/ecs/system/Scene/MainMenuSystem.cpp
        src/ecs/system/ERS/DestructionSystem.cpp
        src/ecs/system/ERS/OnDestroyEvent.cpp
        src/ecs/system/Combat/DamageSystem.cpp
        src/ecs/system/Scene/SceneTransitionDelaySystem.cpp
        src/manager/AudioManager.cpp
        src/scene/SpawnBeakEnemy.cpp
        src/scene/SpawnPlayer.cpp
        src/scene/SpawnBladerEnemy.cpp
        src/ecs/system/ERS/OnPlayerCollisionEvent.cpp
        src/ecs/system/ERS/OnEnemyCollisionEvent.cpp
        src/scene/SpawnOctopusBatteryEnemy.cpp
        src/scene/SpawnSuperCutterEnemy.cpp
        src/ecs/system/Player/DebugTeleportSystem.cpp
        src/ecs/system/Scene/UpdateSceneStateSystem.cpp
        src/scene/SpawnFlyingShellEnemy.cpp
        src/ecs/system/Player/PauseSystem.cpp
)

target_link_libraries(Megaman
        SDL3
        SDL3_image
        SDL3_mixer
)

if (WIN32)
    add_custom_command(TARGET Megaman POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:Megaman>/Assets
            COMMENT "Copying Assets to build output"
    )
endif ()
