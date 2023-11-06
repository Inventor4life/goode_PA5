#include "header.h"


int getNumberOfPlayers(int min, int max) {
	/*
	Returns a number between min and max as selected by the player.
	Will neither confirm nor deny that setting min to negative tears the fabric of reality.
	*/
	int selection = min;
	bool running = true;
	char user_char;

	while (running) {
		printf(
			u8"\x1b[1A%s\x1b[1D"
			u8"\x1b[2B%s\x1b[1D"
			u8"\x1b[1A%d\x1b[1D",
			(selection < max) ? u8"▲" : " ",
			(selection > min) ? u8"▼" : " ",
			selection
		);
		user_char = _getch();

		if (user_char == -32) {  // If user pressed an arrow key
			user_char = (int)_getch();  // Ignore intermediate value

			user_char =
				(user_char == 72) ? 119  // Changes user char to 'W' for up arrow
				: (user_char == 80) ? 115  // or 'S' for down arrow
				: 0;  // Sets it to 0 if the left or right arrows are pressed
		}
		switch (user_char) {
		case 'w':
			(selection < max) ? selection++ : max;  // Increases selection if not at max
			break;
		case 's':
			(selection > min) ? selection-- : min;  // Decreases selection if not at min
			break;
		case 13: //Enter
			running = false;  // Exits outer while loop
		}
	}
	return selection;
}

void updateScoreboard(Die* dice, User* players, int number_of_players) {
	/*
	* updates the scoreboard, does NOT print the scoreboard on screen.
	*/
	for (int i = 0; i < number_of_players; i++) {
		players[i].scores[6] = getPoints(dice, players[i], 6);  // Sub total
		players[i].scores[7] = getPoints(dice, players[i], 7);  // Bonus points
		players[i].scores[8] = getPoints(dice, players[i], 8);  // Upper Total
		players[i].scores[17] = getPoints(dice, players[i], 17);  // Upper Total
		players[i].scores[18] = getPoints(dice, players[i], 18);  // Lower Total
		players[i].scores[19] = getPoints(dice, players[i], 19);  // Grand Total

	}
}

void printScoreboard(User* players, int player_number, int number_of_players, int roll_number, int round_number, int x, int y) {
	//Gotten from https://kidsactivitiesblog.com/wp-content/uploads/2023/09/Yahtzee-Score-Cards-Printable-Screenshot.jpg
	/*
	  Prints the scoreboard at the X and Y location, also shows the selected cell and active player
	
	*/
	char string_buffer[10] = { 0 };
	printf(u8"\x1b[%d;%dH╔═════════════╤", y, x);
	printf(u8"\x1b[%d;%dH║Upper Section", ++y, x);
	printf(u8"\x1b[%d;%dH║Aces ⚀       ", ++y, x);
	printf(u8"\x1b[%d;%dH║Twos ⚁       ", ++y, x);
	printf(u8"\x1b[%d;%dH║Threes ⚂     ", ++y, x);
	printf(u8"\x1b[%d;%dH║Fours ⚃      ", ++y, x);
	printf(u8"\x1b[%d;%dH║Fives ⚄      ", ++y, x);
	printf(u8"\x1b[%d;%dH║Sixes ⚅      ", ++y, x);
	printf(u8"\x1b[%d;%dH║Subtotal     ", ++y, x);
	printf(u8"\x1b[%d;%dH║Subtotal > 63", ++y, x);
	printf(u8"\x1b[%d;%dH║Upper Total  ", ++y, x);
	printf(u8"\x1b[%d;%dH╚═════════════", ++y, x);
	printf(u8"\x1b[%d;%dHRoll %d | Round %d | Player %d", ++y, x, roll_number, round_number, player_number);
	printf(u8"\x1b[%d;%dHLower Section ", ++y, x);
	printf(u8"\x1b[%d;%dH╔═════════════", ++y, x);
	printf(u8"\x1b[%d;%dH║3 of a kind  ", ++y, x);
	printf(u8"\x1b[%d;%dH║4 of a kind  ", ++y, x);
	printf(u8"\x1b[%d;%dH║Full House   ", ++y, x);
	printf(u8"\x1b[%d;%dH║Sm. Straight ", ++y, x);
	printf(u8"\x1b[%d;%dH║Lg. Straight ", ++y, x);
	printf(u8"\x1b[%d;%dH║YAHTZEE      ", ++y, x);
	printf(u8"\x1b[%d;%dH║CHANCE       ", ++y, x);
	printf(u8"\x1b[%d;%dH║YAHTZEE BONUS", ++y, x);
	printf(u8"\x1b[%d;%dH║Upper Total  ", ++y, x);
	printf(u8"\x1b[%d;%dH║Lower Total  ", ++y, x);
	printf(u8"\x1b[%d;%dH║GRAND TOTAL  ", ++y, x);
	printf(u8"\x1b[%d;%dH╚═════════════", ++y, x);
	x += 14;
	for (int i = 0; i < number_of_players; i++) { // Loop to print a new column in the scoreboard for every player
		y -= 26;
		printf(u8"\x1b[%d;%dH╤════════╗", y, x);
		printf(u8"\x1b[%d;%dH│Player %d║", ++y, x, i+1);
		for (int j = 0; j < 20; j++) {
			if (j == 9) {
				printf(u8"\x1b[%d;%dH╧════════╝", ++y, x);
				y+=2;
				printf(u8"\x1b[%d;%dH╤════════╗", ++y, x);
			}
			printf(u8"\x1b[%d;%dH│%s%s%-8s\x1b[m║", ++y, x, (players[i].selected == j)? "\x1b[7m\x1b[6m" : "", (!players[i].selectable[j]) ? "\x1b[2m" : "", (players[i].scores[j] != -1) ? intToString(players[i].scores[j], string_buffer) : " ");
		}
		printf(u8"\x1b[%d;%dH╧════════╝", ++y, x);
		x += 9;
	}
}


char* intToString(int input, char outstring[10]) {
	/*
		Converts an Integer to a string, This is done so scores of -1 show up as blank space instead of '-1'
		returns the address to the string it wrote.
	*/
	int loop = (int)log10(input + .9) + 1;
	for (int i = 0; i < loop; i++) {
		outstring[loop - i - 1] = (input % 10) + '0';
		input /= 10;
	}
	outstring[loop] = '\0';
	return outstring;
}

void startGame(Die* dice, User* players, int number_of_players) {
	
	/*
		starts the game. This is a container function for everything that happens during the game.	
	*/

	int roll_number;

	bool tie = false;  // True if the top score is shared by multiple players.
	int active_player = 0;  // Zero index to the current player in the User* players array
	int round_number = 1;
	int max = 0;  // Highest score achieved by a player

	clearPlayers(players);  // Resets all players to have scores of -1 and all boxes available.
	system("cls");
	while (round_number < 14) {  // Main game loop
		roll_number = 1;
		clearDice(dice, true);  // Resets the dice to show a face value of 1 and not be held.
		printDice(dice, DICE_X, DICE_Y);
		updateScoreboard(dice, players, number_of_players);  // Refreshes scoreboard values 
		printTitle(TITLE_X, TITLE_Y);
		printScoreboard(players, active_player + 1, number_of_players, roll_number, round_number, SCOREBOARD_X, SCOREBOARD_Y);
		pressToRoll(ROLL_BASIC_X, ROLL_BASIC_Y);
		while (roll_number < 4) {
			for (int i = 5; i >= 0; i--) {  // Show all 'free' dice face values start at 1 and increase to 6
				for (int j = 0; j < 6; j++) {
					if (!dice[j].held) {
						dice[j].faceValue = 6-i;
					}
				}
				printDice(dice, DICE_X, DICE_Y);
				Sleep(200);
			}
			rollDice(dice);  // Change all free dice to a random number
			printDice(dice, DICE_X, DICE_Y);
			if (hasPoints(dice, players[active_player], 16)) { // If a yahtzee can be scored
				switch (players[active_player].scores[14]) { // Check the current yahtzee value
				case -1:  // If yahtzee unused
					break; // do nothing
				case 50:
					players[active_player].scores[16] = getPoints(dice, players[active_player], 16);  // If yahtzee has been used, add 100 to the Yahtzee Bonus cell 
				case 0:  // finally
					jokerRules(dice, &players[active_player]);  // play the joker rules (Forced, yahtzee-as-a-wildcard)
				}
				
			}
			// Let player select dice to hold, roll again, or fill in a cell.
			if (!selectMany(dice, players, round_number, number_of_players, active_player, roll_number, ROLL_ADVANCED_X, ROLL_ADVANCED_Y)) {
				roll_number += 50;  // If turn is finished, break while loop 
				players[active_player].selectable[players[active_player].selected] = false;  // Disable the chosen cell
				system("cls");
			}
			players[active_player].selected = -7;  // Select the 'roll again' button
			roll_number++;  // Increase the displayed roll number
		}
		resetJoker(&players[active_player]);  // Disables joker rules
		active_player++;  // move on to the next turn
		if (active_player != (active_player % number_of_players)) {  // Check if it should be a new round
			round_number++;  
			active_player = 0;  // start at the first player
		};
	} // At the end of the game
	system("cls");
	printTitle(TITLE_X, TITLE_Y);
	updateScoreboard(dice, players, number_of_players);
	printScoreboard(players, number_of_players, number_of_players, roll_number-50, 13, SCOREBOARD_X, SCOREBOARD_Y);
	printf("\x1b[%d;1H", TITLE_Y + 7);  // Start 7 columns below the top of the title
	if (number_of_players != 1) {
		for (int i = 0; i < number_of_players; i++) {  // List players and scores, checks for ties
			printf("Player %d: %d points\n", i + 1, players[i].scores[19]);
			if (players[max].scores[19] == players[i].scores[19] && max != i) {
				tie = true;
			}
			if (players[max].scores[19] < players[i].scores[19]) {
				max = i;
				tie = false;
			}
		}
		printf((tie) ? "It's a tie!\n" : "Player %d Wins!\n", max + 1);
	}
	else { // Single player
		printf("You Win!\n");
	}
	printf("Press any key to continue...");  // Finally...
}
                                                                              
int selectOne(int prompt_start, int number_of_options) {
	/*
	* selectOne: Displays a selection menu, player can use W/S, Up/Down, and Enter to choose a menu option.
	* Returns a zero-indexed option number.
	*/
	bool running = true;
	bool is_valid = false;
	int selection = 0;
	int i = 0;
	char user_char;

	while (running) {
		// Prints menu options, inverts the color of the currently selected option.
		for (i = 0; i < number_of_options; i++) {
			printf((selection == i) ?
				"\x1b[7m\x1b[6m * %s\x1b[m\n" : " * %s\n", options[prompt_start + i]);
		}

		// Waits until the user presses Up, Down, W, A, or Enter, and ignores all other keys.
		while (!is_valid) {
			user_char = (int)_getch();

			if (user_char == -32) {  // If user pressed an arrow key
				user_char = (int)_getch();  // Ignore intermediate value

				user_char =
					(user_char == 72) ? 119  // Changes user char to 'w' for up arrow
					: (user_char == 80) ? 115  // or 's' for down arrow
					: 0;  // Sets it to 0 if the left or right arrows are pressed
			}
			switch (user_char) {
			case 'w':
				(selection > 0) ? selection-- : 0;  // Moves user selection up if not at top
				is_valid = true;
				break;
			case 's':
				(selection < number_of_options - 1) ? selection++ : 0;  // Moves user selection down if not at bottom
				is_valid = true;
				break;
			case 13: //Enter
				is_valid = true;
				running = false;  // Exits outer while loop
				break;
			}
		}
	
		is_valid = false;
		printf("\x1b[%dA", number_of_options);   // move cursor to top of menu for overwrite -- Allows text to be above and below menu

	
	}
	system("cls");
	return selection;  // return zero-indexed option number
}

void rollDice(Die* dice) {
	// Sets all free dice to a random value
	for (int i = 0; i < 5; i++) {
		if (!dice[i].held) {
			dice[i].faceValue = rand() % 6 + 1;
		}
	}
}

void clearDice(Die* dice, bool full_clear) {
	// sets all dice.held values to false, and initializes dice on the first run.
	// If full_clear, it also resets face values.
	for (int i = 0; i < 5; i++) {
		if (!dice[i].faceValue || full_clear) {  // If dice are uninitialized
			dice[i].faceValue = 1; // initialize to 1
		}
		if (full_clear) {
			dice[i].held = false;
		}
		dice[i].selected = false;
	}
}

void clearPlayers(User* players) {
	// Resets all players to have no score entries.
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 20; j++) {
			players[i].scores[j] = -1;
			players[i].selected = -1;
			if (j < 6 || (j > 8 && j < 16)) {
				players[i].selectable[j] = true;
			}
		}
	}
}

void printTitle(int x, int y) {
	// *StarWars theme*
	printf(u8"\x1b[%d;%dH██    ██                                             ", y, x);
	printf(u8"\x1b[%d;%dH ██  ██  ▄▄▄▄▄  ██   ██   ██                         ", ++y, x);
	printf(u8"\x1b[%d;%dH  ████  ██   ██ ██   ██ ██████ ██████  ▆▆▆▆▆▆  ▆▆▆▆▆▆", ++y, x);
	printf(u8"\x1b[%d;%dH   ██   ██▄▄▄██ ███████   ██     ▄██▀  ██▃▃▃   ██▃▃▃ ", ++y, x);
	printf(u8"\x1b[%d;%dH   ██   ██   ██ ██   ██   ██   ▄██▀    ██▀▀▀   ██▀▀▀ ", ++y, x);
	printf(u8"\x1b[%d;%dH   ██   ██   ██ ██   ██   ██   ██████  ██▆▆▆▆  ██▆▆▆▆\n\n", ++y, x);
}

void printInstructions(void) {
	// Copied from https://eecs.wsu.edu/~aofallon/cpts121/progassignments/PA5.pdf
	// prints the rules of yahtzee below the title
	printTitle(TITLE_X, TITLE_Y);
	printf(
	"The Rules of Yahtzee:\n"
	"The scorecard used for Yahtzee is composed of two sections. An upper section and a\n"
	"lower section. A total of thirteen boxes or thirteen scoring combinations are divided\n"
	"amongst the sections. The upper section consists of boxes that are scored by summing\n"
	"the value of the dice matching the faces of the box. If a player rolls four 3's, then\n"
	"the score placed in the 3's box is the sum of the dice which is 12. Once a player has\n"
	"chosen to score a box, it may not be changed and the combination is no longer in play\n"
	"for future rounds. If the sum of the scores in the upper section is greater than or\n"
	"equal to 63, then 35 more points are added to the players overall score as a bonus.\n"
	"The lower section contains a number of poker like combinations. See the table below:\n"
	u8"╔═══════════════╤════════════════════════════════╤════════════════════════╗\n"
	u8"║ Name          │ Combination                    │ How to Score           ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ 3 of a kind   │ Three dice with the same face  │ Sum of all face values ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ 4 of a kind   │ Four dice with the same face   │ Sum of all face values ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ Full House    │ One pair and a 3-of-a-kind     │ 25                     ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ Sm. straight  │ a sequence of four dice        │ 25                     ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ Lg. straight  │ a sequence of five dice        │ 40                     ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ Yahtzee       │ Five dice with the same face   │ 50                     ║\n"
	u8"║ (5 of a kind) │                                │                        ║\n"
	u8"╟───────────────┼────────────────────────────────┼────────────────────────╢\n"
	u8"║ Chance        │ Catch-All combination          │ Sum of all face values ║\n"
	u8"╚═══════════════╧════════════════════════════════╧════════════════════════╝\n\n"
	"Press any key to return to menu"
	);
	getch();  // Wait for keypress
	system("cls");
}

bool hasPoints(Die* dice, User player, int row) {
	int freq[6];
	frequency(freq, dice);  // Get frequency of all dice
	if (row < 6) {  //  For all in the lower section
		return freq[row] != 0;  // Returns true if the player has that number in their dice
	}
	if (row == 9) {  // Three of a kind
		for (int i = 0; i < 6; i++) {
			if (freq[i] > 2) {  // Checks if the dice show more than 2 of one number
				return true;
			}
		}
		return false;
	}
	if (row == 10) {  // Four of a kind
		for (int i = 0; i < 6; i++) {
			if (freq[i] > 3) {  // Checks if the dice show more than 3 of that number
				return true;
			}
		}
		return false;
	}
	if (row == 11) {  // Full house
		for (int i = 0; i < 6; i++) {
			if (freq[i] == 3) {  // If the dice show only three of any face value
				for (int j = 0; j < 6; j++) {
					if (freq[j] == 2) {  // And the dice show only two of another face value
						return true;
					}
				}
				return false;
			}
		}
		return false;
	}
	if (row == 12) {  // small straight
		int seq = 0;
		for (int i = 0; i < 6; i++) {  // for all face values
			if (freq[i] != 0) {  // If the player has that value
				seq++;
				if (seq == 4) {  // If the player has four face values in sequence
					return true;
				}
			}
			else {
				seq = 0;  // Reset if the sequence is broken
			}
		}
		return false;
	}
	if (row == 13) {  // Large straight
		for (int i = 0; i < 6; i++) {
			if (freq[i] > 1) {  // If the player has more than 1 of any face value
				return false;
			}
		}
		if (freq[0] == 0 || freq[5] == 0) {  // And they don't have 1 or don't have 6
			return true;
		}
		return false;
	}
	if (row == 14 || row == 16) {  // Yahtzee and Yahtzee Bonus
		for (int i = 0; i < 6; i++) {
			if (freq[i] == 5) {  // Checks for Five-Of-A-Kind
				return true;
			}
		}
		return false;
	}
	return true;
}

int getPoints(Die* dice, User player, int row) {
	int freq[6];
	frequency(freq, dice);
	if (row < 6) {  // Upper section
		return freq[row] * (row+1);  // Total of all dice with that face value
	}
	if (row == 6) {
		return subUpper(player);  // Subtotal of the upper section
	}
	if (row == 7) {
		return (subUpper(player) >= 63) ? 35 : -1;  // 35 point bonus
	}
	if (row == 8 || row == 17) {  // Upper Total -- Total of subtotal + bonus
		return sumUpper(player);
	}
	if (row == 9 || row == 10 || row == 15) {  // Three and Four of a kind, and Chance
		return sumDice(dice);
	}
	if (row == 11) {  // Full House
		return 25;
	}
	if (row == 12) {  // Small Straight
		return 30;
	}
	if (row == 13) {  // Large Straight
		return 40;
	}
	if (row == 14) {  // Yahtzee
		return 50;
	}
	if (row == 16) {  // Yahtzee Bonus
		if (player.scores[16] != -1) { // Adds 100 if not first bonus
			return player.scores[16] + 100;
		}
		return 100;  // Sets to 100 0therwise
	}
	if (row == 18) {  // Lower Total
		return sumLower(player);
	}
	if (row == 19) {  // Grand Total
		return grandTotal(player);
	}
	return 0;
}

int sumDice(Die* dice) {
	// Sums all dice
	int sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += dice[i].faceValue;
	}
	return sum;
}

void frequency(int freq[6],  Die* dice) {
	// Fills an array of size 6 with the frequency each face value.
	memset(freq, 0x00, 6*sizeof(int));
	for (int i = 0; i < 5; i++) {
		freq[dice[i].faceValue-1]++;
	}
}

int subUpper(User player) {
	// Gets subtotal of upper section
	int sum = 0;
	for (int i = 0; i < 6; i++) {
		sum += (player.scores[i] != -1) ? player.scores[i] : 0;
	}
	return sum;
}

int sumUpper(User player) {
	// Gets total of upper section excluding scores of -1
	int sum = subUpper(player);
	sum += (player.scores[7] != -1) ? player.scores[7] : 0;
	return sum;
}

int sumLower(User player) {
	// gets total of lower section, excluding scores of -1
	int sum = 0;
	for (int i = 9; i < 17; i++) {
		sum += (player.scores[i] != -1) ? player.scores[i] : 0;
	}
	return sum;
}

int grandTotal(User player) {
	// Returns the grand total of the scoreboard
	return sumUpper(player) + sumLower(player);
}

void printDie(Die die, int x, int y) {
	// Prints a single die at the given X Y value
	int d1 = die.faceValue;
	printf(u8"\x1b[%d;%dH%s%s"
		u8"┌───────┐\x1b[1B\x1b[9D"
		u8"│ %s   %s │\x1b[1B\x1b[9D"
		u8"│ %s %s %s │\x1b[1B\x1b[9D"
		u8"│ %s   %s │\x1b[1B\x1b[9D"
		u8"└───────┘\x1b[m",
		y, x, (die.held || die.selected) ? u8"\x1b[7m" : "", (die.selected) ? u8"\x1b[6m" : "",
		gt1(d1),          gt3(d1),  // Decide which dots to show

		eq6(d1), odd(d1), eq6(d1),

		gt3(d1),          gt1(d1)
	);
}

void printDice(Die* dice, int x, int y) {
	// Prints 5 dice at a given X Y value
	for (int i = 0; i < 5; i++) {
		printDie(dice[i], i * 11 + x, y);
	}
}

const char* gt1(int die) {
	// returns a dot if the die number is > 1, this is used for the bottom right and top left dots

	return (die > 1) ? u8"◯" : " ";
}

const char* gt3(int die) {
	// returns a dot if the die number is > 3, this is used for top right and bottom left dots

	return (die > 3) ? u8"◯" : " ";
}

const char* eq6(int die) {
	// returns a dot if the die number is 6, this is used for the middle left and middle right dots

	return (die == 6) ? u8"◯" : " ";
}

const char* odd(int die) {
	// returns a dot if the die number is odd, this is used for the center dot.

	return (die % 2 == 1) ? u8"◯" : " ";
}

void pressToRoll(int x, int y) {
	// Will you press the button?
	printf("\x1b[%d;%dH [Press any key to roll]", y, x);
	_getch();
}

bool selectMany(Die* dice, User* players, int round_number, int number_of_players, int active_player, int number_of_rolls, int x, int y) {
	/*
		Breaks the UI into four quadrants (top left going clockwise: Dice, upper scoreboard, lower scoreboard, roll button)
		Lets the player use the WASD/arrow keys to move between quadrants, and changes how the UI behaves depending on which
		quadrant is active

		returns true if dice should be rolled again
		returns false to go to next player
	*/
	char user_char;
	int selected_x = (number_of_rolls == 3) ? 1 : 0;  // Start at the roll button if rolls remain
	int selected_y = (number_of_rolls == 3) ? 0 : 1;  // Otherwise start at top of upper scoreboard

	players[active_player].selected = -7;  // Flash the roll button

	if (number_of_rolls == 3) {  // Flashes the active player's first available score cell after the 3rd roll 
		players[active_player].selected = findNext(players, active_player, 0, 1, false);
	}

	while (true) {  // Forever (until the user chooses a valid option)
		// Rewrite UI to show changes in selection
		printTitle(TITLE_X, TITLE_Y);  
		printScoreboard(players, active_player + 1, number_of_players, number_of_rolls, round_number, SCOREBOARD_X, SCOREBOARD_Y);
		clearDice(dice, false);
		
		// If the dice are currently selected
		if (players[active_player].selected > -7 && players[active_player].selected < 0) {
			dice[6 + players[active_player].selected].selected = true;
		}
		printDice(dice, DICE_X, DICE_Y); // Update dice graphics

		// Print instruction/roll button on all but last roll
		if (number_of_rolls != 3) {
			printf(u8"\x1b[%d;%dH%s┌                                                          ┐", y, x, (selected_x == 0 && selected_y == 1) ? "\x1b[7m\x1b[6m" : "");
			printf(u8"\x1b[%d;%dH│ Use WASD/Arrow keys and Enter to choose.                 │", ++y, x);
			printf(u8"\x1b[%d;%dH│ select dice to hold  or  press this button to roll again │", ++y, x);
			printf(u8"\x1b[%d;%dH│ OR select the scoreboard to get points and end your turn │", ++y, x);
			printf(u8"\x1b[%d;%dH└                                                          ┘\x1b[m", ++y, x);
			y -= 4;
		}
		else {
			// Print a more minimalist button on the last roll
			printf(u8"\x1b[%d;%dH┌                                                          ┐", y, x);
			printf(u8"\x1b[%d;%dH│ Use WASD/Arrow keys and Enter to choose.                 │", ++y, x);
			printf(u8"\x1b[%d;%dH│                                                          │", ++y, x);
			printf(u8"\x1b[%d;%dH│ Select the scoreboard to get points and end your turn    │", ++y, x);
			printf(u8"\x1b[%d;%dH└                                                          ┘", ++y, x);
			y -= 4;
		}
		user_char = (int)_getch();
		if (user_char == -32) {  // If user pressed an arrow key
			user_char = (int)_getch();  // Ignore intermediate value

			user_char =
				(user_char == 72) ? 119  // Changes user char to 'W' for up arrow
				: (user_char == 80) ? 115  // 'S' for down arrow
				: (user_char == 75) ? 97  // 'A' for right arrow
				: (user_char == 77) ? 100  // 'D' for left arrow
				: 0;
		}
		switch (user_char) {
		case 'w':
			if (selected_x == 1) { // If on a scoreboard section
				// Change player selection to the next available cell above
				players[active_player].selected = findNext(players, active_player, players[active_player].selected, -1, true); 
				if (players[active_player].selected < 9) { // If that cell is on the upper board
					selected_y = 0;  // select the upper board
				}
			}
			else {
				if (selected_y != 0) { // If not selecting dice
					selected_y = 0;  // Selected the dice
					players[active_player].selected = -4; // Specifically the center die
				}
			}
			break;
		case 's':
			if (selected_x == 1) {  // If on scoreboard
				// Move to next available cell below
				players[active_player].selected = findNext(players, active_player, players[active_player].selected, 1, true);
				if (players[active_player].selected > 8) { // If cell is on bottom scoreboard
					selected_y = 1; // Select bottom scoreboard
				}
			}
			else if (selected_y != 1 && number_of_rolls != 3) { // If dice are selected
				selected_y = 1; // select the roll button
				players[active_player].selected = -7;
			}
			break;
		case 'a':
			if (number_of_rolls != 3) {  // Disable dice and roll button on last roll
				if (selected_y == 0) {
					if (selected_x == 1) {  // If upper scoreboard selected
						players[active_player].selected = -2;  // Select the die immediately to the left of the board
						selected_x = 0;
					}
					else {
						if (players[active_player].selected > -6) {  // If dice are already selected
							players[active_player].selected--;  // Select the next die to the left
						}
					}
				}
				else { // If on lower scoreboard
					players[active_player].selected = -7; // Select Roll button
					selected_x = 0;
				}
			}
			break;
		case 'd':
			if (selected_x != 1) {
				if (players[active_player].selected == -2 || selected_y == 1) { // If on rightmost die or on roll button
					selected_x = 1;
					// Select the closest available score cell
					players[active_player].selected = findNext(players, active_player, selected_y * 9, 1, false);
					if (players[active_player].selected < 0) {
						players[active_player].selected = findNext(players, active_player, 0, 1, false);
					}
					if (players[active_player].selected > 8) {
						selected_y = 1;
					}
					else {
						selected_y = 0;
					}
				}
				else if (players[active_player].selected != -7) {  // Dice are selected, and it's not the rightmost die
					players[active_player].selected++; // Move one die to the right
				}
			}
			break;
		case 13: //Enter
			if (selected_x == 0) {
				if (selected_y == 1) { // If roll button selected
					return true;
				}
				// If the dice are seleced, hold or release the appropriate die
				dice[6 + players[active_player].selected].held = !dice[6 + players[active_player].selected].held;
			}
			else {
				// On scoreboard
				// and player has no points and Joker rules are off
				if (!hasPoints(dice, players[active_player], players[active_player].selected) && !(hasPoints(dice, players[active_player], 16) && players[active_player].scores[14])) {
					if (!confirmZero(CONFIRM_X, CONFIRM_Y)) { // Asks player to confirm a zero
						system("cls"); // if not confirmed, go back
						break;
					}
					    // If confirmed, fill in the appropriate cell with zero
						players[active_player].scores[players[active_player].selected] = 0;

						// If zero is in Yahtzee, also put zero in Yahtzee Bonus
						if (players[active_player].scores[14] == 0) {
							players[active_player].scores[16] = 0;
						}
						return false;  // Move to next player
				}
				// Gives player points
				players[active_player].scores[players[active_player].selected] = getPoints(dice, players[active_player], players[active_player].selected);
				return false; // move to next player
			}
			break;
		}
	}
}


int findNext(User* players, int active_player, int start, int direction, bool initial) {
	/*
		Given starting cell and direction, return the next available cell.
		If initial is true, return the starting cell if available
	*/
	int first_value = players[active_player].selected; // If no cells are found, return player to current cell
	if (start < 0) {
		start = 0;  // If dice are selected, start at top of upper scoreboard
	}
	if (initial) {
		start += direction; // Start at the cell above or below the given cell
	}
	if (direction == 1) {
		for (int i = start; i < 20; i++) {
			if (players[active_player].selectable[i]) { // move down through cells until one is valid
				return i;
			}
		}
	}
	if (direction == -1) {
		for (int i = start; i >= 0; i--) {
			if (players[active_player].selectable[i]) { // move up through cells until one is valid
				return i;
			}
		}
	}
	return first_value; // Otherwise, move player to original cell
}

bool confirmZero(int x, int y) {
	/*
		Prompts the player to confirm if they want to enter a zero into a score cell
	*/
	char user_char;

	bool selected = false;
	bool is_valid = false;

	while (!is_valid) {
		printf(u8"\x1b[m\x1b[%d;%dH╔═══════════════════════════════════╗", y, x);
		printf(u8"\x1b[%d;%dH║                                   ║", ++y, x);
		printf(u8"\x1b[%d;%dH║ You don't have this! Take a zero? ║", ++y, x);
		printf(u8"\x1b[%d;%dH║                                   ║", ++y, x);
		printf(u8"\x1b[%d;%dH║         %sYes\x1b[m            %sNo\x1b[m         ║", ++y, x, (selected)?"\x1b[7m\x1b[6m" : "", (!selected) ? "\x1b[7m\x1b[6m" : "");
		printf(u8"\x1b[%d;%dH║                                   ║", ++y, x);
		printf(u8"\x1b[%d;%dH╚═══════════════════════════════════╝", ++y, x);
		y -= 6;
		user_char = (int)_getch();
		if (user_char == -32) {  // If user pressed an arrow key
			user_char = (int)_getch();  // Ignore intermediate value

			user_char =
				(user_char == 75) ? 97  // 'A' for right arrow
				: (user_char == 77) ? 100  // 'D' for left arrow
				: 0;
		}
		switch (user_char) {
		case 'a':
			if (!selected) { // If on No, move to yes
				selected = !selected;
			}
			break;
		case 'd':
			if (selected) { // If on Yes, move to no
				selected = !selected;
			}
			break;
		case 13: // Enter
			is_valid = true;
			break;
		}
	}
	return selected;
}

void jokerRules(Die* dice, User* player) {
	/*
		Forced Joker Rules -- If the player rolls a Yahtzee and has already filled in the Yahtzee Box,
		and the appropriate upper section box is filled in
		The Yahtzee acts as a wildcard for the lower section (any cell can be chose and points are given)
		If the lower section is filled:
		The player must take a zero in an upper section cell of their choosing.
	*/
	bool upper_available = false;
	bool lower_available = false;
	for (int i = 0; i < 6; i++) {
		if ((*player).scores[i] == -1 && hasPoints(dice, *player, i)) {  // If available, only the appropriate upper section box is selectable
			(*player).selectable[i] = true;
			upper_available = true;
		}
		else {
			(*player).selectable[i] = false;
		}
	}
	for (int i = 9; i < 16; i++) {
		if ((*player).scores[i] == -1 && !upper_available) {  // If upper is not available, enable any open lowers
			(*player).selectable[i] = true;
			lower_available = true;
		}
		else {
			(*player).selectable[i] = false;
		}
	}
	if (!upper_available && !lower_available) {  // If only other upper cells are available

		resetJoker(player);  // Give a zero to a selected upper section
	}
}

void resetJoker(User* player) {
	// Disables the joker rules
	// For all regular cells: If they are empty, they are now selectable
	for (int i = 0; i < 6; i++) {
		if ((*player).scores[i] == -1) {
			(*player).selectable[i] = true;
		}
	}
	for (int i = 9; i < 16; i++) {
		if ((*player).scores[i] == -1) {
			(*player).selectable[i] = true;
		}
	}
}