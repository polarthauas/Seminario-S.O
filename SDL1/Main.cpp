/*

Autor: Thauan Stanziola Souza Vieira

Jogo feito por mim para apresentar o semin�rio de S.O

Tema: Configura��es de Privacidade e Seguran�a no Windows

Um projeto bem top que eu decidi fazer :)

Deu um trabalhinho viu, mas deu tudo certo

Um abra�o para geral ksks

*/


#include <SDL.h>

#include "Globals.h"
#include "Game.h"


int main(int argc, char**argv) {
	// N�o vou usar esses arg
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