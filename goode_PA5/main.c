#include "header.h"

int freq[6] = { 0 };
char options[3][6] = {
	"Play",
	"Rules",
	"Exit" };

int main(void) {

	struct Die dice[5] = { 0 };  // Array of 5 dice
	struct User players[4] = { 0 };  // Array of up to 5 players

	SetConsoleOutputCP(CP_UTF8);  // Necessary to show unicode characters
	
	srand((unsigned int)time(NULL));  // Re-seeds RNG

	start:  // Start of the game loop, returns here after game.

	system("cls");
	printTitle(TITLE_X, TITLE_Y);
	printf("Play the game in fullscreen for best results\n");  // No seriously, play fullscreen or it might break
	switch (selectOne(0, 3)) {  // 0 - Play, 1 - Print Rules, 2 - Exit 
	case 0:
		printTitle(TITLE_X, TITLE_Y);
		printf("Number of players: ");
		startGame(dice, players, getNumberOfPlayers(1, 4));
		_getch();  // At end of game, wait for keypress
		goto start;  // Start game again
	case 1:
		printInstructions();
		goto start;
	case 2:
		return 0;
	}

	return 0;
}
