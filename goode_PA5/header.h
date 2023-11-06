#pragma once

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <math.h>

#define _CRT_SECURE_NO_WARNINGS

#define TITLE_X 3
#define TITLE_Y 1

#define SCOREBOARD_X 62
#define SCOREBOARD_Y 1

#define DICE_X 3
#define DICE_Y 8

#define ROLL_BASIC_X 15
#define ROLL_BASIC_Y 14

#define ROLL_ADVANCED_X 0
#define ROLL_ADVANCED_Y 14

#define CONFIRM_X 5
#define CONFIRM_Y 10

typedef struct Die Die;
typedef struct User User;

extern int freq[6];
extern char options[3][6];

struct Die {
	int faceValue;
	bool held;
	bool selected;
};

struct User {
	int scores[20];
	int selected;
	bool selectable[20];
};

void startGame(Die* dice, User* players, int number_of_players);
void clearDice(Die* dice, bool full_clear);
void printDie(Die die, int x, int y);
void printDice(Die* dice, int x, int y);
void printInstructions(void);
void clearPlayers(User* players);
void printTitle(int x, int y);
void frequency(int freq[6], Die* dice);
void rollDice(Die* dice);
void updateScoreboard(Die* dice,User* players, int number_of_players);
void printScoreboard(User* players, int player_number, int number_of_players, int roll_number, int round_number, int x, int y);
void pressToRoll(int x, int y);
void jokerRules(Die* dice, User* player);
void resetJoker(User* player);

const char* gt1(int die);
const char* gt3(int die);
const char* odd(int die);
const char* eq6(int die);
char* intToString(int input, char outstring[10]);

int selectOne(int prompt_start, int number_of_options);
int getNumberOfPlayers(int min, int max);
int getPoints(Die* dice, User player, int row);
int sumDice(Die* dice);
int subUpper(User player);
int sumUpper(User player);
int sumLower(User player);
int grandTotal(User player);
int findNext(User* players, int active_player, int start, int direction, bool initial);

bool selectMany(Die* dice, User* players, int round_number, int number_of_players, int active_player, int number_of_rolls, int x, int y);
bool confirmZero(int x, int y);
bool hasPoints(Die* dice, User player, int row);


#endif