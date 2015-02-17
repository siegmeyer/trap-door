#include "Board.h"
#include <Windows.h>
#include <SFML\Window.hpp>

int main(int argc, char* argv[]){
	__int16 width = 800;
	__int16 height = 800;

	const __int16 tileSize = 32; // pixels
	const __int16 tilesAcross = 15;
	const __int16 tilesDown = 15;

	width = tileSize * tilesAcross;
	height = tileSize * tilesDown;


	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	sf::Window window(sf::VideoMode(800, 800, 32), "TrapDoor", sf::Style::Titlebar | sf::Style::Close, settings);

	Board board(tilesAcross, tilesDown);

	board.board_setup_opengl();

	//
	// Set up gameloop timer
	//
	const unsigned int FRAMES_PER_SECOND = 25;
	const unsigned int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	DWORD next_game_tick = clock();
	int sleep_time = 0;
	
	while (window.isOpen()){
		//
		// Handle Events
		//
		sf::Event e;
		while (window.pollEvent(e)){
			switch (e.type){
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			window.close();
		}

		//
		// Do gameloop stuff
		//
		board.board_handle_input();
		board.board_update();
		board.board_draw();

		// Swap buffers
		window.display();

		// Update timer
		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - clock();

		if (sleep_time >= 0){
			Sleep(sleep_time);
		}
		else {
			printf("Long frame.");
		}
	}

	return 0;
}