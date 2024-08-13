#pragma once

#include <string>

enum class QuestType {
	Main,
	Secondary
};

class Quest {
public:
	Quest(QuestType questtype, const std::string& title, const std::string& text)
		: m_QuestType(questtype), m_Title(title), m_Text(text) {}
	
	std::string GetTitle() { return m_Title; }

private:
	QuestType m_QuestType;

	std::string m_Title;
	std::string m_Text;
};
