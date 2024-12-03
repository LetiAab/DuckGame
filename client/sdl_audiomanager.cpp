#include "sdl_audiomanager.h"

AudioManager::AudioManager() 
    : backgroundMusic(nullptr), soundEffect(nullptr) {
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }
}

AudioManager::~AudioManager() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
    }
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
    }
    Mix_CloseAudio();
}

bool AudioManager::loadMusic(const std::string& path) {
    backgroundMusic = Mix_LoadMUS(path.c_str());
    if (!backgroundMusic) {
        std::cerr << "Error al cargar la música: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void AudioManager::playMusic(int loops) {
    if (backgroundMusic) {
        if (Mix_PlayMusic(backgroundMusic, loops) == -1) {
            std::cerr << "Error al reproducir la música: " << Mix_GetError() << std::endl;
        }
    }
}

void AudioManager::stopMusic() {
    if (Mix_PlayingMusic()) {
        Mix_HaltMusic();
    }
}

void AudioManager::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

bool AudioManager::isPlaying() {
    return Mix_PlayingMusic() && Mix_VolumeMusic(-1) > 0; //con -1 devuelve el volumen actual sin cambiarlo
}

bool AudioManager::loadSoundEffect(const std::string& path) {
    soundEffect = Mix_LoadWAV(path.c_str());
    if (!soundEffect) {
        std::cerr << "Error al cargar el efecto de sonido: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void AudioManager::playSoundEffect(int loops) {
    if (soundEffect) {
        if (Mix_PlayChannel(-1, soundEffect, loops) == -1) {
            std::cerr << "Error al reproducir el efecto de sonido: " << Mix_GetError() << std::endl;
        }
    }
}

void AudioManager::setSoundEffectVolume(int volume) {
    Mix_VolumeChunk(soundEffect, volume);
}
