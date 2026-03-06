cmake_minimum_required(VERSION 4.1)
project(Tutorial1)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths

if (WIN32)
    set(SDL3_PATH "C:/Users/antho/OneDrive/Desktop/GameArchitecture/sdl3")
    set(SDL3_IMAGE_PATH "C:/Users/antho/OneDrive/Desktop/GameArchitecture/sdl3_image")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
    )
endif()

add_executable(Tutorial1 src/main.cpp
        src/Game.cpp
        src/TextureManager.cpp
        src/Map.cpp
        src/GameObject.cpp
        src/utils/Vector2D.cpp
        src/utils/Vector2DTest.cpp
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.cpp
        src/ecs/World.cpp
        src/vendor/tinyxml2.cpp
        src/manager/AssetManager.cpp
        src/manager/Scene.cpp
)

target_link_libraries(Tutorial1
        SDL3
        SDL3_image
)
