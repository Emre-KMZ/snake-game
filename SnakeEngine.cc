#include "SnakeEngine.hh"
#include <unistd.h>

void SnakeGame::initializeWindow(){
	initscr();
	nodelay(stdscr,TRUE);
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr, height, width);
	// height -= 1;
}

void SnakeGame::drawBorders(){
	// Draw top and bottom borders
	for (int i = 0; i < height; i++){
		mvaddch(i, 0, kBorder);
		mvaddch(i, width - 1, kBorder);
	}

	for (int i = 0; i < width; i++){
		mvaddch(0, i, kBorder);
		mvaddch(height - 1, i, kBorder);
	}

	refresh(); // is maybe redundant
}

void SnakeGame::drawInitialSnake(){
	int curRow = (height - kInitialLength * dir_row[curDir]) / 2;
	int curCol = (width - kInitialLength * dir_col[curDir]) / 2;

	for (int i = 0; i < kInitialLength; i++){
		move(curRow, curCol);
		addch(kSnakeBody);
		snake.push_front({curRow, curCol});
		curCol += dir_col[curDir];
		curRow += dir_row[curDir];
	}

}

bool SnakeGame::getDirectionOrQuitNotBlocking(){

	int input = getch();

	switch (input)
	{
	case KEY_RIGHT:
		if (curDir == 2) break;
		curDir = 0;
		break;
	case KEY_DOWN:
		if (curDir == 3) break;
		curDir = 1;
		break;
	case KEY_LEFT:
		if (curDir == 0) break;
		curDir = 2;
		break;
	case KEY_UP:
		if (curDir == 1) break;
		curDir = 3;
		break;
	case 'q':
	case 'Q':
		return false;
	}

	return true;
}

void SnakeGame::extendSnake(){
	SnakeGame::Coord head = snake.front();
	move(head.row, head.col);
	addch(kSnakeBody);
	int newRow = head.row + dir_row[curDir];
	int newCol = head.col + dir_col[curDir];
	move(newRow, newCol);
	addch(dirToHead[curDir]);
	snake.push_front({newRow, newCol});
	refresh();
}

void SnakeGame::placeFood(){
    // Definetely can be optimized by keeping track of free cells instead of random trials
    // Honestly tho, I don't really care
	int freeCells = (height - 2) * (width - 2) - static_cast<int>(snake.size());
	if (freeCells <= 0) return; // no space left

	int foodRow = 0, foodCol = 0;
	bool onSnake;
	do {
		foodRow = std::rand() % (height - 2) + 1;
		foodCol = std::rand() % (width - 2) + 1;
		onSnake = false;
		for (const auto &seg : snake) {
			if (seg.row == foodRow && seg.col == foodCol) { onSnake = true; break; }
		}
	} while (onSnake);

	move(foodRow, foodCol);
	addch(kFood);
	foodLoc = {foodRow, foodCol};
	refresh();
}

bool SnakeGame::justAteFood(){
	SnakeGame::Coord head = snake.front();
	return (head.row == foodLoc.row && head.col == foodLoc.col);
}

void SnakeGame::trimTail(){
	SnakeGame::Coord tail = snake.back();
	move(tail.row, tail.col);
	addch(' ');
	snake.pop_back();
	refresh();
}

bool SnakeGame::hitsBorderOrItself(){
	SnakeGame::Coord head = snake.front();
	for (size_t i = 1; i < snake.size(); i++) {
		if (head.row == snake[i].row && head.col == snake[i].col) {
			return true;
		}
	}   
	return (head.row == 0 || head.row == height - 1 || head.col == 0 || head.col == width - 1);
}

void SnakeGame::finishScreen(){
	clear();
	std::string msg = "Game Over! Press any key to exit.";
	mvprintw(height / 2, (width - msg.length()) / 2, "%s", msg.c_str());
	refresh();
	nodelay(stdscr, FALSE); // Make getch blocking
	getch();
}

SnakeGame::SnakeGame(){
	curDir = 0;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void SnakeGame::run(){
    initializeWindow();
	drawBorders();
	drawInitialSnake();
	placeFood();
	while (getDirectionOrQuitNotBlocking()) {
		extendSnake();

		// If we didn't eat, trim the tail BEFORE collision check so
		// moving into the previous tail cell is allowed.
		bool ate = justAteFood();
		if (!ate) {
			trimTail();
		}

		if (hitsBorderOrItself()) {
			finishScreen();
			break; // Hit border or itself
		}

		if (ate){
			placeFood();
		}

		refresh();
		usleep(refreshRate);
	}
	endwin();
}