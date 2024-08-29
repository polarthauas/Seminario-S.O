#include "Globals.h"
#include "MessageManager.h"
#include "Notepad.h"

#include <SDl2/SDL.h>
#include <string>
#include <vector>

Notepad::Notepad(const SDL_Rect& r)
	: notepadRect({ (r.x + r.w) / 2, (r.y + r.h) / 4, 448, 499 }),
	cursorBlinkTime(SDL_GetTicks()) 
{
	cursorRect = { 0, 0, CURSOR_WIDTH, CURSOR_HEIGHT };

	noteText.emplace_back("TAREFAS:");
	noteText.emplace_back("PENDENTES:");
	noteText.emplace_back("Expulsar o Tauan e o Kayan da minha sala de POO");
	noteText.emplace_back("Pagar o agiota Pedrin");
	noteText.emplace_back("Achar a clinica de tratatamento contra a calvice na Turquia");
	noteText.emplace_back("Reformar minha mansao nos EUA");
	noteText.emplace_back("Pagar o agiota da PADARIA");
	noteText.emplace_back("CONCLUIDAS:");
	noteText.emplace_back("Tirar a ISA da minha sala de POO");
	noteText.emplace_back("OBS: Sério, eu não aguentava mais aquele tema branco");
	noteText.emplace_back("Construir a décima 8º mansão nos EUA");

	msgManager = std::make_unique<MessageManager>();
	if(!msgManager->setFont("../fonts/Roboto-Regular.ttf", 11)){
		SDL_Log("Erro ao carregar fonte");
	}
}

void Notepad::updateCursor() {
	cursorRect.x = notepadRect.x + 20 + Global::getTextWidth(noteText[linhaNote].substr(0, cursorPos), msgManager->getFont());
	cursorRect.y = notepadRect.y + 25 * (linhaNote+1) + 2;
}

void Notepad::render(SDL_Renderer* rend, const SDL_Rect& rCollision) {
	
	// Distância entre as linhas
	int offy = OFFY;

	SDL_RenderFillRect(rend, &notepadRect);

	// renderiza o X para sair

	SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);

	msgManager->render(rend, "NotePad FODA", { 255, 0, 0 }, notepadRect.x + 5, notepadRect.y + 2);

	SDL_RenderDrawLine(rend, rCollision.x, rCollision.y,
		rCollision.x + rCollision.w, rCollision.y + rCollision.h);
	SDL_RenderDrawLine(rend, rCollision.x + rCollision.w, rCollision.y,
		rCollision.x, rCollision.y + rCollision.h);

	// renderiza a linha divisória superior

	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderDrawLine(rend, notepadRect.x, notepadRect.y + 25,
		notepadRect.x + notepadRect.w, notepadRect.y + 25);

	// cor do notepad
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

	// renderiza o numero das linha
	for (uint32_t i = 1; i <= maxLinha; i++) {
		msgManager->render(rend, std::to_string(i), { 0, 0, 0 }, notepadRect.x + 5, notepadRect.y + offy * i);
	}

	for (const auto& s : noteText) {
		msgManager->render(rend, s, { 0, 0, 0 }, notepadRect.x + 20, notepadRect.y + offy);
		offy += OFFY;
	}

	// alternar visibilidade da setinha
	if (SDL_GetTicks() - cursorBlinkTime > 500) { // alterna a cada 500 ms
		cursorVisible = !cursorVisible;
		cursorBlinkTime = SDL_GetTicks();
	}

	// desenha a setinha
	if (cursorVisible) {
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderFillRect(rend, &cursorRect);
	}
}

void Notepad::Events(const SDL_Event& e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int x;
		int y;

		SDL_GetMouseState(&x, &y);

		if (Global::mouseInRect(x, y, notepadRect)) {
			isDraggingNotePad = true;
			dragStartX = x;
			dragStartY = y;
			notePadStartX = notepadRect.x;
			notePadStartY = notepadRect.y;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONUP && isDraggingNotePad) {
		isDraggingNotePad = false;
	}
	if (e.type == SDL_MOUSEMOTION && isDraggingNotePad) {
		int x;
		int y;

		SDL_GetMouseState(&x, &y);

		int deltaX = x - dragStartX;
		int deltaY = y - dragStartY;

		notepadRect.x = notePadStartX + deltaX;
		notepadRect.y = notePadStartY + deltaY;
	}

	if (e.type == SDL_TEXTINPUT && isTyping) {
		noteText[linhaNote].insert(cursorPos, e.text.text);
		cursorPos += strlen(e.text.text);
		updateCursor();
	}
	else if (e.type == SDL_KEYDOWN && isTyping) {
		switch (e.key.keysym.sym) {
		case SDLK_BACKSPACE:
			if (cursorPos > 0) {
				noteText[linhaNote].erase(cursorPos - 1, 1);
				cursorPos--;
			}
			else if (linhaNote > 0) {
				linhaNote--;
				cursorPos = noteText[linhaNote].length();
			}
			updateCursor();
			break;

		case SDLK_RETURN:
			linhaNote++;
			noteText.emplace_back("");
			cursorPos = 0;
			updateCursor();
			break;

		case SDLK_DOWN:
			linhaNote++;
			noteText.emplace_back("");
			updateCursor();
			break;

		case SDLK_UP:
			if (linhaNote > 0) {
				linhaNote--;
				cursorPos = noteText[linhaNote].length();
				updateCursor();
			}
			break;

		case SDLK_LEFT:
			if (cursorPos > 0) {
				cursorPos--;
				updateCursor();
			}
			break;

		case SDLK_RIGHT:
			if (cursorPos < noteText[linhaNote].length()) {
				cursorPos++;
				updateCursor();
			}
			break;
		}
	}
	if (linhaNote > maxLinha) maxLinha = linhaNote;
}

void Notepad::SetIsTyping(bool b) {
	isTyping = b;
}
