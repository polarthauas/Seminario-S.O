#include "SoundMngr.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_log.h>

#include <thread>

void playSound(const std::string& path)
{
    std::thread([path]() {
        auto sound = Mix_LoadWAV(path.c_str());
        if (!sound) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erro ao carregar o som: %s", Mix_GetError());
            return;
        }

        // Reproduzir o som
        int channel = Mix_PlayChannel(-1, sound, 0);
        if (channel == -1) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erro ao reproduzir o som: %s", Mix_GetError());
            Mix_FreeChunk(sound);
            return;
        }

        // Esperar até que a reprodução do som termine
        while (Mix_Playing(channel)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Mix_FreeChunk(sound);

        }).detach();
}

void playMusic(const std::string& path)
{
    std::thread([path]() {
        auto music = Mix_LoadMUS(path.c_str());
        
        if (music == nullptr) {
            SDL_Log("Erro %s", Mix_GetError());
            return;
        }

        Mix_PlayMusic(music, 0);

        while (Mix_PlayingMusic()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Mix_FreeMusic(music);
           

        }).detach();
}

void pauseMusic()
{
    Mix_PauseMusic();
}

