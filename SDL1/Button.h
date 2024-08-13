#pragma once

#include <SDL.h>

#include <functional>

class Button {
public:
	Button(int x, int y, int width, int height, const std::function<void()>& onClick, SDL_Texture* tex = nullptr);

	SDL_Rect GetRect() const { return m_ButtonRect; }
	void Update(const SDL_Event& e);
	void SetPosition(int x, int y) { m_ButtonRect.x = x; m_ButtonRect.y = y; };
	void Draw(SDL_Renderer* rend);
	
	inline void SetVisible(const bool v) { m_Visible = v; }

private:
	SDL_Rect m_ButtonRect;
	SDL_Texture* m_Tex;
	std::function<void()> m_OnClick;
	bool m_IsClicked = false;
	bool m_Visible = true;
};
