#pragma once

#include <SDL.h>

#include <map>
#include <string>
#include "MessageManager.h"

#include <vector>

#include "Quest.h"

class QuestManager
{
public:
	QuestManager();

	~QuestManager();

	void Render(SDL_Renderer* rend);
	void RenderMoldure(SDL_Renderer* rend);
	void RenderQuests(SDL_Renderer* rend);

	void AddQuest(QuestType QuestTypem, const std::string& title, const std::string& desc);

	void Event(const SDL_Event& e);

	void DropQuest(std::string Title) {
		for (uint8_t i = 0; i < m_Quests.size(); ++i ) {
			if (m_Quests[i]->GetTitle() == Title) {
				delete m_Quests[i];
				m_Quests.erase(m_Quests.begin() + i);
			}
		}
	}

private:
	std::vector<Quest*> m_Quests;
	MessageManager* m_MsgManager{ nullptr };

	uint8_t countInitial{ 0 };
};

