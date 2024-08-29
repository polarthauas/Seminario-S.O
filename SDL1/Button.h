/*

	--------------
	Made by Thauan
	--------------

	Classe a qual representa um botão, só isso ksksks

	Também aqui estão os botões dinâmicos (Não, eu não criei outra classe apenas para eles)

*/

#pragma once

#include "Globals.h"
#include <functional>

class Button {
public:
	/*
	*
	* Cria um botão
	*
	* OBS: Todos os valores de x, y, w e h estão passando pelo resize
	*
	* @param x - posição x do botão
	* @param y - posição y do botão
	* @param width - tamanho horizontal do botão
	* @param height - tamanho vertical do botão
	* @param onClick - função a ser executada ao pressionar o botão
	*
	*
	*/
	Button(int x, int y, int width, int height, const std::function<void()>& onClick);

	~Button();

	inline bool loadTexture(SDL_Renderer* rend, const std::string& pathTex) { 
		m_Tex = IMG_LoadTexture(rend, pathTex.c_str());
		return m_Tex != nullptr;
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
	void update(const SDL_Event& e);

	/**
	* 
	* Troca a posição para um X e Y dinâmico, ou seja, deve-se posicionar conforme seu monitor que a classe se encarrega do redimensionamento
	*
	* @param x - Pos x
	* @param y - Pos y
	* 
	*/
	inline void setPosition(int x, int y, int w = 0, int h = 0) { 
		m_ButtonRect.x = x;
		m_ButtonRect.y = y;
		if (w > 0) m_ButtonRect.w = w;
		if (h > 0) m_ButtonRect.h = h;
	};
	
	bool draw(SDL_Renderer* rend);

	inline void resetPosition() {
		m_ButtonRect = m_OriginRect;
	}

	inline void resetHeight() {
		m_ButtonRect.h = m_OriginRect.h;
	}

	inline void SetClicable(bool c) { m_Clicable = c; }
	inline void SetVisible(bool v) { m_Visible = v; }

private:
	SDL_Rect m_ButtonRect;
	SDL_Rect m_OriginRect;
	SDL_Texture* m_Tex{ nullptr };
	std::function<void()> m_OnClick;
	bool m_IsClicked{ false };
	bool m_Visible{ true };
	bool m_Clicable{ true };
};
