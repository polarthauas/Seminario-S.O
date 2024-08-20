#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include <functional>

class Button {
public:
	Button(int x, int y, int width, int height, const std::function<void()>& onClick);
	
	inline bool loadTexture(SDL_Renderer* rend, const std::string& pathTex) { 
		m_Tex = IMG_LoadTexture(rend, pathTex.c_str());
		if (m_Tex == nullptr) return false;
		return true;
	}

	inline SDL_Rect GetRect() const { return m_ButtonRect; }
	inline SDL_Rect GetOriginRect() const { return m_OriginRect; }

	/*
	* 
	* Verifica se o botão foi pressionado, se foi, chama o m_OnClick;
	* 
	* @param e - Para analisar o evento de click
	* 
	*/
	void Update(const SDL_Event& e);

	/**
	* 
	* Troca a posição para um X e Y dinâmico, ou seja, deve-se posicionar conforme seu monitor que a classe se encarrega do redimensionamento
	*
	* @param x - Pos x
	* @param y - Pos y
	* 
	*/
	void SetPosition(int x, int y) { m_ButtonRect.x = x; m_ButtonRect.y = y; };
	void Draw(SDL_Renderer* rend);

	void ResetPosition();

	inline void SetClicable(const bool c) { m_Clicable = c; }
	inline void SetVisible(const bool v) { m_Visible = v; }

private:
	SDL_Rect m_ButtonRect;
	SDL_Rect m_OriginRect;
	SDL_Texture* m_Tex{ nullptr };
	std::function<void()> m_OnClick;
	bool m_IsClicked{ false };
	bool m_Visible{ true };
	bool m_Clicable{ true };
};
