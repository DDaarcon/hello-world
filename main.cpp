#include <iostream>
#include <chrono>
#include "game_of_life.h"

const int SCREEN_HEIGHT = 650;
const int SCREEN_WIDTH = 800;
const sf::String WINDOW_NAME = "Game of Life";


int main(){
	sf::View mainView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_NAME);
	window.setView(mainView);
	
	Stage testStage(Cell::States::Dead, sf::Vector2i(200, 200));
	// testStage.makeAlive(50, 50);
	// testStage.makeAlive(50, 51);
	// testStage.makeAlive(50, 52);
	// testStage.makeAlive(51, 51);
	// testStage.makeAlive(52, 51);
	testStage.createRandomLife(5);
	
	while(window.isOpen()){
		auto startTime = std::chrono::high_resolution_clock::now();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			window.close();
		}//closing event
		
		
		window.clear(sf::Color(255, 255, 255, 255));
		testStage.draw(window);
		window.display();
		// updateStageCells(testStage);
		testStage.calculateStage();
		testStage.applyCalculationsAndDraw(window);

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() << " milliseconds\n";

		while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() < 100) {}
	}
	
	return 0;
}
