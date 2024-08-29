#include "Computer.h"
#include "Globals.h"
#include "Notepad.h"
#include "TextureMngr.h"
#include "ButtonMngr.h"
#include "MessageManager.h"
#include "SoundMngr.h"
#include "TextAnimLetterByLetter.h"
#include "FadeIn.h"

#include "Game.h"

#include "CONFIGS.h"

#include <tinyxml2.h>

Computer::Computer(SDL_Renderer* rend, std::shared_ptr<TextureMngr> texturemngr, std::shared_ptr<ButtonMngr> buttonmngr, std::shared_ptr<MessageManager> msgMngr)
	: m_ComputerState(WORKSPACE1), m_Rend(rend), m_TextureMngr(texturemngr), m_ButtonMngr(buttonmngr), m_MsgManager(msgMngr)
{
	printRect = { Global::resizeValue(20, Global::RESIZE_MODE_WIDTH), Global::resizeValue(20, Global::RESIZE_MODE_HEIGHT), 
		Global::windowWidth - Global::resizeValue(40, Global::RESIZE_MODE_WIDTH),
		Global::windowHeight - Global::resizeValue(40, Global::RESIZE_MODE_HEIGHT) };

	screenListaAfaz = { (printRect.x + printRect.w) / 2, (printRect.y + printRect.h) / 4, 448, 499 };

	ParseXML();

	// Carrega as primeiras texturas
	m_LoadTexture();
	m_MsgManager->setFontSize(28);
	m_ButtonMngr->clean();
}

void Computer::m_LoadTexture() {
	// Destrói e limpa as texturas para serem colocadas novas
	for (auto& p : m_Textures) {
		SDL_DestroyTexture(p.Tex);
	}

	m_Textures.clear();

	m_TextureMngr->clean();

	// Alerta: MUITOS IF ELSE

	if (m_ComputerState == WORKSPACE1) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[WORKSPACE1].c_str()), printRect);
	}
	else if (m_ComputerState == INFIREFOX1) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[INFIREFOX1].c_str()), printRect);
	}
	else if (m_ComputerState == INFIREFOX2) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[INFIREFOX2].c_str()), printRect);
	}
	else if (m_ComputerState == INBLUESCREEN) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[INBLUESCREEN].c_str()), printRect);
	}
	else if (m_ComputerState == WORKSPACE2) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[WORKSPACE2].c_str()), printRect);
	}
	else if (m_ComputerState == SETTINGS1) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[SETTINGS1].c_str()), printRect);
	}
	else if (m_ComputerState == WINDOWSDEFENDER1) {
		_LoadWindowsBar();
		_LoadDefenderLateral();
		
		SDL_Rect rectPrint = Global::resizeRect({BORDER_SIZE+310, BORDER_SIZE, Global::DESIGN_WIDTH - 350, Global::DESIGN_HEIGHT - 2 * BORDER_SIZE - WINDOWSBAR_SIZE});

		SDL_Rect srcPrint = { 0, 0, 1080, 728 };
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[WINDOWSDEFENDER1].c_str()), rectPrint, srcPrint, true, 845);


	}
	else if (m_ComputerState == WINDOWSDEFENDERVIRUS1) {
		_LoadWindowsBar();
		_LoadDefenderLateral();

		SDL_Rect rectPrint = Global::resizeRect({BORDER_SIZE+310, BORDER_SIZE, Global::DESIGN_WIDTH - 350, Global::DESIGN_HEIGHT - 2 * BORDER_SIZE - WINDOWSBAR_SIZE});

		SDL_Rect srcPrint = { 0, 0, 1040, 728 };
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[WINDOWSDEFENDERVIRUS1].c_str()), rectPrint, srcPrint, true, 1038);
	}
	else if (m_ComputerState == WINDOWSFIREWALL1) {
		_LoadWindowsBar();
		_LoadDefenderLateral();

		SDL_Rect rectPrint = Global::resizeRect({BORDER_SIZE+310, BORDER_SIZE, Global::DESIGN_WIDTH - 350, Global::DESIGN_HEIGHT - 2 * BORDER_SIZE - WINDOWSBAR_SIZE});
		SDL_Rect srcPrint = { 0, 0, 1080, 728 };
		
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[WINDOWSFIREWALL1].c_str()), rectPrint, srcPrint, true, 1080);

	}
	else if (m_ComputerState == RANSOMWERE_MENU) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[RANSOMWERE_MENU].c_str()), printRect);
	}
	else if (m_ComputerState == SEGURANCA_DISPOSITIVO_MENU) {
		auto tex = IMG_LoadTexture(m_Rend, "Assets/Game/prints/Seguranca_Dispositivo.png");
		m_Textures.emplace_back(tex, printRect);
	}
	else if (m_ComputerState == PROTECAO_CONTAS_MENU) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[PROTECAO_CONTAS_MENU].c_str()), printRect);
	}
	else if (m_ComputerState == OPCOES_FAMILIA_MENU) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[m_ComputerState].c_str()), printRect);
	}
	else if (m_ComputerState == CONFIGURACOES_ANTIVIRUS_MENU) {
		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[m_ComputerState].c_str()), printRect);
	}
	else if (m_ComputerState == CONFIGURACOES_SEG_DISPOSITIVO) {
		_LoadWindowsBar();
		_LoadDefenderLateral();

		SDL_Rect rectPrint = Global::resizeRect({ BORDER_SIZE + 310, BORDER_SIZE, Global::DESIGN_WIDTH - 350, Global::DESIGN_HEIGHT - 2 * BORDER_SIZE - WINDOWSBAR_SIZE });
		SDL_Rect srcPrint = { 0, 0, 1080, 728 };

		m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap[CONFIGURACOES_SEG_DISPOSITIVO].c_str()), rectPrint, srcPrint, true, 954);

	}

	// UFA! Finalmente acabou esses IF ELSE :)
}

inline void Computer::_LoadDefenderLateral() {
	SDL_Rect rectLateral = Global::resizeRect({ BORDER_SIZE, BORDER_SIZE, 310, Global::DESIGN_HEIGHT - 2 * BORDER_SIZE - WINDOWSBAR_SIZE });
	m_Textures.emplace_back(IMG_LoadTexture(m_Rend, m_PrintsMap["WINDOWS_FIREWALL_LATERAL"].c_str()), rectLateral);
}

inline void Computer::_LoadExitBtn()
{
	auto button1 = std::make_unique<Button>(printRect.x + printRect.w - 30, printRect.y, 30, 30, [this]() {
		setState(WORKSPACE2);
		});

	m_ButtonMngr->addButton("workspace2Enter", std::move(button1));
}

inline void Computer::_LoadDefenderLateralBtns()
{
	auto button1 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 120, 310, 30, [this]() {
		setState(WINDOWSDEFENDER1);
	});

	auto button2 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 155, 310, 30, [this]() {
		setState(WINDOWSDEFENDERVIRUS1);
		});

	auto button3 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 195, 310, 30, [this]() {
		setState(PROTECAO_CONTAS_MENU);
		});

	auto button4 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 235, 310, 30, [this]() {
		setState(WINDOWSFIREWALL1);
		});

	auto button5 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 275, 310, 30, [this]() {});

	auto button6 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 310, 310, 30, [this]() {
		setState(SEGURANCA_DISPOSITIVO_MENU);
		});

	auto button7 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 350, 310, 30, [this]() {});

	auto button8 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 390, 310, 30, [this]() {
		setState(OPCOES_FAMILIA_MENU);
		});


	auto button9 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 650, 310, 30, [this]() {
		setState(CONFIGURACOES_ANTIVIRUS_MENU);
		});

	m_ButtonMngr->addButton("Lateral_WindowsDefenderEnter", std::move(button1));
	m_ButtonMngr->addButton("Lateral_windowsdefenderVirusEnter", std::move(button2));
	m_ButtonMngr->addButton("Lateral_proteContasEnter", std::move(button3));
	m_ButtonMngr->addButton("Lateral_FirewallEnter", std::move(button4));
	m_ButtonMngr->addButton("Lateral_controleApp&Navegador", std::move(button5));
	m_ButtonMngr->addButton("Lateral_SegDispoEnter", std::move(button6));
	m_ButtonMngr->addButton("Lateral_desempenhoIntegridade", std::move(button7));
	m_ButtonMngr->addButton("Lateral_opcoesFamilia", std::move(button8));
	m_ButtonMngr->addButton("Lateral_configsEnter", std::move(button9));
}

inline void Computer::m_CleanButtonMap() {
	m_ButtonMngr->clean();
}

void Computer::m_LoadButtons()
{
	m_ButtonMngr->cleanNow();

	// ALERTA: MUITOS IF E ELSE

	if (m_ComputerState == WORKSPACE2) {

		auto button1 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE, 75, 75, [this]() {
			m_inLixeira = true; 
		});

		auto button2 = std::make_unique<Button>(BORDER_SIZE + 150, BORDER_SIZE, 75, 75, [this]() {
			m_inListaAfazeres = true;
			m_Notepad = std::make_unique<Notepad>(screenListaAfaz);
			m_Notepad->SetIsTyping(true);
		});

		auto button3 = std::make_unique<Button>(BORDER_SIZE, BORDER_SIZE + 80, 75, 75, [this]() {
			m_inLixeira = false;
			setState("INSETTINGS1");
		});

		auto button4 = std::make_unique<Button>(BORDER_SIZE + 75, BORDER_SIZE, 75, 75, [this]() {
			if (m_inSecretPasta) return;
			
			m_inSecretPasta = true;

			playMusic("Audio/Douglas_rico.ogg");

			auto messgae1 = std::make_unique<Text::LetterByLetter>("OH NÃO, MEU SEGREDO MAIS PRECIOSO E ALARMANTE!", SDL_BLUE, 500, 400);

			m_MsgManager->addDialogBox(std::move(messgae1));

			m_TextureMngr->loadTex("douglasRico", "Assets/Douglas/douglasRico.png");
		});
		
		m_ButtonMngr->addButton("enterLixeira", std::move(button1));
		m_ButtonMngr->addButton("enterNotepad", std::move(button2));
		m_ButtonMngr->addButton("enterSecretFolder", std::move(button4));
		m_ButtonMngr->addButton("enterTrash", std::move(button3));

	}
	else if (m_ComputerState == SETTINGS1) {
		_LoadExitBtn();

		if(!findLocal(SETTINGS1)){
			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Essa é a primeira tela de configurações na seção configurações e privacidade", "permite que você controle as permissões dos APPS", "histórico, controle de voz, entre no windows defender, etc..."}, SDL_PURPLE, 350, 50);

			m_MsgManager->addDialogBox(std::move(message1));

			m_LocaisJaVisitados.push_back(SETTINGS1);
		}
		
		auto button1 = std::make_unique<Button>(printRect.x + 320, 155, 970, 65, [this]() {
			setState(WINDOWSDEFENDER1);
		});
		
		m_ButtonMngr->addButton("windowsDefenderEnter", std::move(button1));	
	}
	else if (m_ComputerState == WINDOWSDEFENDER1) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(WINDOWSDEFENDER1)) {
			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Essa é a tela geral do windows defender", "Você poderá configurar o sistema de DEFESA padrão do WINDOWS"}, SDL_BLACK, 365, 50);

			m_MsgManager->addDialogBox(std::move(message1));

			m_LocaisJaVisitados.push_back(WINDOWSDEFENDER1);
		}

		auto button1 = std::make_unique<Button>(printRect.x + 350, 220, 230, 300, [this]() {
			setState("INWINDOWSDEFENDERVIRUS1");
		});

		auto button3 = std::make_unique<Button>(printRect.x + 580, 220, 230, 300, [this]() {
			setState(PROTECAO_CONTAS_MENU);
		});

		auto button2 = std::make_unique<Button>(printRect.x + 810, 220, 230, 300, [this]() {
			setState(WINDOWSFIREWALL1);
		});

		auto button4 = std::make_unique<Button>(printRect.x + 1040, 220, 230, 300, [this]() {});

		auto button5 = std::make_unique<Button>(printRect.x + 350, 540, 230, 280, [this]() {
			setState(SEGURANCA_DISPOSITIVO_MENU);
			});

		auto button6 = std::make_unique<Button>(printRect.x + 580, 540, 230, 280, [this]() {});

		auto button7 = std::make_unique<Button>(printRect.x + 810, 540, 230, 280, [this]() {});

		auto button8 = std::make_unique<Button>(printRect.x + 1040, 540, 230, 280, [this]() {});

		m_ButtonMngr->addButton("windowsDefenderEnter", std::move(button1));
		m_ButtonMngr->addButton("fireWallEnter", std::move(button2));
		m_ButtonMngr->addButton("protecaoContasEnter", std::move(button3));
		m_ButtonMngr->addButton("controleApp&NavegadorEnter", std::move(button4));
		m_ButtonMngr->addButton("segurancaDispositivoEnter", std::move(button5));
		m_ButtonMngr->addButton("desempenho&IntegridadeEnter", std::move(button6));
		m_ButtonMngr->addButton("opcoesDeFamiliaEnter", std::move(button7));
		m_ButtonMngr->addButton("historicoDeProtecaoEnter", std::move(button8));

	}
	else if (m_ComputerState == WINDOWSDEFENDERVIRUS1) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(WINDOWSDEFENDERVIRUS1)) {
			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Aqui é onde podemos fazer verificações de ameaça no S.O utilizando-se do windows defender", "Tal verificação utiliza IA, segundo a documentação do WINDOWS, para identficar ameaças", "Aqui também podemos ativar a proteção contra RANSOMWARE"}, SDL_BLUE, 300, 50);

			m_MsgManager->addDialogBox(std::move(message1));

			m_LocaisJaVisitados.push_back(WINDOWSDEFENDERVIRUS1);
		}

		auto button1 = std::make_unique<Button>(printRect.x + 350, 999, 240, 20, [this]() {
			setState(RANSOMWERE_MENU);
		});

		m_ButtonMngr->addButton("ransomwererEnter", std::move(button1));
	}
	else if (m_ComputerState == WINDOWSFIREWALL1) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		auto button1 = std::make_unique<Button>(BORDER_SIZE + 335, 495, 100, 40, [this]() {});

		m_ButtonMngr->addButton("redeDominioEnter", std::move(button1));
	}
	else if (m_ComputerState == RANSOMWERE_MENU) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(RANSOMWERE_MENU)) {

			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Aqui é bem simples, podemos ativar ou desativar a proteção contra o RANSOMWARE", "E o que é RANSOMWARE?", "Segundo a documentação do windows:", "'Ransomware é um malware que criptografa seus arquivos ou impede", "que você use o computador até que você pague(um resgate) para que eles sejam desbloqueados' "}, SDL_BLUE, 350, 50);

			auto message2 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"O acesso controlado à pasta protege seus dados que estão em pastas protegidas", "verificiando aplicativos em uma lista de aplicativos confiáveis e conhecidos do windows"}, SDL_RED, 350, 50);

			m_MsgManager->addDialogBox(std::move(message1));
			m_MsgManager->addDialogBox(std::move(message2));

			m_LocaisJaVisitados.push_back(RANSOMWERE_MENU);
		}

	}
	else if (m_ComputerState == PROTECAO_CONTAS_MENU) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(PROTECAO_CONTAS_MENU)) {

			auto message1 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"Aqui é trabalho de outro grupo, mas enfim", "Um breve resumo, é para configurar o windows Hello(enrtrar com digital, reconhecimento facial, etc)"},
				SDL_BLACK, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX
			);

			m_MsgManager->addDialogBox(std::move(message1));

			m_LocaisJaVisitados.push_back(PROTECAO_CONTAS_MENU);
		}
	}
	else if (m_ComputerState == SEGURANCA_DISPOSITIVO_MENU) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(SEGURANCA_DISPOSITIVO_MENU)) {

			auto message1 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"Essa é uma das partes mais importantes de segurça em um S.O", "Imagine, o que é pior do que ter o núcleo do S.O contaminado?", "Ou um vírus que quando você liga o windows ele inicia a nível de kernal", "Ou um programa malicioso modificando os valores na memória, alterando dados e comprometendo o S.O"}, SDL_PURPLE, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX);

			auto message2 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"É por isso que existe uma grande segurança na parte do KERNAL e memória no Windows"},
				SDL_BLACK, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX
			);

			m_MsgManager->addDialogBox(std::move(message1));
			m_MsgManager->addDialogBox(std::move(message2));

			m_LocaisJaVisitados.push_back(SEGURANCA_DISPOSITIVO_MENU);
		}

		auto button1 = std::make_unique<Button>(390, 280, 200, 20, [this]() {
			setState(CONFIGURACOES_SEG_DISPOSITIVO);
			});

		m_ButtonMngr->addButton("IsolamentoNucleo", std::move(button1));

	}
	else if (m_ComputerState == OPCOES_FAMILIA_MENU) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(OPCOES_FAMILIA_MENU)) {
			
			auto message1 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"Essa é a parte que vai causar DEPRESSÃO no seu filho", 
				"Basicamente serve para definir filtros e tempo de tela para membros da família"},
				SDL_RED, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX
			);

			m_MsgManager->addDialogBox(std::move(message1));

			m_LocaisJaVisitados.push_back(OPCOES_FAMILIA_MENU);
		}
	} 
	else if (m_ComputerState == CONFIGURACOES_ANTIVIRUS_MENU) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(CONFIGURACOES_ANTIVIRUS_MENU)) {
			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Aqui é onde você pode gerenciar quais notificações o windows defender pode te enviar", "Ou trocar os serviços do windows defender por outro", "E por que trocar os serviços do windows defender ?"}, SDL_BLUE, 320, 50);

			auto message2 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"O windows defender é uma ótima solução gratuíta para um usuário comum!", "A grande diferença são os recursos extras que o windows defender não possui", "Mas em geral ele é ótimo para identificar e combater ameaças"}, SDL_RED, 340, 50);

			auto message3 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"Alguns dos recursos que o windows defender não tem e outras soluções pagas tem são: ", "VPN, Gerenciadores de Senha e protetores mais avançados para navegação WEB", "Seu firewall também pode não atender usuários avançados, sendo melhor uma outra solução"}, SDL_RED, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX
			);

			m_MsgManager->addDialogBox(std::move(message1));
			m_MsgManager->addDialogBox(std::move(message2));
			m_MsgManager->addDialogBox(std::move(message3));

			m_LocaisJaVisitados.push_back(CONFIGURACOES_ANTIVIRUS_MENU);
		}
	}
	else if (m_ComputerState == CONFIGURACOES_SEG_DISPOSITIVO) {
		_LoadExitBtn();
		_LoadDefenderLateralBtns();

		if (!findLocal(CONFIGURACOES_SEG_DISPOSITIVO)) {

			auto message1 = std::make_unique<Text::LetterByLetter>(
				std::vector<std::string>{"Segundo a documentação do Windows:", "O isolamento do núcleo fornce proteção adicional contra malware",
				"Ele protege processos centrais e cruciais do kernal do Windows contra malware", "isolando - os na memória através de um ambiente virtual"},
				SDL_BLUE, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX
			);

			auto message2 = std::make_unique<Text::FadeIn>(
				std::vector<std::string>{"A integridade de memória, conhecida como HVCI é um recurso de segurança do Windows",
				"O qual dificulta o uso de drivers de baixo nível por programas mal-intencionados", "A integridade de memória cria um ambiente isolado usando virtualização de hardware"}, SDL_RED, 0, 0, Text::MESSAGE_REPOSITION_DIALOGBOX);

			m_MsgManager->addDialogBox(std::move(message1));
			m_MsgManager->addDialogBox(std::move(message2));

			m_LocaisJaVisitados.push_back(CONFIGURACOES_SEG_DISPOSITIVO);
		}

	}

	// Cabooo :)
}

inline void Computer::LoadNewScreen()
{
	m_LoadTexture();
	m_LoadButtons();

	//Foi carregado
	m_LoadNewScreen = false;
}

// Renderizadores:

void Computer::_RenderTextures() {
	for (auto& p : m_Textures) {
		if (p.srcRect.w == 0) {
			SDL_RenderCopy(m_Rend, p.Tex, nullptr, &p.dstRect);
			continue;
		}
		
		if (SDL_RenderCopy(m_Rend, p.Tex, &p.srcRect, &p.dstRect) != 0) {
			SDL_Log(SDL_GetError());
		}
	}

	if (m_inListaAfazeres) RenderNotePad();
	if (m_inSecretPasta) _RenderSecretFolder();
	if (m_LoadNewScreen) LoadNewScreen();
}

// Cria a moldura do pc (a borda)
void Computer::_RenderMoldure() {
	SDL_SetRenderDrawColor(m_Rend, 192, 192, 192, 255);
	SDL_Rect backgroundRect = { 0, 0, Global::windowWidth, Global::windowHeight };
	SDL_RenderFillRect(m_Rend, &backgroundRect);
}

void Computer::_RenderSecretFolder()
{
	SDL_Rect dstRect = { 400, 100, 500, 500 };
	m_TextureMngr->draw("douglasRico", nullptr, &dstRect);
}

void Computer::RenderNotePad(){
	SDL_Rect auxRect = m_Notepad->getRect();

	if (!m_ButtonMngr->find("NotePadExit")) {
		auto button1 = std::make_unique<Button>(auxRect.x + auxRect.w - 20, auxRect.y + 5, 20, 20, [this]() {
		m_inListaAfazeres = false;
		m_Notepad.reset();		
		});

		m_ButtonMngr->addButton("NotePadExit", std::move(button1));
	}else {
		m_ButtonMngr->updatePosition("NotePadExit", auxRect.x + auxRect.w - 20, auxRect.y + 5);
	}
	
	m_Notepad->render(m_Rend, { auxRect.x + auxRect.w - 20, auxRect.y + 5, 20, 20 });
}


// Renderizador geral
void Computer::Render() {
	_RenderMoldure();
	_RenderTextures();
}


// Eventos:

void Computer::MouseInComputer() {
	// Verificador simples
	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	SDL_Point p = { x, y };

	if (SDL_PointInRect(&p, &printRect)) {
		SDL_ShowCursor(SDL_ENABLE);
	}
	else {
		SDL_ShowCursor(SDL_DISABLE);
	}
}


// Gerencia o Comportamento do Scroll do Mouse
void Computer::MouseWhell(const SDL_Event& e) {
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

void Computer::m_UpdtDinamicBtsState() {
	for (auto& p : m_Textures) {
		if (p.Scroll) {
			m_ButtonMngr->updateDinamicButtons(p);
		}
	}
}

// Função geral dos Events
void Computer::Events(const SDL_Event& e) {
	MouseInComputer();

	// Lista de Afazeres dhr
	if (m_inListaAfazeres) m_Notepad->Events(e);
	
	// Atualiza o scroll do mouse
	MouseWhell(e);

	// Atualiza os botões
	// Sim, a função Update ta dentro do events :)
	m_UpdtDinamicBtsState();
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

