#include "Computer.h"
#include "Globals.h"
#include "Notepad.h"

#include <vector>
#include <memory>
#include <tinyxml2.h>

Computer::Computer(SDL_Renderer* rend)
	: m_ComputerState("WORKSPACE1"), m_Rend(rend)
{
	printRect = { calcAlterWindowSize(20, 'w'), calcAlterWindowSize(20, 'h'), 
		windowWidth - calcAlterWindowSize(40, 'w'), windowHeight - calcAlterWindowSize(40, 'h') };

	screenListaAfaz = { (printRect.x + printRect.w) / 2, (printRect.y + printRect.h) / 4, 448, 499 };

	ParseXML();

	// Carrega as primeiras texturas
	_LoadTexture();
}

void Computer::_LoadTexture() {

	// Calcula a varia��o altX e altY por causa do tamanho da tela
	// [DEPRECATED]

	int altX = calcAlterWindowSize(20, 'w');
	int altY = calcAlterWindowSize(20, 'h');

	// Destr�i e limpa as texturas para serem colocadas novas
	for (auto& p : m_Textures) {
		SDL_DestroyTexture(p.Tex);
	}

	m_Textures.clear();

	// Alerta: MUITOS IF ELSE

	if (m_ComputerState == "WORKSPACE1") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WORKSPACE1"].c_str()), printRect);
	}
	else if (m_ComputerState == "INFIREFOXY1") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INFIREFOXY1"].c_str()), printRect);
	}
	else if (m_ComputerState == "INFIREFOXY2") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INFIREFOXY2"].c_str()), printRect);
	}
	else if (m_ComputerState == "BLUESCREEN") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["BLUESCREEN"].c_str()), printRect);
	}
	else if (m_ComputerState == "WORKSPACE2") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WORKSPACE2"].c_str()), printRect);
	}
	else if (m_ComputerState == "INSETTINGS1") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INSETTINGS1"].c_str()), printRect);
	}
	else if (m_ComputerState == "INWINDOWSDEFENDER1") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INWINDOWSDEFENDER1"].c_str()), printRect);
	}
	else if (m_ComputerState == "INWINDOWSDEFENDERVIRUS1") {

		// Cria a barra do windows

		SDL_Rect bar = { altX, windowHeight - altY - calcAlterWindowSize(42, 'h'), windowWidth - 2 * altX, calcAlterWindowSize(42, 'h')};

		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_BAR"].c_str()), bar);

		SDL_Rect rectLateral = { altX, altY, calcAlterWindowSize(320, 'w'), windowHeight - 2 * altY - bar.h};
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_DEFENDERLATERAL"].c_str()), rectLateral);
		SDL_Rect rectPrint = {altX+320, altY, windowWidth - calcAlterWindowSize(40, 'w') - 320, windowHeight - 2 * altY - bar.h};
		SDL_Rect srcPrint = { 0, 0, 1040, 728 };
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INWINDOWSDEFENDERVIRUS1"].c_str()), rectPrint, srcPrint, true, 1038);
	}
	else if (m_ComputerState == "INWINDOWSFIREWALL1") {
		SDL_Rect bar = { altX, windowHeight - altY - calcAlterWindowSize(42, 'h'), windowWidth - 2 * altX, calcAlterWindowSize(42, 'h')};
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_BAR"].c_str()), bar);

		SDL_Rect rectLateral = { altX, altY, calcAlterWindowSize(320, 'w'), windowHeight - 2 * altY - bar.h};
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_FIREWALL_LATERAL"].c_str()), rectLateral);

		SDL_Rect rectPrint = {altX+320, altY, windowWidth - calcAlterWindowSize(40, 'w') - 320, windowHeight - 2 * altY - bar.h};
		SDL_Rect srcPrint = { 0, 0, 1080, 728 };
		
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["INWINDOWSFIREWALL1"].c_str()), rectPrint, srcPrint, true, 1080);

	}
	else if (m_ComputerState == "RANSOMWER_MENU") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["RANSOMWER_MENU"].c_str()), printRect);
	}
	else if (m_ComputerState == "SEGURANCA_DISPOSITIVO_MENU") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["SEGURANCA_DISPOSITIVO_MENU"].c_str()), printRect);
	}
	else if (m_ComputerState == "PROTECAO_CONTAS_MENU") {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["PROTECAO_CONTAS_MENU"].c_str()), printRect);
	}
	// UFA! Finalmente acabou esses IF ELSE :)
}

void Computer::LoadNewScreen()
{
	_LoadTexture();
	_LoadButtons();
	//Foi carregado
	_LoadNewScreen = false;
}

void Computer::_LoadButtons()
{
	// Deleta e limpa os bot�pes
	for (auto& b : m_ButtonsMap ) {
		delete b.second;
	}

	m_ButtonsMap.clear();
	
	int alt = calcAlterWindowSize(20, 'w');
	
	// ALERTA: MUITOS IF E ELSE

	if (m_ComputerState == "WORKSPACE2") {
		m_ButtonsMap["LixeiraEnter"] = new Button(alt, alt, 75, 75, [this]() {
			_inLixeira = true; 
		});

		m_ButtonsMap["NotePadEnter"] = new Button(alt + 150, alt, 75, 75, [this]() {
			_inListaAfazeres = true;
			m_Notepad = std::make_unique<Notepad>(screenListaAfaz);
			m_Notepad->SetIsTyping(true);
		});

		m_ButtonsMap["SettingsEnter"] = new Button(alt, alt + 80, 75, 75, [this]() {
			_inLixeira = false;
			setState("INSETTINGS1");
		});

		m_ButtonsMap["SecretEnter"] = new Button(alt + 75, alt, 75, 75, [this]() {
			_inSecretPasta = true;
		});

	}
	else if (m_ComputerState == "INFIREFOXY1") {
		//No futuro haver� adi��o
	}

	else if (m_ComputerState == "INSETTINGS1") {
		m_ButtonsMap["WINDOWSDEFENDER_ENTER"] = new Button(printRect.x + 320, 155, 970, 65, [this]() {
			setState("INWINDOWSDEFENDER1");
		});

		m_ButtonsMap["WORKSPACE2_ENTER"] = new Button(printRect.x + printRect.w - 30, printRect.y, 30, 30, [this]() {
			setState("WORKSPACE2");
		});
	}
	else if (m_ComputerState == "INWINDOWSDEFENDER1") {
		m_ButtonsMap["WORKSPACE2_ENTER"] = new Button(printRect.x + printRect.w - 30, printRect.y, 30, 30, [this]() {
			setState("WORKSPACE2");
		});

		m_ButtonsMap["INWINDOWSDEFENDERVRIUS1_ENTER"] = new Button(printRect.x + 350, 220, 230, 290, [this]() {
			setState("INWINDOWSDEFENDERVIRUS1");
		});

		m_ButtonsMap["INWINDOWSFIREWALL1_ENTER"] = new Button(printRect.x + 830, 230, 220, 275, [this]() {
			setState("INWINDOWSFIREWALL1");
		});

		m_ButtonsMap["PROTECAO_CONTAS_ENTER"] = new Button(printRect.x + 585, 230, 220, 275, [this]() {
			setState("PROTECAO_CONTAS_MENU");
		});
	}
	else if (m_ComputerState == "INWINDOWSDEFENDERVIRUS1") {
		m_ButtonsMap["WORKSPACE2_ENTER"] = new Button(printRect.x + printRect.w - 30, printRect.y, 30, 30, [this]() {
			setState("WORKSPACE2");
		});
	}
	else if(m_ComputerState == "INWINDOWSFIREWALL1") {
		m_ButtonsMap["WORKSPACE2_ENTER"] = new Button(printRect.x + printRect.w - 30, printRect.y, 30, 30, [this]() {
			setState("WORKSPACE2");
		});
	}

	// Cabooo :)
}

void Computer::_MouseInComputer() {
	// Verificador simples

	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	if (x >= printRect.x && y >= printRect.y && x <= printRect.x + printRect.w &&
		y <= printRect.y + printRect.h) {
		SDL_ShowCursor(SDL_ENABLE);
	}
	else {
		SDL_ShowCursor(SDL_DISABLE);
	}
}

// Renderizadores:

void Computer::RenderTextures() {
	for (auto& p : m_Textures) {

		if (p.srcRect.w == 0) {
			SDL_RenderCopy(m_Rend, p.Tex, nullptr, &p.dstRect);
		}
		else {
			if (SDL_RenderCopy(m_Rend, p.Tex, &p.srcRect, &p.dstRect) != 0) {
				SDL_Log(SDL_GetError());
			}
		}

	}

	if (_inListaAfazeres) RenderNotePad();

	DrawButtons();

	if (_LoadNewScreen) {
		LoadNewScreen();
	}
}

// Cria a moldura do pc (a borda)
void Computer::RenderMoldure() {
	SDL_SetRenderDrawColor(m_Rend, 192, 192, 192, 255);
	SDL_Rect backgroundRect = { 0, 0, windowWidth, windowHeight };
	SDL_RenderFillRect(m_Rend, &backgroundRect);
}

// Renderizador geral
void Computer::Render() {
	RenderMoldure();
	RenderTextures();
}

void Computer::RenderNotePad(){
	SDL_Rect auxRect = m_Notepad->getRect();

	if (!m_ButtonsMap["NotePadExit"]) {
		m_ButtonsMap["NotePadExit"] = new Button(auxRect.x + auxRect.w - 20, auxRect.y + 5, 20, 20, [this]() {
		_inListaAfazeres = false;
		m_Notepad.reset();		
	});

	}else {
		m_ButtonsMap["NotePadExit"]->SetPosition(auxRect.x + auxRect.w - 20, auxRect.y + 5);
	}
	
	m_Notepad->Render(m_Rend, { auxRect.x + auxRect.w - 20, auxRect.y + 5, 20, 20 });
}

// Eventos:


// Gerencia o Comportamento do Scroll do Mouse
void Computer::_MouseWhell(const SDL_Event& e) {
	if (e.type == SDL_MOUSEWHEEL) {
		for (auto& p : m_Textures) {
			if (p.Scroll) {
				if (e.wheel.y < 0) {
					p.srcRect.y += SCROLL_SPEED;

					if (p.srcRect.y >= p.ScrollMax) p.srcRect.y = p.ScrollMax;
				}
				else if (e.wheel.y > 0) {
					p.srcRect.y -= SCROLL_SPEED;
					if (p.srcRect.y <= 0) p.srcRect.y = 0;
				}
			}
		}
	}
}

void Computer::Events(const SDL_Event& e) {
	_MouseInComputer();

	// Lista de Afazeres dhr
	if (_inListaAfazeres) {
		m_Notepad->Events(e);
	}
	
	if (m_ComputerState == "INSETTINGS1") { SDL_Log("Ok, ta indo"); }

	// Atualiza os bot�es
	for (auto& b : m_ButtonsMap) {
		b.second->Update(e);
	}	
	
	if (m_ComputerState == "INSETTINGS1") { SDL_Log("Ok, ta indo (depois)"); }
	
	_MouseWhell(e);
}

// O parse do XML

void Computer::ParseXML()
{
	tinyxml2::XMLDocument doc;

	if (doc.LoadFile("Textures.xml") != tinyxml2::XML_SUCCESS) {
		SDL_Log(doc.ErrorName());
		return;
	}

	tinyxml2::XMLElement* root = doc.RootElement();

	tinyxml2::XMLElement* ComputerElement = root->FirstChildElement("Computer");

	for (tinyxml2::XMLElement* e = ComputerElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		std::string id = e->Attribute("id");
		std::string path = e->Attribute("pathfile");

		m_PrintsMap[id] = path;
	}
}

