#include "game_of_life.h"
// game_of_life_def.cpp

Cell::Cell(States state_) : state(state_) {
	if (state == Alive)
		setAlive();
	if (state == Dead)
		setDead();
	
	const float side = 3;
	rectangle.setSize(sf::Vector2f(side, side));	// setting cell size
}
Cell::Cell(const sf::Vector2f size_, States state_) : state(state_) {
	if (state == Alive)
		setAlive();
	if (state == Dead)
		setDead();
	
	rectangle.setSize(size_);	// setting cell size
}


bool Cell::setAlive() {
	state = States::Alive;
	rectangle.setFillColor(sf::Color::Black);
	
	return true;
}

bool Cell::setDead() {
	state = States::Dead;
	rectangle.setFillColor(sf::Color::White);
	
	return true;
}

void Cell::setAbsolutePosition(const sf::Vector2f& vec_) {
	rectangle.setPosition(vec_);
}

Cell::States Cell::getState() const {
	return state;
}

void Cell::setState(States state_) {
	switch (state_){
		case Alive:
			setAlive();
			break;
		case Dead:
			setDead();
			break;
	}
}

void Cell::resolveStateByRules() {
	if (state == States::Alive) {
		if (!(aliveNeighbours == 2 || aliveNeighbours == 3)) {
			setDead();
		}
	}
	if (state == States::Dead) {
		if (aliveNeighbours == 3) {
			setAlive();
		}
	}
	resetAliveNeighbours();
}


void Cell::draw(sf::RenderWindow& w) const {
	w.draw(rectangle);
}

// END of methods of class Cell. Class Stage below

Stage::Stage(sf::Vector2i size_) : sizeOfStage(size_){
	cells = new Cell*[size_.x]; //?? "x" czy "y"
	for (int i = 0; i < size_.x; i++){
		cells[i] = new Cell[size_.y]; //?? same
	}
	
	setTableDimensions();
}

Stage::Stage(Cell::States state_, sf::Vector2i size_) : sizeOfStage(size_){
	cells = new Cell*[size_.x]; //?? "x" czy "y"
	for (int i = 0; i < size_.x; i++){
		cells[i] = new Cell[size_.y]; //?? same
		for (int j = 0; j < size_.y; j++){
			cells[i][j].setState(state_);
		}
	}
	
	setTableDimensions();
}

Stage::~Stage() {
	for (int i = 0; i < sizeOfStage.x; i++){
		delete [] cells[i];
	}
	delete [] cells;
}

void Stage::createRandomLife(int lifeProbabilityDivider) {
	std::srand(time(NULL));
	
	for (int i = 0; i < sizeOfStage.x; i++){
		for (int j = 0; j < sizeOfStage.y; j++){
			int random = (std::rand() % lifeProbabilityDivider);
			if (random == 0)
				cells[i][j].setState(Cell::States::Alive);
			else
				cells[i][j].setState(Cell::States::Dead);
		}
	}
}

void Stage::setTableDimensions() {
	for (int i = 0; i < sizeOfStage.x; i++){
		for (int j = 0; j < sizeOfStage.y; j++){
			cells[i][j].setAbsolutePosition(sf::Vector2f(sideOfCell * i, sideOfCell * j));
		}
	}
}

void Stage::draw(sf::RenderWindow& w) const {
	for (int i = 0; i < sizeOfStage.x; i++){
		for (int j = 0; j < sizeOfStage.y; j++){
			cells[i][j].draw(w);
		}
	}
}

bool Stage::setCellState(const sf::Vector2i coord_, Cell::States state_) {
	
	if (coord_.x >= 0 && coord_.x < sizeOfStage.x &&
		coord_.y >= 0 && coord_.y < sizeOfStage.y){
		cells[coord_.x][coord_.y].setState(state_);
		return true;
	}
	return false;
}
bool Stage::makeAlive(int x, int y) {
	return setCellState(sf::Vector2i(x, y), Cell::States::Alive);
}
bool Stage::makeDead(int x, int y) {
	return setCellState(sf::Vector2i(x, y), Cell::States::Dead);
}

Cell::States Stage::getCellState(const sf::Vector2i coord_) const{
	Cell* cell = getCellAt(coord_);
	if (cell == nullptr) throw "coordinates exceeded stage";
	
	return cell->getState();
}

bool Stage::checkIfXInside(const int x) const {
	return (x > 0 && x <= sizeOfStage.x);
}

bool Stage::checkIfYInside(const int y) const {
	return (y > 0 && y <= sizeOfStage.y);
}

int Stage::getSizeX() const {
	return sizeOfStage.x;
}
int Stage::getSizeY() const {
	return sizeOfStage.y;
}

Cell* Stage::getCellAt(const sf::Vector2i pos) const {
	if (pos.x < 0 || pos.y < 0 || pos.x >= sizeOfStage.x || pos.y >= sizeOfStage.y) return nullptr;

	return &cells[pos.x][pos.y];
}

void Stage::calculateStage() {
	for (int x = 0; x < sizeOfStage.x; x++) {
		for (int y = 0; y < sizeOfStage.y; y++) {
			Cell::States state = getCellState(sf::Vector2i(x, y));

			if (state == Cell::States::Alive) {
				Cell* neighbour = getCellAt(sf::Vector2i(x - 1, y - 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x, y - 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x + 1, y - 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x - 1, y));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x + 1, y));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x - 1, y + 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x, y + 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}

				neighbour = getCellAt(sf::Vector2i(x + 1, y + 1));
				if (neighbour != nullptr) {
					neighbour->increaseAliveNeighbors();
				}
			}
		}
	}
}

void Stage::applyCalculationsAndDraw(sf::RenderWindow& w) {
	for (int x = 0; x < sizeOfStage.x; x++) {
		for (int y = 0; y < sizeOfStage.y; y++) {
			cells[x][y].resolveStateByRules();
			cells[x][y].draw(w);
		}
	}
}




