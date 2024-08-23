#include "QuestManager.h"

#include "Globals.h"

QuestManager::QuestManager()
{
	m_MsgManager = new MessageManager();

	m_MsgManager->setFont("../fonts/Minecraft_Seven_2.ttf", 25);
}

QuestManager::~QuestManager()
{
	for (auto& p : m_Quests) {
		delete p;
	}

	m_Quests.clear();
}

void QuestManager::Render(SDL_Renderer* rend)
{
	RenderMoldure(rend);
	RenderQuests(rend);
	
	m_MsgManager->render(rend, "Quests", { 255, 0, 0 }, Global::DESIGN_WIDTH / 2 - 50, Global::DESIGN_HEIGHT / 2 - 176);
}

void QuestManager::RenderQuests(SDL_Renderer* rend) {
	uint8_t auxCount{ 1 };

	for (uint8_t i = countInitial; i < countInitial + 3 && i < m_Quests.size(); ++i) {
		SDL_SetRenderDrawColor(rend, 0, 0, 250, 255);

		SDL_Rect questRect = Global::resizeRect({Global::DESIGN_WIDTH / 2 - 230, Global::DESIGN_HEIGHT / 2 - 75 - 55 * (2-auxCount), 400, 50});
		SDL_RenderFillRect(rend, &questRect);

		m_MsgManager->render(rend, m_Quests[i]->GetTitle(), {0, 255, 0}, Global::DESIGN_WIDTH / 2 - 200, Global::DESIGN_HEIGHT / 2 - 70 - (55 * (2 - auxCount)));
		auxCount++;
	}
}

void QuestManager::RenderMoldure(SDL_Renderer* rend)
{
	SDL_Rect mainRect = {Global::windowWidth / 2 - 300, Global::windowHeight / 2 - 200, 600, 300};
	SDL_SetRenderDrawColor(rend, 255, 215, 0, 255);
	SDL_RenderFillRect(rend, &mainRect);


	// Não necessita de redimencionamento no x e y, apenas no W e H!
	SDL_Rect questRect = { mainRect.x + 20, mainRect.y + 20, Global::resizeValue(560, Global::RESIZE_MODE_WIDTH), Global::resizeValue(260, Global::RESIZE_MODE_HEIGHT)};
	SDL_SetRenderDrawColor(rend, 128, 128, 128, 255);
	SDL_RenderFillRect(rend, &questRect);
}

void QuestManager::AddQuest(QuestType questType, const std::string& title, const std::string& desc)
{
	m_Quests.push_back(new Quest(questType, title, desc));
}

void QuestManager::Event(const SDL_Event& e)
{
	if (e.type == SDL_MOUSEWHEEL) {
		if (e.wheel.y < 0) {  
			if (countInitial < m_Quests.size() - 3 && m_Quests.size() > 3) {
				countInitial++;
			}
		}
		else if (e.wheel.y > 0) {  
			if (countInitial > 0) {
				countInitial--;
			}
		}
	}
}
