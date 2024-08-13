/*

Autor: Thauan Stanziola Souza Vieira

Jogo feito por mim para apresentar o seminário de S.O

Tema: Configurações de Privacidade e Segurança no Windows

Um projeto bem top que eu decidi fazer :)

Deu um trabalhinho viu, mas deu tudo certo

Um abraço para geral ksks

*/


#include <SDL.h>

#include "Globals.h"
#include "Game.h"


int main(int argc, char**argv) {
	// Não vou usar esses arg
	(void)argc;
	(void)argv;

	// Cria o game
	Game game;

	// Verifica se inicializou corretamente
	if (!game.Init("Seminario", DESIGN_WIDTH, DESIGN_HEIGHT)) {
		return -1;
	}


	// Looping Principal
	while (game.GetIsRunning()) {
		game.Event();
		game.Update();
		game.Render();
	}

	// Limpa tudo
	game.cleanUp();

	return 0;
}