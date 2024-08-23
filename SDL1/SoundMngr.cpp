#include "SoundMngr.h"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_log.h>

#include <thread>

SoundMngr::SoundMngr()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Erro ao iniciar o Mixer");
	}
}

SoundMngr::~SoundMngr()
{
	Mix_CloseAudio();
}

void SoundMngr::playSound(const std::string& path)
{
    std::thread([this, path]() {
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

        // Esperar at� que a reprodu��o do som termine
        while (Mix_Playing(channel)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Mix_FreeChunk(sound);

        }).detach();
}
