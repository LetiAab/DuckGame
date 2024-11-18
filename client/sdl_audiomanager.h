#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <iostream>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Métodos para manejar la música de fondo
    bool loadMusic(const std::string& path);
    void playMusic(int loops = -1);
    void stopMusic();
    void setMusicVolume(int volume);  // 0 - 128

    // Métodos para manejar efectos de sonido
    bool loadSoundEffect(const std::string& path);
    void playSoundEffect(int loops = 0);
    void setSoundEffectVolume(int volume);  // 0 - 128

private:
    Mix_Music* backgroundMusic;  
    Mix_Chunk* soundEffect;   
};

#endif // AUDIOMANAGER_H
