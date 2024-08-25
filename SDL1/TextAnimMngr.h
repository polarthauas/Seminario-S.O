#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "TextAnimation.h"
#include "TextAnimLetterByLetter.h"

enum TextAnimTypes {
	TEXT_ANIMATION_LETTERBYLETTER
};

namespace Text {

	struct ProprietsTextAnim {
		std::string text;
		SDL_Color color;
		int x;
		int y;
		TextAnimTypes animType;
	};

	class TextAnimMngr {
	public:
		TextAnimMngr() {}

		void addTextAnimation(const std::string& id, ProprietsTextAnim propriets, bool square = false, bool texture = false) {
			std::unique_ptr<Text::TextAnimation> _anim = nullptr;
			
			switch (propriets.animType)
			{
			case TEXT_ANIMATION_LETTERBYLETTER:
				//_anim = std::make_unique<LetterByLetter>(propriets.text, propriets.color, propriets.x, propriets.y, square, texture);
				break;
			default:
				break;
			}
			
			if (_anim == nullptr) return;

			_anim->startAnimation();

			m_TextsAnims[id] = std::move(_anim);
		}
	
		void dropTextAnimation(const std::string& id) {
			m_TextsAnims.erase(id);
		}
	
		void clean() {
			m_TextsAnims.clear();
		}


		void updateAll(std::shared_ptr<SoundMngr> soundMngr) {
			for (auto& b : m_TextsAnims) {
				//b.second->update(soundMngr);
			}
		}

		void renderAll(SDL_Renderer* rend, std::shared_ptr<MessageManager> msgManager) {
			for (auto& b : m_TextsAnims) {
				//b.second->render(rend, msgManager);
			}
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<Text::TextAnimation>> m_TextsAnims;
	};
}
