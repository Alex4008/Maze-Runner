/*
This product includes software written by Alex Gray (alexgray2017@gmail.com)
*/
#define _WIN32_WINNT 0x0500
#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <Lmcons.h>
#include <cwchar>
#include <locale> 
#include <fstream>
#include <direct.h>
#include <iomanip>
#include <Winuser.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <atlstr.h>
#pragma comment(lib, "winmm.lib")
#pragma warning(disable : 4996)

using namespace std;
using namespace std::experimental::filesystem::v1;

#include <fstream>
#include <filesystem>
namespace fs = std::experimental::filesystem::v1;

//rows cols
int roomOne[9][14];
int roomTwo[9][14];
int roomThree[9][14];
int roomFour[9][14];
int roomFive[9][14];
int roomSix[9][14];
int roomSeven[9][14];
int roomEight[9][14];
int roomNine[9][14];
int roomTen[9][14];
int roomEleven[9][14];
int roomTwelve[9][14];
int roomThirteen[9][14];
int roomFourteen[9][14];
int roomFifteen[9][14];
int roomSixteen[9][14];
int roomSeventeen[9][14];
int roomEighteen[9][14];

int dungeonMap[9][11];

int masterDungeonMap[9][11];

int pauseMenu[9][14];

int cheatMenuOne[9][14];
int cheatMenuTwo[9][14];
int cheatMenuThree[9][14];

int titleScreen[14][26];

int SaveAndLoadScreen[14][26];

int overwriteScreen[14][26];

int optionsScreen[14][26];

int controlsScreen[14][26];

int loadedRoom[9][14];

struct itemDropped {
	bool roomOne = false;
	bool roomThree = false;
	bool roomSix = false;
	bool roomSeven = false;
	bool roomTen = false;
	bool roomEleven = false;
	bool roomTwelve = false;
	bool roomFourteen = false;
	bool roomSeventeen = false;
};

struct doors {
	bool doorOne = false;
	bool doorTwo = false;
	bool doorThree = false;
	bool doorFour = false;
	bool doorFive = false;
	bool doorSix = false;
	bool doorSeven = false;
	bool doorEight = false;
};


itemDropped dropped;

doors openDoors;

int maxWallMasters = 8;
int createdWallMasters = 0;
int wallMasterCounter = 0;

vector<int> bladeSet(4, 0); //0: Not active. 1: Active, No hit. 2: Active, Returning.
vector<int> bladeReturnCounter(4, 0);
vector<int> bladeSingleMoving(4, 4);

int count = 0;
bool keyW = false;
bool keyA = false;
bool keyS = false;
bool keyD = false;
bool keyB = false;
bool keyPress = false;
bool saveFile = false;

int startingLocY = 7;
int startingLocX = 7;
int startingRoom = 2;

int charYLoc = startingLocY;
int charXLoc = startingLocX;
int currentRoom = startingRoom;

// Up = 0
// Right = 1
// Down = 2
// Left = 3
int lastDirection = 0; //Defaults to up.

bool isDevMode = false;

int playerHealth = 10; //Balance the game as to where we can get this to 6
int playerKeys = 0;
int playerBombs = 5; 
int playerCoins = 0;
bool hasCompass = false;
bool hasMap = false;
bool hasBoomerang = false;
bool hasBow = false;

bool playerIsStunned = false;
int playerStunnedCounter = 0;

int maxPlayerHealth = 10; //Will need balanced.

bool enabledCheats = false;
bool isInvincible = false;
bool infBombs = false;
bool infKeys = false;
bool infCoins = false;
bool disableWallMasters = false;

int bossIcon = 0; //Used to cycle and make the icon blink

int roomNineBlockPushed = false;

bool allEnemiesCleared = false; //Used to open the "action door" of the room once all enemies have been cleared.

int swordLocCols = 0;
int swordLocRows = 0;

int arrowCooldown = 0;

int ballCount = 0;

int roomLong = 9;
int roomWide = 14;

int currentLoadedSave = 0;

int playerScore = 0;

bool saveOneExist = false;
bool saveTwoExist = false;
bool saveThreeExist = false;

string windowsUsername;

string mainDir;

string version = "1.0";

void UpdatePlayerScore(string enemyType);
void RunSound(string soundType);

class Enemy {
public:
	Enemy() { //Default enemy
		health = 1;
		type = "default";
		isStunned = false;
		locationRows = 0;
		locationCols = 0;
		currentDirection = 0;
		collideWall = true;
		movementSpeed = 5;
		movementTimer = 0;
		thrownBoomerang = false;
		stunnedCounter = 0;
		dieBySword = false;
	}

	Enemy(string e, int rows, int cols) { //Overloaded constructor.
		locationRows = rows;
		locationCols = cols;
		isStunned = false;
		type = e;
		currentDirection = 0;
		collideWall = true;
		movementTimer = 0;
		thrownBoomerang = false;
		stunnedCounter = 0;
		dieBySword = false;
		lastDirection = -1;
		if (type == "keese") {
			health = 1;
			movementSpeed = 3;
		}
		else if (type == "goriya") {
			health = 1;
			movementSpeed = 5;
		}
		else if (type == "gel") {
			health = 1;
			movementSpeed = 8;
		}
		else if (type == "skeleton") {
			health = 1;
			movementSpeed = 5;
		}
		else if (type == "bladetrap") {
			health = 1;
			movementSpeed = 0;
		}
		else if (type == "wallmaster") {
			health = 1;
			movementSpeed = 12;
		}
		else if (type == "dragon") {
			health = 3;
			currentDirection = 3;
			movementSpeed = 5;
		}
	}

	int GetLocationRows() {
		return locationRows;
	}

	int GetLocationCols() {
		return locationCols;
	}

	string GetEnemyType() {
		return type;
	}

	int GetCurrentDirection() {
		return currentDirection;
	}

	bool GetCollideWithWall() {
		return collideWall;
	}

	int GetMovementTimer() {
		return movementTimer;
	}

	int GetMovementSpeed() {
		return movementSpeed;
	}

	bool GetStunned() {
		return isStunned;
	}

	bool GetBoomerangState() {
		return thrownBoomerang;
	}

	int GetStunnedCounter() {
		return stunnedCounter;
	}

	bool GetDieBySwordStatus() {
		return dieBySword;
	}

	int GetLastDirection() {
		return lastDirection;
	}

	int GetEnemyHealth() {
		return health;
	}

	void SetLocation(int r, int c) { //Rows, Cols
		locationRows = r;
		locationCols = c;
	}

	void SetLocationRows(int r) {
		locationRows = r;
	}

	void SetLocationCols(int c) {
		locationCols = c;
	}

	void SetCurrentDirection(int newDir) {
		currentDirection = newDir;
	}

	void SetCollideWithWall(bool c) {
		collideWall = c;
	}

	void SetMovementTimer(int value) {
		movementTimer = value;
	}

	void ResetMovementTimer() {
		movementTimer = 0;
	}

	void SetStunned(bool value) {
		isStunned = value;
	}

	void SetBoomerangState(bool value) {
		thrownBoomerang = value;
	}

	void SetDieBySwordStatus(bool value) {
		dieBySword = value;
	}

	void IncreaseStunnedCounter() {
		stunnedCounter++;
	}
	
	void ResetStunnedCounter() {
		stunnedCounter = 0;
	}

	void SetLastDirection(int value) {
		lastDirection = value;
	}

	void SetHealth(int value) {
		if (type == "dragon") {
			RunSound("bossHit");
		}

		health = value;
	}

	void RunDrop() {

		bool wasInWall = false;

		if (type != "dragon") {
			RunSound("enemyDie");
		}
		else {
			RunSound("bossDie");
		}

		if (type == "wallmaster") { //This is to check if the wallmaster died in the wall.
			if (locationRows == 0 || locationRows == 8) {
				wasInWall = true;
			}
			else if (locationCols == 0 || locationCols == 13) {
				wasInWall = true;
			}
		}

		if (loadedRoom[locationRows][locationCols] != 80 && type != "dragon" && wasInWall == false) {
			srand(time(0));
			int random = rand() % 10;
			if (random <= 1) {
				loadedRoom[locationRows][locationCols] = 72;
			}
			else if (random >= 9) {
				loadedRoom[locationRows][locationCols] = 82;
			}
		}
		else if (type == "dragon") {
			loadedRoom[locationRows][locationCols] = 48;
			loadedRoom[locationRows + 1][locationCols] = 48;
			loadedRoom[locationRows + 1][locationCols + 1] = 48;
		}

		UpdatePlayerScore(type);

	}

private:
	int health;
	string type;
	bool isStunned;
	int stunnedCounter;
	int locationRows;
	int locationCols;
	int movementSpeed; // Fast = 3, Normal = 5, Slow = 8
	int currentDirection; //0 Up, 1 Right, 2 Down, 3 Left. 4 - 7 in between
	bool collideWall;
	int movementTimer;
	bool thrownBoomerang;
	bool dieBySword;
	int lastDirection;
};

class Boomerang {
public:
	Boomerang() { //A default boomerang. Not ever to be used.
		locationRows = 0;
		locationCols = 0;
		roomLocated = 0;
		travelDirection = 0;
		isReturning = false;
		e = Enemy();
		thrownByEnemy = false;
		originRow = 0;
		originCol = 0;
	}

	Boomerang(int rows, int cols, int room, int direction) { //Used to create a player thrown boomerang
		locationRows = rows;
		locationCols = cols;
		roomLocated = room;
		travelDirection = direction;
		e = Enemy(); //N/A
		thrownByEnemy = false;
		isReturning = false;
		originRow = 0; //N/A
		originCol = 0; //N/A
	}

	Boomerang(int rows, int cols, int room, int direction, Enemy enemyWhoThrew, int originR, int originC) { //Used to create an enemy thrown boomerang
		locationRows = rows;
		locationCols = cols;
		roomLocated = room;
		travelDirection = direction;
		e = enemyWhoThrew;
		thrownByEnemy = true;
		isReturning = false;
		originRow = originR;
		originCol = originC;
	}

	int GetLocationRows() {
		return locationRows;
	}

	int GetLocationCols() {
		return locationCols;
	}

	int GetTravelDirection() {
		return travelDirection;
	}

	bool GetReturning() {
		return isReturning;
	}

	Enemy GetEnemy() {
		return e;
	}

	bool GetThrownByEnemy() {
		return thrownByEnemy;
	}

	int GetOriginRow() {
		return originRow;
	}

	int GetOriginCol() {
		return originCol;
	}

	void SetLocation(int rows, int cols) {
		locationRows = rows;
		locationCols = cols;
	}

	void SetReturning(bool value) {
		isReturning = value;
	}

private:
	int locationRows;
	int locationCols;
	int roomLocated;
	int travelDirection; //0 is up, 1 is right, 2 is down, 3 is left.
	bool isReturning;
	Enemy e;
	bool thrownByEnemy;
	int originRow;
	int originCol;
};

class Arrow {
public:
	Arrow() { //A default Arrow. Not ever to be used.
		locationRows = 0;
		locationCols = 0;
		roomLocated = 0;
		travelDirection = 0;
	}

	Arrow(int rows, int cols, int room, int direction) { //Used to create a player shot arrow
		locationRows = rows;
		locationCols = cols;
		roomLocated = room;
		travelDirection = direction;
	}

	int GetLocationRows() {
		return locationRows;
	}

	int GetLocationCols() {
		return locationCols;
	}

	int GetTravelDirection() {
		return travelDirection;
	}

	void SetLocation(int rows, int cols) {
		locationRows = rows;
		locationCols = cols;
	}

private:
	int locationRows;
	int locationCols;
	int roomLocated;
	int travelDirection; //0 is up, 1 is right, 2 is down, 3 is left.
};

class Bomb {
public:
	Bomb() { //Default bomb. Not to be used.
		locationRows = 0;
		locationCols = 0;
		roomLocated = 0;
		timer = 0;
		explodeDoor = false;
	}

	Bomb(int rows, int cols, int room) { //Used to create player placed bomb
		locationRows = rows;
		locationCols = cols;
		roomLocated = room;
		timer = 0; //Will explode at 15
		explodeDoor = false;
	}

	Bomb(int rows, int cols, int room, bool door) { //Used to create player placed bomb that will break doors.
		locationRows = rows;
		locationCols = cols;
		roomLocated = room;
		timer = 0; //Will explode at 15
		explodeDoor = true;
	}

	int GetLocationRows() {
		return locationRows;
	}

	int GetLocationCols() {
		return locationCols;
	}

	int GetRoomLocated() {
		return roomLocated;
	}

	int GetTimer() {
		return timer;
	}

	bool GetExplodeDoor() {
		return explodeDoor;
	}

	void SetLocation(int rows, int cols) {
		locationRows = rows;
		locationCols = cols;
	}

	void SetLocatedRoom(int room) {
		roomLocated = room;
	}

	void IncrementTimer() {
		timer++;
	}

private:
	int locationRows;
	int locationCols;
	int roomLocated;
	int timer;
	bool explodeDoor;
};

class Fireball {
public:
	Fireball() { //A default fireball. Not ever to be used.
		locationRows = 0;
		locationCols = 0;
		roomLocated = 0;
		ballNum = 0;
		split = false;
		spread = false;
	}

	Fireball(int rows, int cols, int ballNumber, bool splitStatus) { //Used to create a fireball
		locationRows = rows;
		locationCols = cols;
		ballNum = ballNumber;
		split = splitStatus;
		spread = false;
		loadedRoom[locationRows][locationCols] = 96;
	}

	int GetLocationRows() {
		return locationRows;
	}

	int GetLocationCols() {
		return locationCols;
	}

	int GetBallNumber() {
		return ballNum;
	}

	bool GetSplitStatus() {
		return split;
	}

	bool GetSpread() {
		return spread;
	}

	void SetSplitStatus(bool value) {
		split = value;
	}

	void SetBallNumber(bool value) { //A value between 0 and 2
		ballNum = value;
	}

	bool Move() { //If returns false, it needs to be destroyed.

		if (spread == false) {
			locationCols = locationCols - 1;
			spread = true;
		}
		else {
			spread = false;
			if (ballNum == 0) { //If its the top fireball
				if (loadedRoom[locationRows - 1][locationCols - 1] == 80) {
					RunSound("linkHurt");
					playerHealth--;
					locationCols = locationCols - 1;
					locationRows = locationRows - 1;
					return false;
				}
				else if (loadedRoom[locationRows - 1][locationCols - 1] != 48) {
					loadedRoom[locationRows][locationCols] = 48;
					locationCols = locationCols - 1;
					locationRows = locationRows - 1;
					return false;
				}
				else {
					locationCols = locationCols - 1;
					locationRows = locationRows - 1;
				}
			}
			else if (ballNum == 1) { //If its the middle fireball
				if (loadedRoom[locationRows][locationCols - 1] == 80) {
					RunSound("linkHurt");
					playerHealth--;
					locationCols = locationCols - 1;
					return false;
				}
				else if (loadedRoom[locationRows][locationCols - 1] != 48) {
					loadedRoom[locationRows][locationCols] = 48;
					locationCols = locationCols - 1;
					return false;
				}
				else {
					locationCols = locationCols - 1;
				}
			}
			else if (ballNum == 2) { //If its the bottom fireball
				if (loadedRoom[locationRows + 1][locationCols - 1] == 80) {
					RunSound("linkHurt");
					playerHealth--;
					locationCols = locationCols - 1;
					locationRows = locationRows + 1;
					return false;
				}
				else if (loadedRoom[locationRows + 1][locationCols - 1] != 48) {
					loadedRoom[locationRows][locationCols] = 48;
					locationCols = locationCols - 1;
					locationRows = locationRows + 1;
					return false;
				}
				else {
					locationCols = locationCols - 1;
					locationRows = locationRows + 1;
				}
			}
		}

		if (loadedRoom[locationRows][locationCols] == 80) { //Fireball hits player
			RunSound("linkHurt");
			playerHealth--;
			return false;
		}
		else if (loadedRoom[locationRows][locationCols] != 48) {
			return false;
		}
		else {
			if (spread == false) {
				if (ballNum == 0) { //If its the top fireball
					loadedRoom[locationRows + 1][locationCols + 1] = 48;
				}
				else if (ballNum == 1) { //If its the middle fireball
					loadedRoom[locationRows][locationCols + 1] = 48;
				}
				else if (ballNum == 2) { //If its the bottom fireball
					loadedRoom[locationRows - 1][locationCols + 1] = 48;
				}
			}
			else {
				loadedRoom[locationRows][locationCols + 1] = 48;
			}

			loadedRoom[locationRows][locationCols] = 96;
			return true;
		}

	}

private:
	int locationRows;
	int locationCols;
	int roomLocated;
	int ballNum; //0 is top, 1 is middle, 2 is bottom
	bool spread;
	bool split;
};

vector<Arrow> arrowsLoadedRoom(0);

vector<Boomerang> boomerangsLoadedRoom(0);

vector<Enemy> enemiesLoadedRoom(0);

vector<Bomb> bombsInGame(0);

vector<Fireball> fireballsInGame(0);

vector<Enemy> enemiesRoomOne(0);
vector<Enemy> enemiesRoomTwo(0);
vector<Enemy> enemiesRoomThree(0);
vector<Enemy> enemiesRoomFour(0);
vector<Enemy> enemiesRoomFive(0);
vector<Enemy> enemiesRoomSix(0);
vector<Enemy> enemiesRoomSeven(0);
vector<Enemy> enemiesRoomEight(0);
vector<Enemy> enemiesRoomNine(0);
vector<Enemy> enemiesRoomTen(0);
vector<Enemy> enemiesRoomEleven(0);
vector<Enemy> enemiesRoomTwelve(0);
vector<Enemy> enemiesRoomThirteen(0);
vector<Enemy> enemiesRoomFourteen(0);
vector<Enemy> enemiesRoomFifteen(0);
vector<Enemy> enemiesRoomSixteen(0);
vector<Enemy> enemiesRoomSeventeen(0);

bool gameRunning = true;

int counter = 0;

bool swordActive = false;
int swordCounter = 0;
int swordDirection = 0; //Up = 0, Right = 1, Down = 2, Left = 3

bool boomActive = false;

int dragonShotTimer = 0;

int currentScreenSize = 0; //Small = 0, Medium = 1, Large = 2

int swordDelay = 0;

HWND console = GetConsoleWindow();
RECT r;

bool isConsoleWindowFocused = true;

double gameRunSpeed = 10; //Defines how often player, enemies and objects can move.

//Print Screen Chars
char upperLeft = 201;
char lowerLeft = 200;
char upperRight = 187;
char lowerRight = 188;
char topSide = 205;
char leftSide = 186;
char block = 254;
char actionDoor = 197;
char error = 176;
char player = 219;
char swordUp = 179;
char swordLeft = 196;
char boomerang = 236;
char bow = 237;
char arrowUp = 94;
char arrowDown = 118;
char arrowLeft = 60;
char arrowRight = 62;

char doorLeft = 185;
char doorRight = 204;

char dragonHead = 243;
char dragonLeftLeg = 218;
char dragonRightLeg = 191;

char triforceEnds = 220;
char triforceMiddle = 219;

void ShowConsoleCursor()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

}

bool CheckDir(string strPath) {
	if (access(strPath.c_str(), 0) == 0) {
		struct stat status;
		stat(strPath.c_str(), &status);
		if (status.st_mode & S_IFDIR) {
			return true;
		}
	}
	else {
		return false;
	}
}

void DeleteSave(int saveNumber) {
	string saveNum = "";
	if (saveNumber == 1) {
		remove(mainDir + "savedata\\1-save\\1-room.txt");
		remove(mainDir + "savedata\\1-save\\2-room.txt");
		remove(mainDir + "savedata\\1-save\\3-room.txt");
		remove(mainDir + "savedata\\1-save\\4-room.txt");
		remove(mainDir + "savedata\\1-save\\5-room.txt");
		remove(mainDir + "savedata\\1-save\\6-room.txt");
		remove(mainDir + "savedata\\1-save\\7-room.txt");
		remove(mainDir + "savedata\\1-save\\8-room.txt");
		remove(mainDir + "savedata\\1-save\\9-room.txt");
		remove(mainDir + "savedata\\1-save\\10-room.txt");
		remove(mainDir + "savedata\\1-save\\11-room.txt");
		remove(mainDir + "savedata\\1-save\\12-room.txt");
		remove(mainDir + "savedata\\1-save\\13-room.txt");
		remove(mainDir + "savedata\\1-save\\14-room.txt");
		remove(mainDir + "savedata\\1-save\\15-room.txt");
		remove(mainDir + "savedata\\1-save\\16-room.txt");
		remove(mainDir + "savedata\\1-save\\17-room.txt");
		remove(mainDir + "savedata\\1-save\\18-room.txt");
		remove(mainDir + "savedata\\1-save\\data.txt");
		remove_all(mainDir + "savedata\\1-save");
	}
	else if (saveNumber == 2) {
		remove(mainDir + "savedata\\2-save\\1-room.txt");
		remove(mainDir + "savedata\\2-save\\2-room.txt");
		remove(mainDir + "savedata\\2-save\\3-room.txt");
		remove(mainDir + "savedata\\2-save\\4-room.txt");
		remove(mainDir + "savedata\\2-save\\5-room.txt");
		remove(mainDir + "savedata\\2-save\\6-room.txt");
		remove(mainDir + "savedata\\2-save\\7-room.txt");
		remove(mainDir + "savedata\\2-save\\8-room.txt");
		remove(mainDir + "savedata\\2-save\\9-room.txt");
		remove(mainDir + "savedata\\2-save\\10-room.txt");
		remove(mainDir + "savedata\\2-save\\11-room.txt");
		remove(mainDir + "savedata\\2-save\\12-room.txt");
		remove(mainDir + "savedata\\2-save\\13-room.txt");
		remove(mainDir + "savedata\\2-save\\14-room.txt");
		remove(mainDir + "savedata\\2-save\\15-room.txt");
		remove(mainDir + "savedata\\2-save\\16-room.txt");
		remove(mainDir + "savedata\\2-save\\17-room.txt");
		remove(mainDir + "savedata\\2-save\\18-room.txt");
		remove(mainDir + "savedata\\2-save\\data.txt");
		remove_all(mainDir + "savedata\\2-save");
	}
	else if (saveNumber == 3) {
		remove(mainDir + "savedata\\3-save\\1-room.txt");
		remove(mainDir + "savedata\\3-save\\2-room.txt");
		remove(mainDir + "savedata\\3-save\\3-room.txt");
		remove(mainDir + "savedata\\3-save\\4-room.txt");
		remove(mainDir + "savedata\\3-save\\5-room.txt");
		remove(mainDir + "savedata\\3-save\\6-room.txt");
		remove(mainDir + "savedata\\3-save\\7-room.txt");
		remove(mainDir + "savedata\\3-save\\8-room.txt");
		remove(mainDir + "savedata\\3-save\\9-room.txt");
		remove(mainDir + "savedata\\3-save\\10-room.txt");
		remove(mainDir + "savedata\\3-save\\11-room.txt");
		remove(mainDir + "savedata\\3-save\\12-room.txt");
		remove(mainDir + "savedata\\3-save\\13-room.txt");
		remove(mainDir + "savedata\\3-save\\14-room.txt");
		remove(mainDir + "savedata\\3-save\\15-room.txt");
		remove(mainDir + "savedata\\3-save\\16-room.txt");
		remove(mainDir + "savedata\\3-save\\17-room.txt");
		remove(mainDir + "savedata\\3-save\\18-room.txt");
		remove(mainDir + "savedata\\3-save\\data.txt");
		remove_all(mainDir + "savedata\\3-save");
	}

	//Updates directories.

	saveOneExist = CheckDir(mainDir + "savedata\\1-save");
	saveTwoExist = CheckDir(mainDir + "savedata\\2-save");
	saveThreeExist = CheckDir(mainDir + "savedata\\3-save");

}

void SaveRoom(int room[9][14], int saveNumber, int roomNumber) { //Copies master files from /rooms and puts them into a new directory called 'X-save' where X is a number 1 - 3.
	string saveNum = "";
	if (saveNumber == 1) {
		saveNum = "1-save";
	}
	else if (saveNumber == 2) {
		saveNum = "2-save";
	}
	else if (saveNumber == 3) {
		saveNum = "3-save";
	}

	ofstream outputFile;

	//Room Saving
	outputFile.open(mainDir + "savedata\\" + saveNum + "\\" + to_string(roomNumber) + "-room.txt");
	for (int rows = 0; rows < 9; rows++) {
		for (int cols = 0; cols < 14; cols++) {
			int value = room[rows][cols];

			if (value == 48) { //Display Space
				outputFile << "0 ";
			}
			else if (value == 49) { //Display Top/Bottom Wall piece
				outputFile << "1 ";
			}
			else if (value == 45) {
				outputFile << "- ";
			}
			else if (value == 50) { //Display Left/Right Wall piece
				outputFile << "2 ";
			}
			else if (value == 42) { //Block piece
				outputFile << "* ";
			}
			else if (value == 54 || value == 55 || value == 56 || value == 57) {
				if (value == 54) { //Display Top left corner Wall piece
					outputFile << "6 ";
				}
				else if (value == 55) { //Display Bottom Left corner Wall piece
					outputFile << "7 ";
				}
				else if (value == 56) { //Display Top right corner Wall piece
					outputFile << "8 ";
				}
				else if (value == 57) { //Display Bottom right corner Wall piece
					outputFile << "9 ";
				}
			}
			else if (value == 51) { //Display opened door. On the top or bottom walls
				outputFile << "3 ";
			}
			else if (value == 52) { //Display opened door. On the side wall.
				outputFile << "4 ";
			}
			else if (value == 61) { //Display locked door, on the top or bottom walls (|_|)
				outputFile << "= ";
			}
			else if (value == 124) { //A locked door on the side wall
				outputFile << "| ";
			}
			else if (value == 95) { //An action door on the side wall
				outputFile << "_ ";
			}
			else if (value == 69) { //Display a Gel Enemy
				outputFile << "E ";
			}
			else if (value == 87) { //Display a Wallmaster Enemy
				outputFile << "W ";
			}
			else if (value == 43) { //Display a Blade Trap Enemy
				outputFile << "+ ";
			}
			else if (value == 70) { //Display a Keese Enemy
				outputFile << "F ";
			}
			else if (value == 71) { //Display a Goriya Enemy
				outputFile << "G ";
			}
			else if (value == 83) { //Display a Skeleton Enemy
				outputFile << "S ";
			}
			else if (value == 80) { //Display the player
				outputFile << "0 ";
			}
			else if (value == 179) { //Display sword up/down
				outputFile << "0 ";
			}
			else if (value == 196) { //Display sword left/right
				outputFile << "0 ";
			}
			else if (value == 66) { //An actual bomb
				playerBombs++; //Return the unused bomb to players inv
				if (rows == 0 || rows == 8) {
					outputFile << "1 ";
				}
				else if (cols == 0 || cols == 13) {
					outputFile << "2 ";
				}
				else {
					outputFile << "0 ";
				}
			}
			else if (value == 236) { //The boomerang ACTIVE
				outputFile << "0 ";
			}
			else if (value == 94) { //An arrow going up
				outputFile << "0 ";
			}
			else if (value == 118) { //An arrow going down
				outputFile << "0 ";
			}
			else if (value == 60) { //An arrow going left
				outputFile << "0 ";
			}
			else if (value == 62) { //An arrow going right
				outputFile << "0 ";
			}
			else if (value == 75) { //Display a Key
				outputFile << "K ";
			}
			else if (value == 82) { //Display a Rupee
				outputFile << "R ";
			}
			else if (value == 72) { //Display a heart
				outputFile << "H ";
			}
			else if (value == 67) { //Display compass.
				outputFile << "C ";
			}
			else if (value == 77) { //Display map.
				outputFile << "M ";
			}
			else if (value == 235) { //Display item on ground version of boomerang
				outputFile << '235';
				outputFile << " ";
			}
			else if (value == 'b') { //Display item on ground version of bow
				outputFile << "b ";
			}
			else if (value == 47) { //Display stairs
				outputFile << "/ ";
			}
			else if (value == 33) { //Display invisible wall.
				outputFile << "! ";
			}
			else if (value == 63) { //Display pushable block. (Looks like normal block, can be pushed)
				outputFile << "? ";
			}
			else if (value == 46) { // '.' for messages in room 8
				outputFile << ". ";
			}
			else if (value == 'T') { //Display triforce piece
				outputFile << "T ";
			}
			else if (value == 'D') { //Display dragon Head
				outputFile << "D ";
			}
			else if (value == 'd') { //Display dragon body
				outputFile << "d ";
			}
			else if (value == 96) { //Display a fireball
				outputFile << "0 ";
			}
		}
		outputFile << endl;
	}

	outputFile.close();
}

void SaveGameData(int saveNumber) {
	string saveNum = "";
	if (saveNumber == 1) {
		saveNum = "1-save";
	}
	else if (saveNumber == 2) {
		saveNum = "2-save";
	}
	else if (saveNumber == 3) {
		saveNum = "3-save";
	}

	ofstream outputFile;

	//Data Saving
	outputFile.open(mainDir + "savedata\\" + saveNum + "\\data.txt");

	outputFile << playerHealth << endl;
	outputFile << playerKeys << endl;
	outputFile << playerBombs << endl;
	outputFile << playerCoins << endl;
	outputFile << currentRoom << endl;
	outputFile << charYLoc << endl;
	outputFile << charXLoc << endl;
	outputFile << hasBow << endl;
	outputFile << hasBoomerang << endl;
	outputFile << hasMap << endl;
	outputFile << hasCompass << endl;
	outputFile << enabledCheats << endl;
	outputFile << isInvincible << endl;
	outputFile << infBombs << endl;
	outputFile << infKeys << endl;
	outputFile << infCoins << endl;
	outputFile << disableWallMasters << endl;
	outputFile << roomNineBlockPushed << endl;
	outputFile << dropped.roomOne << endl;
	outputFile << dropped.roomThree << endl;
	outputFile << dropped.roomSix << endl;
	outputFile << dropped.roomSeven << endl;
	outputFile << dropped.roomTen << endl;
	outputFile << dropped.roomEleven << endl;
	outputFile << dropped.roomTwelve << endl;
	outputFile << dropped.roomFourteen << endl;
	outputFile << dropped.roomSeventeen << endl;
	outputFile << openDoors.doorOne << endl;
	outputFile << openDoors.doorTwo << endl;
	outputFile << openDoors.doorThree << endl;
	outputFile << openDoors.doorFour << endl;
	outputFile << openDoors.doorFive << endl;
	outputFile << openDoors.doorSix << endl;
	outputFile << openDoors.doorSeven << endl;
	outputFile << openDoors.doorEight << endl;
	outputFile << maxWallMasters << endl;
	outputFile << createdWallMasters << endl;
	outputFile << playerScore << endl;

	outputFile.close();
}

void SaveGame(int saveNumber) {
	int num = saveNumber;

	if (saveNumber == 1) {
		create_directory(mainDir + "savedata\\1-save");
	}
	else if (saveNumber == 2) {
		create_directory(mainDir + "savedata\\2-save");
	}
	else if (saveNumber == 3) {
		create_directory(mainDir + "savedata\\3-save");
	}

	SaveRoom(roomOne, num, 1);
	SaveRoom(roomTwo, num, 2);
	SaveRoom(roomThree, num, 3);
	SaveRoom(roomFour, num, 4);
	SaveRoom(roomFive, num, 5);
	SaveRoom(roomSix, num, 6);
	SaveRoom(roomSeven, num, 7);
	SaveRoom(roomEight, num, 8);
	SaveRoom(roomNine, num, 9);
	SaveRoom(roomTen, num, 10);
	SaveRoom(roomEleven, num, 11);
	SaveRoom(roomTwelve, num, 12);
	SaveRoom(roomThirteen, num, 13);
	SaveRoom(roomFourteen, num, 14);
	SaveRoom(roomFifteen, num, 15);
	SaveRoom(roomSixteen, num, 16);
	SaveRoom(roomSeventeen, num, 17);
	SaveRoom(roomEighteen, num, 18);

	SaveGameData(num);
}

void LoadGameData(int saveNumber) {
	ifstream inFile;
	int input;
	if (saveNumber == 1) {
		inFile.open(mainDir + "savedata\\1-save\\data.txt");
	}
	else if (saveNumber == 2) {
		inFile.open(mainDir + "savedata\\2-save\\data.txt");
	}
	else if (saveNumber == 3) {
		inFile.open(mainDir + "savedata\\3-save\\data.txt");
	}

	int place = 1;

	while (inFile >> input) {
		if (input >= 0) {
			switch (place) {
			case 1:
				playerHealth = input;
				break;
			case 2:
				playerKeys = input;
				break;
			case 3:
				playerBombs = input;
				break;
			case 4:
				playerCoins = input;
				break;
			case 5:
				currentRoom = input;
				break;
			case 6:
				charYLoc = input;
				break;
			case 7:
				charXLoc = input;
				break;
			case 8:
				hasBow = input;
				break;
			case 9:
				hasBoomerang = input;
				break;
			case 10:
				hasMap = input;
				break;
			case 11:
				hasCompass = input;
				break;
			case 12:
				enabledCheats = input;
				break;
			case 13:
				isInvincible = input;
				break;
			case 14:
				infBombs = input;
				break;
			case 15:
				infKeys = input;
				break;
			case 16:
				infCoins = input;
				break;
			case 17:
				disableWallMasters = input;
				break;
			case 18:
				roomNineBlockPushed = input;
				break;
			case 19:
				dropped.roomOne = input;
				break;
			case 20:
				dropped.roomThree = input;
				break;
			case 21:
				dropped.roomSix = input;
				break;
			case 22:
				dropped.roomSeven = input;
				break;
			case 23:
				dropped.roomTen = input;
				break;
			case 24:
				dropped.roomEleven = input;
				break;
			case 25:
				dropped.roomTwelve = input;
				break;
			case 26:
				dropped.roomFourteen = input;
				break;
			case 27:
				dropped.roomSeventeen = input;
				break;
			case 28:
				openDoors.doorOne = input;
				break;
			case 29:
				openDoors.doorTwo = input;
				break;
			case 30:
				openDoors.doorThree = input;
				break;
			case 31:
				openDoors.doorFour = input;
				break;
			case 32:
				openDoors.doorFive = input;
				break;
			case 33:
				openDoors.doorSix = input;
				break;
			case 34:
				openDoors.doorSeven = input;
				break;
			case 35:
				openDoors.doorEight = input;
				break;
			case 36:
				maxWallMasters = input;
				break;
			case 37:
				createdWallMasters = input;
				break;
			case 38:
				playerScore = input;
				break;
			default:
				cout << "Error. Couldnt load Value: " << place << endl;
				Sleep(5000);
				break;
			}

			//cout << place << ": " << input << endl;
			place++;
		}
	}
	//Sleep(500000);
	inFile.close();
}

void PrintControlsScreen(int menu[14][26], int cursorPos) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	ShowConsoleCursor();
	cout << endl;

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < 14; cols++) {
		for (int rows = -1; rows < 26; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (menu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (menu[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (menu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] <= 90) {
				if (menu[cols][rows] == 'A') {
					cout << " Maze Runner Controls             " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'B') {
					cout << "'WASD' or Arrow Keys to move.         " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'C') {
					cout << "'B' To place bombs.                   " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'D') {
					cout << "'Q' To shoot an arrow.                " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'E') {
					cout << "'E' To throw your boomerang.          " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'F') {
					cout << "'Space' to swing your sword.          " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'G') {
					cout << " Return to Options Menu           " << leftSide;
					rows = 26;
				}
			}
			else if (menu[cols][rows] == 'g' && cursorPos == 0) {
				cout << " " << block;
			}
			else {
				cout << error << " ";
			}
		}
		cout << endl;
	}
}

void PrintOptionsScreen(int menu[14][26], int cursorPos) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	ShowConsoleCursor();
	cout << endl;

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < 14; cols++) {
		for (int rows = -1; rows < 26; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (menu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (menu[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (menu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] <= 90) {
				if (menu[cols][rows] == 'A') {
					cout << "Options Menu                  " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'B') {
					cout << "Controls                    " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'C') {
					if (currentScreenSize == 0) {
						cout << "Screen Size - Small               " << leftSide;
						rows = 26;
					}
					else if (currentScreenSize == 1) {
						cout << "Screen Size - Medium              " << leftSide;
						rows = 26;
					}
					else if (currentScreenSize == 2) {
						cout << "Screen Size - Large               " << leftSide;
						rows = 26; 
					}
				}
				else if (menu[cols][rows] == 'D') {
					cout << "Delete ALL Save Files             " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'E') {
					cout << " Return to Title                " << leftSide;
					rows = 26;
				}
			}
			else if (menu[cols][rows] >= 97) {
				if (menu[cols][rows] == 'b' && cursorPos == 0) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'c' && cursorPos == 1) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'd' && cursorPos == 2) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'e' && cursorPos == 3) {
					cout << " " << block;
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << error << " ";
			}
		}
		cout << endl;
	}
}

void PrintOverwriteScreen(int menu[14][26], int cursorPos, bool isOverwrite = true) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	ShowConsoleCursor();
	cout << endl;

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < 14; cols++) {
		for (int rows = -1; rows < 26; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (menu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (menu[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (menu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] == 'A' && isOverwrite == true) {
				cout << "Do you wish to overwrite this save data?";
				rows = 26;
			}
			else if (menu[cols][rows] == 'A' && isOverwrite == false) {
				cout << "  Do you wish to DELETE all save data?";
				rows = 26;
			}
			else if (menu[cols][rows] == 'Y') {
				cout << "Yes ";
				rows++; //Since its the size of two normal "pixels"
			}
			else if (menu[cols][rows] == 'N') {
				cout << "No";
			}
			else if (menu[cols][rows] == 'y' && cursorPos == 0) {
				cout << block << " ";
			}
			else if (menu[cols][rows] == 'n' && cursorPos == 1) {
				cout << block << " ";
			}
			else {
				cout << "  ";
			}
		}
		cout << endl;
	}
}

void PrintTitleScreen(int menu[14][26], int cursorPos) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	ShowConsoleCursor();
	cout << endl;

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < 14; cols++) {
		for (int rows = -1; rows < 26; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (menu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (menu[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (menu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] <= 90) {
				if (menu[cols][rows] == 'A') {
					cout << "Maze Runner v" << version << "                " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'B') {
					cout << "Start";
					rows = rows + 2;
				}
				else if (menu[cols][rows] == 'C') {
					cout << "Load   ";
					rows = rows + 2;
				}
				else if (menu[cols][rows] == 'D') {
					cout << "Options";
					rows = rows + 3;
				}
				else if (menu[cols][rows] == 'E') {
					cout << "Exit   ";
					rows = rows + 2;
				}
				else if (menu[cols][rows] == 'F') {
					cout << "Developed By: Alex Gray             " << leftSide;
					rows = 26;
				}
			}
			else if (menu[cols][rows] >= 97) {
				if (menu[cols][rows] == 'b' && cursorPos == 0) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'c' && cursorPos == 1) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'd' && cursorPos == 2) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'e' && cursorPos == 3) {
					cout << block << " ";
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << error << " ";
			}
		}
		cout << endl;
	}
}

void PrintSaveLoadScreen(int menu[14][26], int cursorPos, bool save) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	ShowConsoleCursor();
	cout << endl;

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < 14; cols++) {
		for (int rows = -1; rows < 26; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (menu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (menu[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (menu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] <= 90 && save == true) {
				if (menu[cols][rows] == 'A') {
					cout << " Create New Game                " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'B') {
					if (saveOneExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #1                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'C') {
					if (saveTwoExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #2                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'D') {
					if (saveThreeExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #3                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'E') {
					cout << " Return to Title                " << leftSide;
					rows = 26;
				}
			}
			else if (menu[cols][rows] <= 90 && save == false) {
				if (menu[cols][rows] == 'A') {
					cout << "   Load Game                    " << leftSide;
					rows = 26;
				}
				else if (menu[cols][rows] == 'B') {
					if (saveOneExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #1                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'C') {
					if (saveTwoExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #2                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'D') {
					if (saveThreeExist == false) {
						cout << " Empty File                   " << leftSide;
						rows = 26;
					}
					else {
						cout << " Save File #3                 " << leftSide;
						rows = 26;
					}
				}
				else if (menu[cols][rows] == 'E') {
					cout << " Return to Title                " << leftSide;
					rows = 26;
				}
			}
			else if (menu[cols][rows] >= 97) {
				if (menu[cols][rows] == 'b' && cursorPos == 0) {
					cout << " " << block;
				}
				else if (menu[cols][rows] == 'c' && cursorPos == 1) {
					cout << " " << block;
				}
				else if (menu[cols][rows] == 'd' && cursorPos == 2) {
					cout << " " << block;
				}
				else if (menu[cols][rows] == 'e' && cursorPos == 3) {
					cout << " " << block;
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << error << " ";
			}
		}
		cout << endl;
	}
}

void PrintStats() {

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	//cout << endl;

	cout << "  ";
	cout << upperLeft << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << upperRight;
	//cout << endl;

	//cout << "  " << leftSide <<"     ~ Player Stats ~    " << " ";
	//cout << "   " << " " << "   ~ Inventory ~   " << leftSide << endl;

	cout << endl;

	if (playerHealth > 9 || playerHealth < 0) {
		cout << "  " << leftSide << " HP: " << playerHealth << "/" << maxPlayerHealth;
		cout << "    Keys: " << playerKeys << "    " << " ";
		if (hasBow == true) {
			cout << "   " << " " << "   Q Key - [Bow]   " << leftSide << " " << endl;
		}
		else {
			cout << "   " << " " << "  Q Key - [Empty]  " << leftSide << " " << endl;
		}
	}
	else {
		cout << "  " << leftSide << " HP: " << playerHealth << "/" << maxPlayerHealth;
		cout << "     Keys: " << playerKeys << "    " << " ";
		if (hasBow == true) {
			cout << "   " << " " << "   Q Key - [Bow]   " << leftSide << " " << endl;
		}
		else {
			cout << "   " << " " << "  Q Key - [Empty]  " << leftSide << " " << endl;
		}
	}

	cout << "  " << leftSide << " Bombs: " << playerBombs;
	if (playerBombs > 9 || playerBombs < 0) {
		if (playerCoins > 9 || playerCoins < 0) {
			cout << "    Rupees: " << playerCoins << " " << " ";
		}
		else {
			cout << "    Rupees: " << playerCoins << "  " << " ";
		}
	}
	else {
		if (playerCoins > 9 || playerCoins < 0) {
			cout << "    Rupees: " << playerCoins << "  " << " ";
		}
		else {
			cout << "     Rupees: " << playerCoins << "  " << " ";
		}
	}

	if (hasBoomerang == true) {
		if (playerCoins > 9 || playerCoins < 0) {
			cout << "   " << " " << "E Key - [Boomerang]" << leftSide << " " << endl;
		}
		else {
			cout << "   " << " " << "E Key - [Boomerang]" << leftSide << " " << endl;
		}
	}
	else {
		if (playerCoins > 9 || playerCoins < 0) {
			cout << "   " << " " << "  E Key - [Empty]  " << leftSide << " " << endl;
		}
		else {
			cout << "   " << " " << "  E Key - [Empty]  " << leftSide << " " << endl;
		}
	}

	cout << "  ";
	cout << lowerLeft << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide;
	cout << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << topSide << lowerRight;

	if (isDevMode == true) {
		cout << endl;
		cout << "  Debug Mode is ON - Current Room: " << currentRoom << endl;
	}
	else if (enabledCheats == true) {
		cout << endl;
		cout << "                                ~ Cheat Mode - TRUE ~ " << endl;
	}
	else {
		cout << endl << endl;
	}
}

void PrintCheatMenu(int menu[9][14], int cursorPos) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);

	cout << endl;
	PrintStats();

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	char pageUp = 94;
	char pageDown = 118;

	for (int cols = 0; cols < roomLong; cols++) {
		for (int rows = -1; rows < roomWide; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (menu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (menu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (menu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (menu[cols][rows] == 54 || menu[cols][rows] == 55 || menu[cols][rows] == 56 || menu[cols][rows] == 57) {
				if (menu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (pauseMenu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (pauseMenu[cols][rows] == 56) { //Display Top right conrner Wall piece
					cout << upperRight << " ";
				}
				else if (pauseMenu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (menu[cols][rows] == 51) {
				cout << pageUp << " ";
			}
			else if (menu[cols][rows] == 52) {
				cout << pageDown << " ";
			}
			else if (menu[cols][rows] <= 90) {
				if (menu[cols][rows] == 'A') {
					if (isInvincible == true) {
						cout << "Invincible - TRUE   " << leftSide << " ";
					}
					else {
						cout << "Invincible - FALSE  " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'B') {
					if (infBombs == true) {
						cout << "Inf Bombs - TRUE    " << leftSide << " ";
					}
					else {
						cout << "Inf Bombs - FALSE   " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'C') {
					if (infKeys == true) {
						cout << "Inf Keys - TRUE     " << leftSide << " ";
					}
					else {
						cout << "Inf Keys - FALSE    " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'D') {
					if (infCoins == true) {
						cout << "Inf Rupees - TRUE   " << leftSide << " ";
					}
					else {
						cout << "Inf Rupees - FALSE  " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'E') {
					if (hasBoomerang == true) {
						cout << "Boomerang - TRUE    " << leftSide << " ";
					}
					else {
						cout << "Boomerang - FALSE   " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'F') {
					if (hasBow == true) {
						cout << "Bow - TRUE          " << leftSide << " ";
					}
					else {
						cout << "Bow - FALSE         " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'G') {
					if (hasMap == true && hasCompass == true) {
						cout << "Map/Compass - TRUE  " << leftSide << " ";
					}
					else {
						cout << "Map/Compass - FALSE " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'H') {
					if (disableWallMasters == true) {
						cout << "WallMaster: DISABLED" << leftSide << " ";
					}
					else {
						cout << "WallMaster: ENABLE  " << leftSide << " ";
					}
					rows = roomWide;
				}
				else if (menu[cols][rows] == 'I') {
					cout << "Exit Cheat Menu     " << leftSide << " ";
					rows = roomWide;
				}
			}
			else if (menu[cols][rows] >= 97) {
				if (menu[cols][rows] == 'a' && cursorPos == 0) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'b' && cursorPos == 1) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'c' && cursorPos == 2) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'd' && cursorPos == 0) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'e' && cursorPos == 1) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'f' && cursorPos == 2) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'g' && cursorPos == 0) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'h' && cursorPos == 1) {
					cout << block << " ";
				}
				else if (menu[cols][rows] == 'i' && cursorPos == 2) {
					cout << block << " ";
				}
				else {
					cout << "  ";
				}
			}
			else {
				cout << "  ";
			}
		}

		//Print actual Map
		cout << "  ";
		for (int length = 0; length < 11; length++) {
			if (dungeonMap[cols][length] == 48) { //Blank space
				cout << "  ";
			}
			else if (dungeonMap[cols][length] == 49) { //Side wall
				cout << leftSide << " ";
			}
			else if (dungeonMap[cols][length] == 50) { //Top/bottom wall
				cout << topSide << topSide;
			}
			else if (dungeonMap[cols][length] == 51) { //Room icon

				if (cols == 2 && length == 8 && hasCompass == true) {
					if (bossIcon > 6) {
						cout << block << " ";
						if (bossIcon > 12) {
							bossIcon = 0;
						}
						else {
							bossIcon++;
						}
					}
					else {
						cout << "  ";
						bossIcon++;
					}
				}
				else if (hasMap == true) { //If the player has the map, display the room. Otherwise dont.
					cout << block << " ";
				}
				else {
					cout << "  ";
				}
			}
			else if (dungeonMap[cols][length] == 43) { //Player icon
				cout << "+ ";
			}
			else if (dungeonMap[cols][length] == 54) { //Display Top left corner Wall piece
				cout << upperLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 55) { //Display Bottom Left corner Wall piece
				cout << lowerLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 56) { //Display Top right conrner Wall piece
				cout << upperRight << " ";
			}
			else if (dungeonMap[cols][length] == 57) { //Display Bottom right corner Wall piece
				cout << lowerRight << " ";
			}
			else if (dungeonMap[cols][length] == 'A') { //Displays the title
				cout << " " << "~" << " Map " << "~" << "      " << leftSide;
				length = 11;
			}
		}

		cout << endl;
	}
}

void PrintPauseMenu(int cursorPos) {
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);

	cout << endl;
	PrintStats();

	char upperLeft = 201;
	char lowerLeft = 200;
	char upperRight = 187;
	char lowerRight = 188;
	char topSide = 205;
	char leftSide = 186;
	char block = 254;
	char error = 176;

	for (int cols = 0; cols < roomLong; cols++) {
		for (int rows = -1; rows < roomWide; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (pauseMenu[cols][rows] == 48) {
				cout << "  ";
			}
			else if (pauseMenu[cols][rows] == 49) { //Display Top/Bottom Wall piece.
				cout << topSide << topSide;
			}
			else if (pauseMenu[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (pauseMenu[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (pauseMenu[cols][rows] == 54 || pauseMenu[cols][rows] == 55 || pauseMenu[cols][rows] == 56 || pauseMenu[cols][rows] == 57) {
				if (pauseMenu[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (pauseMenu[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (pauseMenu[cols][rows] == 56) { //Display Top right conrner Wall piece
					cout << upperRight << " ";
				}
				else if (pauseMenu[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if(pauseMenu[cols][rows] == 'A') {
				cout << "Return to the game  " << leftSide << " ";
				rows = roomWide;
			}
			else if (pauseMenu[cols][rows] == 'B') {
				cout << "Enter Cheat Menu    " << leftSide << " ";
				rows = roomWide;
			}
			else if (pauseMenu[cols][rows] == 'C') {
				cout << "Save and Exit       " << leftSide << " ";
				rows = roomWide;
			}
			else if (pauseMenu[cols][rows] == 'a' && cursorPos == 0) {
				cout << block << " ";
			}
			else if (pauseMenu[cols][rows] == 'b' && cursorPos == 1) {
				cout << block << " ";
			}
			else if (pauseMenu[cols][rows] == 'c' && cursorPos == 2) {
				cout << block << " ";
			}
			else {
				cout << "  ";
			}
		}

		//Print actual Map
		cout << "  ";
		for (int length = 0; length < 11; length++) {
			if (dungeonMap[cols][length] == 48) { //Blank space
				cout << "  ";
			}
			else if (dungeonMap[cols][length] == 49) { //Side wall
				cout << leftSide << " ";
			}
			else if (dungeonMap[cols][length] == 50) { //Top/bottom wall
				cout << topSide << topSide;
			}
			else if (dungeonMap[cols][length] == 51) { //Room icon

				if (cols == 2 && length == 8 && hasCompass == true) {
					if (bossIcon > 6) {
						cout << block << " ";
						if (bossIcon > 12) {
							bossIcon = 0;
						}
						else {
							bossIcon++;
						}
					}
					else {
						cout << "  ";
						bossIcon++;
					}
				}
				else if (hasMap == true) { //If the player has the map, display the room. Otherwise dont.
					cout << block << " ";
				}
				else {
					cout << "  ";
				}
			}
			else if (dungeonMap[cols][length] == 43) { //Player icon
				cout << "+ ";
			}
			else if (dungeonMap[cols][length] == 54) { //Display Top left corner Wall piece
				cout << upperLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 55) { //Display Bottom Left corner Wall piece
				cout << lowerLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 56) { //Display Top right conrner Wall piece
				cout << upperRight << " ";
			}
			else if (dungeonMap[cols][length] == 57) { //Display Bottom right corner Wall piece
				cout << lowerRight << " ";
			}
			else if (dungeonMap[cols][length] == 'A') { //Displays the title
				cout << " " << "~" << " Map " << "~" << "      " << leftSide;
				length = 11;
			}
		}

		cout << endl;
	}
}

void PrintScreen(int screen[9][14], int dungeonMap[8][11]) {
	//Counters
	int dragonLegCounter = 0;
	int longDoor = 0;
	int placeInMessage = 0; //The place in the special message for room 8
	const COORD p = { 0, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
	cout << endl;

	PrintStats();

	for (int cols = 0; cols < roomLong; cols++) {
		for (int rows = -1; rows < roomWide; rows++) {
			if (rows == -1) {
				cout << "  ";
			}
			else if (screen[cols][rows] == 48) { //Display Space
				cout << "  ";
			}
			else if (screen[cols][rows] == 49 || screen[cols][rows] == 45) { //Display Top/Bottom Wall piece OR A bombable wall. Diffrent ID'S, same character
				cout << topSide << topSide; 
			}
			else if (screen[cols][rows] == 50) { //Display Left/Right Wall piece
				cout << leftSide << " ";
			}
			else if (screen[cols][rows] == 42) { //Block piece
				cout << block << " ";
			}
			else if (screen[cols][rows] == 80) { //Display the player
				cout << player << " ";
			}
			else if (screen[cols][rows] == 54 || screen[cols][rows] == 55 || screen[cols][rows] == 56 || screen[cols][rows] == 57) {
				if (screen[cols][rows] == 54) { //Display Top left corner Wall piece
					cout << upperLeft << topSide;
				}
				else if (screen[cols][rows] == 55) { //Display Bottom Left corner Wall piece
					cout << lowerLeft << topSide;
				}
				else if (screen[cols][rows] == 56) { //Display Top right corner Wall piece
					cout << upperRight << " ";
				}
				else if (screen[cols][rows] == 57) { //Display Bottom right corner Wall piece
					cout << lowerRight << " ";
				}
			}
			else if (screen[cols][rows] == 51) { //Display opened door. On the top or bottom walls
				if (longDoor == 0) {
					cout << doorLeft << " ";
					longDoor++;
				}
				else {
					cout << doorRight << topSide;
					longDoor = 0;
				}
			}
			else if (screen[cols][rows] == 52) { //Display opened door. On the side wall.
				cout << "  ";
			}
			else if (screen[cols][rows] == 61) { //Display locked door, on the top or bottom walls (|_|)
				if (longDoor == 0) {
					cout << doorLeft << "_";
					longDoor++;
				}
				else {
					cout << doorRight << topSide;
					longDoor = 0;
				}
			}
			else if (screen[cols][rows] == 124) { //A locked door on the side wall
				cout << "| ";
			}
			else if (screen[cols][rows] == 95) { //An action door on the side wall
				cout << actionDoor << " ";
			}
			else if (screen[cols][rows] == 69) { //Display a Gel Enemy
				cout << "E ";
			}
			else if (screen[cols][rows] == 87) { //Display a Wallmaster Enemy
				cout << "W ";
			}
			else if (screen[cols][rows] == 43) { //Display a Blade Trap Enemy
				cout << "+ ";
			}
			else if (screen[cols][rows] == 70) { //Display a Keese Enemy
				cout << "F ";
			}
			else if (screen[cols][rows] == 71) { //Display a Goriya Enemy
				cout << "G ";
			}
			else if (screen[cols][rows] == 83) { //Display a Skeleton Enemy
				cout << "S ";
			}
			else if (screen[cols][rows] == 179) { //Display sword up/down
				cout << swordUp << " ";
			}
			else if (screen[cols][rows] == 196) { //Display sword left/right
				cout << swordLeft << " ";
			}
			else if (screen[cols][rows] == 66) { //An actual bomb
				cout << "B ";
			}
			else if (screen[cols][rows] == 236) { //The boomerang ACTIVE
				cout << boomerang << " ";
			}
			else if (screen[cols][rows] == 94) { //An arrow going up
				cout << arrowUp << " ";
			}
			else if (screen[cols][rows] == 118) { //An arrow going down
				cout << arrowDown << " ";
			}
			else if (screen[cols][rows] == 60) { //An arrow going left
				cout << arrowLeft << " ";
			}
			else if (screen[cols][rows] == 62) { //An arrow going right
				cout << arrowRight << " ";
			}
			else if (screen[cols][rows] == 75) { //Display a Key
				cout << "K ";
			}
			else if (screen[cols][rows] == 82) { //Display a Rupee
				cout << "R ";
			}
			else if (screen[cols][rows] == 72) { //Display a heart
				cout << "H ";
			}
			else if (screen[cols][rows] == 'D') {
				cout << dragonHead << " ";
			}
			else if (screen[cols][rows] == 'd') {
				if (dragonLegCounter == 0) {
					cout << dragonLeftLeg << " ";
					dragonLegCounter++;
				}
				else {
					cout << dragonRightLeg << " ";
				}
			}
			else if (screen[cols][rows] == 96) { //Display a fireball
				cout << "* ";
			}
			else if (screen[cols][rows] == 67) { //Display compass.
				cout << "C ";
			}
			else if (screen[cols][rows] == 77) { //Display map.
				cout << "M ";
			}
			else if (screen[cols][rows] == 235) { //Display item on ground version of boomerang
				cout << boomerang << " ";
			}
			else if (screen[cols][rows] == 'b') { //Display item on ground version of bow
				cout << bow << " ";
			}
			else if (screen[cols][rows] == 47) { //Display stairs
				cout << "\\ ";
			}
			else if (screen[cols][rows] == 33) { //Display invisible wall.
				cout << "  ";
			}
			else if (screen[cols][rows] == 63) { //Display pushable block. (Looks like normal block, can be pushed)
				cout << block << " ";
			}
			else if (screen[cols][rows] == 'T') { //Display triforce piece
				cout << triforceEnds << triforceMiddle << triforceEnds << " ";
				rows++; //Because it takes two spots.
			}
			else if (currentRoom == 8) {
				if (screen[cols][rows] == 46) {
					if (placeInMessage == 0) {
						cout << "Ea";
						placeInMessage++;
					}
					else if (placeInMessage == 1) {
						cout << "st";
						placeInMessage++;
					}
					else if (placeInMessage == 2) {
						cout << "mo";
						placeInMessage++;
					}
					else if (placeInMessage == 3) {
						cout << "st";
						placeInMessage++;
					}
					else if (placeInMessage == 4) {
						cout << "Pe";
						placeInMessage++;
					}
					else if (placeInMessage == 5) {
						cout << "nn";
						placeInMessage++;
					}
					else if (placeInMessage == 6) {
						cout << "in";
						placeInMessage++;
					}
					else if (placeInMessage == 7) {
						cout << "su";
						placeInMessage++;
					}
					else if (placeInMessage == 8) {
						cout << "la";
						placeInMessage++;
					}
					else if (placeInMessage == 9) {
						cout << "is";
						placeInMessage++;
					}
					else if (placeInMessage == 10) {
						cout << "th";
						placeInMessage++;
					}
					else if (placeInMessage == 11) {
						cout << "e ";
						placeInMessage++;
					}
					else if (placeInMessage == 12) {
						cout << " S";
						placeInMessage++;
					}
					else if (placeInMessage == 13) {
						cout << "ec";
						placeInMessage++;
					}
					else if (placeInMessage == 14) {
						cout << "re";
						placeInMessage++;
					}
					else if (placeInMessage == 15) {
						cout << "t.";
						placeInMessage++;
					}
				}
			}
			else {
				cout << error << " ";
			}
		}
		
		//Print actual Map
		cout << "  ";
		for (int length = 0; length < 11; length++) {
			if (dungeonMap[cols][length] == 48) { //Blank space
				cout << "  ";
			}
			else if (dungeonMap[cols][length] == 49) { //Side wall
				cout << leftSide << " ";
			}
			else if (dungeonMap[cols][length] == 50) { //Top/bottom wall
				cout << topSide << topSide;
			}
			else if (dungeonMap[cols][length] == 51) { //Room icon

				if (cols == 3 && length == 8 && hasCompass == true) {
					if (bossIcon > 6 * gameRunSpeed) {
						cout << block << " ";
						if (bossIcon > 12 * gameRunSpeed) {
							bossIcon = 0;
						}
						else {
							bossIcon++;
						}
					}
					else {
						cout << "  ";
						bossIcon++;
					}
				}
				else if (hasMap == true) { //If the player has the map, display the room. Otherwise dont.
					cout << block << " ";
				}
				else {
					cout << "  ";
				}
			}
			else if (dungeonMap[cols][length] == 43) { //Player icon
				cout << "+ ";
			}
			else if (dungeonMap[cols][length] == 54) { //Display Top left corner Wall piece
				cout << upperLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 55) { //Display Bottom Left corner Wall piece
				cout << lowerLeft << topSide;
			}
			else if (dungeonMap[cols][length] == 56) { //Display Top right conrner Wall piece
				cout << upperRight << " ";
			}
			else if (dungeonMap[cols][length] == 57) { //Display Bottom right corner Wall piece
				cout << lowerRight << " ";
			}
			else if (dungeonMap[cols][length] == 'A') { //Displays the title
				cout << " " << "~" << " Map " << "~" << "      " << leftSide;
				length = 11;
			}
		}
		
		cout << endl;
	}

	//PrintStats();
}

void LoadMenus() {

	ifstream inFile;
	char input;
	int rows = 0; // Max 9 (8)
	int cols = 0; // Max 14 (13)
	//DUNGEON MAP
	inFile.open(mainDir + "other\\map.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load map" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {

			dungeonMap[cols][rows] = input;
			rows++;

			if (rows == 11) {
				cols++;
				rows = 0;
			}
		}
	}

	memcpy(masterDungeonMap, dungeonMap, sizeof(masterDungeonMap)); //Version of the map that never changes. Used for moving rooms.

	inFile.close();

	//TITLE SCREEN
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\titleScreen.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Title Screen" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			//14 x 26
			titleScreen[rows][cols] = input;

			cols++;
			if (cols == 26) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//OPTIONS SCREEN
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\options.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Options Screen" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			//14 x 26
			optionsScreen[rows][cols] = input;

			cols++;
			if (cols == 26) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//CONTROLS SCREEN
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\controls.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Controls Screen" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			//14 x 26
			controlsScreen[rows][cols] = input;

			cols++;
			if (cols == 26) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//SAVE/LOAD SCREEN
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\SaveAndLoadScreen.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Save/Load Screen" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			//14 x 26
			SaveAndLoadScreen[rows][cols] = input;

			cols++;
			if (cols == 26) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//PAUSE MENU
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\menu.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Pause Menu" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			// 9 x 14
			pauseMenu[rows][cols] = input;

			cols++;
			if (cols == 14) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//OVERWRITE MENU
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\overwriteScreen.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Overwrite Menu" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			//14 x 26
			overwriteScreen[rows][cols] = input;

			cols++;
			if (cols == 26) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//CHEAT MENU ONE
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\1-cheat.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Cheat Menu One" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			// 9 x 14
			cheatMenuOne[rows][cols] = input;

			cols++;
			if (cols == 14) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//CHEAT MENU TWO
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\2-cheat.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Cheat Menu Two" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			// 9 x 14
			cheatMenuTwo[rows][cols] = input;

			cols++;
			if (cols == 14) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();

	//CHEAT MENU THREE
	rows = 0;
	cols = 0;

	inFile.open(mainDir + "other\\3-cheat.txt");
	if (!inFile) {
		cout << "Critcal Error: Unable to load Cheat Menu Three" << endl;
		cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		exit(1);
	}
	while (inFile >> input) {
		if (isspace(input) == false) {
			// 9 x 14
			cheatMenuThree[rows][cols] = input;

			cols++;
			if (cols == 14) {
				rows++;
				cols = 0;
			}
		}
	}

	inFile.close();
}

void LoadRoom(int roomX[9][14], int room, int &charYLoc, int &charXLoc, bool starting) {

	if (currentRoom != 18) {
		if (charYLoc == 0 || charYLoc == 1) { //Player is heading up
			charYLoc = 7;
		}
		else if (charYLoc == 8 || charYLoc == 9) { //Player is heading down
			charYLoc = 1;
		}
		else if (charXLoc == 0 || charXLoc == 1) { //Player is going left
			charXLoc = 12;
		}
		else if (charXLoc == 12 || charXLoc == 13) { //Player is going right
			charXLoc = 1;
		}
		else if (starting == true) {
			//Nothing needs to happen. Just no error.
		}
		else if (currentRoom == 16) {
			charYLoc = 1;
			charXLoc = 2;
		}
		else {
			cout << "Critical Error: Could not find direction. LoadRoom(). NOT room 18." << endl;
			Sleep(5000);
			exit(1);
		}
	}
	else if (currentRoom == 18) {
		RunSound("stairs");
		charYLoc = 4;
		charXLoc = 5;
	}
	else {
		cout << "Critical Error: Could not find direction. LoadRoom()." << endl;
		Sleep(5000);
		exit(1);
	}
	currentRoom = room;
	roomX[charYLoc][charXLoc] = 80;
}

void MovePlayer(int room[9][14], int &charYLoc, int &charXLoc, bool sword, int swordDir) {
	bool done = false;
	bool changeRoom = false;
	while (done == false) {
		for (int cols = 0; cols < roomLong; cols++) {
			for (int rows = 0; rows < roomWide; rows++) {
				if (room[cols][rows] == 80) {
					room[cols][rows] = 48;
					done = true;
				}
			}
		}

		if (done == false) {
			room[charYLoc][charXLoc] = 80;
		}

	}

	//For resetting the last direction of an enemy.
	for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
		enemiesLoadedRoom.at(i).SetLastDirection(-1);
	}


	//If player stepped on a key, add a key to players inv.
	if (room[charYLoc][charXLoc] == 75) {
		playerKeys++;
		RunSound("getHeart");
	}
	else if (room[charYLoc][charXLoc] == 72) { //Obtain health
		RunSound("getHeart");
		if (playerHealth < maxPlayerHealth) {
			playerHealth++;
		}
	}
	else if (room[charYLoc][charXLoc] == 82) { //Obtain Rupee
		if (currentRoom == 8) { //Room eight contains the two mega rupees worth 10 each.
			playerCoins = playerCoins + 10;
			RunSound("getItem");
		}
		else { //Regular rupees
			playerCoins++;
			RunSound("getRupee");
		}
	}
	else if (room[charYLoc][charXLoc] == 67) { //Obtain Compass
		hasCompass = true;
		RunSound("getItem");
	}
	else if (room[charYLoc][charXLoc] == 77) { //Obtain Map
		hasMap = true;
		RunSound("getItem");
	}
	else if (room[charYLoc][charXLoc] == 235) { //Obtain boomerang
		hasBoomerang = true;
		RunSound("getItem");
	}
	else if (room[charYLoc][charXLoc] == 98) { //Obtain bow
		hasBow = true;
		RunSound("getBow");
	}
	else if (room[charYLoc][charXLoc] == 'T') { //Obtain Triforce. Beat the game.
		RunSound("getItem");
		RunSound("beatGame");
		system("CLS");
		cout << "Congratulations! You won!" << endl;
		cout << "Thank you for playing!" << endl;
		cout << "Press 'ESC' To Return To The Title Screen." << endl;
		cout << endl;
		cout << "Maze Runner Version " << version << " Developed By Alex Gray." << endl;
		cout << "Based on The Legend of Zelda, Created by Nintendo." << endl;
		bool waiting = true;
		while (waiting == true) {
			if (GetKeyState(VK_ESCAPE) & 0x8000) {
				gameRunning = false;
				waiting = false;
				DeleteSave(currentLoadedSave);
				RunSound("bombDrop");
				Sleep(300);
			}
		}

	}

	if (room[charYLoc][charXLoc] == 51 || room[charYLoc][charXLoc] == 52 || room[charYLoc][charXLoc] == 61 || room[charYLoc][charXLoc] == 124 || room[charYLoc][charXLoc] == 47) { //If you will be leaving the room
		//This is for opening double doors with one key
		if (room[charYLoc][charXLoc - 1] == 61) {
			room[charYLoc][charXLoc - 1] = 51;
			room[charYLoc][charXLoc] = 51;
		}
		else if (room[charYLoc][charXLoc + 1] == 61) {
			room[charYLoc][charXLoc + 1] = 51;
			room[charYLoc][charXLoc] = 51;
		}
		else if (room[charYLoc][charXLoc] == 124) { //For doors on the sides.
			room[charYLoc][charXLoc] = 52;
		}
	}
	else {
		room[charYLoc][charXLoc] = 80;

		//Sword Handler Code
		if (sword == true) {
			if (swordDir == 0 || swordDir == 2) {
				loadedRoom[swordLocRows][swordLocCols] = 179;
			}
			else if (swordDir == 1 || swordDir == 3) {
				loadedRoom[swordLocRows][swordLocCols] = 196;
			}
		}
	}

}

void ResetBladeTraps() {
	//Special code to reset blade traps

	bladeSet.clear();
	bladeSet.push_back(0);
	bladeSet.push_back(0);
	bladeSet.push_back(0);
	bladeSet.push_back(0);

	bladeReturnCounter.clear();
	bladeReturnCounter.push_back(0);
	bladeReturnCounter.push_back(0);
	bladeReturnCounter.push_back(0);
	bladeReturnCounter.push_back(0);

	bladeSingleMoving.clear();
	bladeSingleMoving.push_back(4);
	bladeSingleMoving.push_back(4);
	bladeSingleMoving.push_back(4);
	bladeSingleMoving.push_back(4);

	for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
		loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;

		roomSixteen[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
	}

	enemiesRoomSixteen.clear();
	enemiesRoomSixteen.push_back(Enemy("bladetrap", 1, 1));
	enemiesRoomSixteen.push_back(Enemy("bladetrap", 1, 11));
	enemiesRoomSixteen.push_back(Enemy("bladetrap", 7, 1));
	enemiesRoomSixteen.push_back(Enemy("bladetrap", 7, 11));

	enemiesLoadedRoom.clear();
	enemiesLoadedRoom.push_back(Enemy("bladetrap", 1, 1));
	enemiesLoadedRoom.push_back(Enemy("bladetrap", 1, 11));
	enemiesLoadedRoom.push_back(Enemy("bladetrap", 7, 1));
	enemiesLoadedRoom.push_back(Enemy("bladetrap", 7, 11));

	loadedRoom[1][1] = 43;
	roomSixteen[1][1] = 43;

	loadedRoom[7][1] = 43;
	roomSixteen[7][1] = 43;

	loadedRoom[1][11] = 43;
	roomSixteen[1][11] = 43;

	loadedRoom[7][11] = 43;
	roomSixteen[7][11] = 43;

	memcpy(roomSixteen, loadedRoom, sizeof(roomSixteen));
	//End special code
}

void ChangeRoom(int room[9][14], int &charYLoc, int &charXLoc) {

	if (room[charYLoc][charXLoc] == 51 || room[charYLoc][charXLoc] == 52) {

		memcpy(dungeonMap, masterDungeonMap, sizeof(dungeonMap)); //Resets dungeon map.
		allEnemiesCleared = false; //Resets the action door bool
		system("CLS");

		switch (currentRoom) {
		case 1: //If the current room is 1, the only place to go is room 2. So it loads room 2
			LoadRoom(roomTwo, 2, charYLoc, charXLoc, false);
			dungeonMap[7][5] = 43; //Setting player icon to room two
			memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomTwo;
			break;
		case 2:
			if (charYLoc == 0) { //Going up to room 4
				LoadRoom(roomFour, 4, charYLoc, charXLoc, false);
				dungeonMap[6][5] = 43; //Setting player icon for room 4
				memcpy(loadedRoom, roomFour, sizeof(loadedRoom));
				//From 2 to 4 uses a locked door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorOne = true;
				enemiesLoadedRoom = enemiesRoomFour;
			}
			else if (charXLoc == 0) { //Going left to room 1
				LoadRoom(roomOne, 1, charYLoc, charXLoc, false);
				dungeonMap[7][4] = 43; //Setting player icon for room 1
				memcpy(loadedRoom, roomOne, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomOne;
			}
			else if (charXLoc == 13) { //Going right to room 3
				LoadRoom(roomThree, 3, charYLoc, charXLoc, false);
				dungeonMap[7][6] = 43; //Setting player icon for room 3
				memcpy(loadedRoom, roomThree, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomThree;
			}
			break;
		case 3: //If the current room is 3, the only place to go is room 2. So it loads room 2
			LoadRoom(roomTwo, 2, charYLoc, charXLoc, false);
			dungeonMap[7][5] = 43; //Setting player icon to room two
			memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomTwo;
			break;
		case 4:
			if (charYLoc == 0) { //Going up to room six
				LoadRoom(roomSix, 6, charYLoc, charXLoc, false);
				dungeonMap[5][5] = 43; //Setting player icon for room 6
				memcpy(loadedRoom, roomSix, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomSix;
			}
			else if (charYLoc == 8) { //Going down to room 2
				LoadRoom(roomTwo, 2, charYLoc, charXLoc, false);
				dungeonMap[7][5] = 43; //Setting player icon to room two
				memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));
				//From 4 to 2 uses a locked door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorOne = true;
				enemiesLoadedRoom = enemiesRoomTwo;
			}
			break;
		case 5:
			if (charYLoc == 0) { //Going up to room nine
				LoadRoom(roomNine, 9, charYLoc, charXLoc, false);
				dungeonMap[4][4] = 43; //Setting player icon for room 9
				memcpy(loadedRoom, roomNine, sizeof(loadedRoom));
				//From 5 to 9 uses a locked door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorTwo = true;
				enemiesLoadedRoom = enemiesRoomNine;
			}
			else if (charXLoc == 13) { //Going right to room six
				LoadRoom(roomSix, 6, charYLoc, charXLoc, false);
				dungeonMap[5][5] = 43; //Setting player icon for room 6
				memcpy(loadedRoom, roomSix, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomSix;
			}
			break;
		case 6:
			if (charYLoc == 0) { //Going up to room ten
				LoadRoom(roomTen, 10, charYLoc, charXLoc, false);
				dungeonMap[4][5] = 43; //Setting player icon for room 10
				memcpy(loadedRoom, roomTen, sizeof(loadedRoom));
				//From 6 to 10 uses a bombable door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorThree = true;
				enemiesLoadedRoom = enemiesRoomTen;
			}
			else if (charXLoc == 0) { //Going left to room five
				LoadRoom(roomFive, 5, charYLoc, charXLoc, false);
				dungeonMap[5][4] = 43; //Setting player icon for room 5
				memcpy(loadedRoom, roomFive, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomFive;
			}
			else if (charXLoc == 13) { //Going right to room seven
				LoadRoom(roomSeven, 7, charYLoc, charXLoc, false);
				dungeonMap[5][6] = 43; //Setting player icon for room 7
				memcpy(loadedRoom, roomSeven, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomSeven;
			}
			else if (charYLoc == 8) { //Going down to room four
				LoadRoom(roomFour, 4, charYLoc, charXLoc, false);
				dungeonMap[6][5] = 43; //Setting player icon for room 4
				memcpy(loadedRoom, roomFour, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomFour;
			}
			break;
		case 7:
			if (charYLoc == 0) { //Going up to room eleven
				LoadRoom(roomEleven, 11, charYLoc, charXLoc, false);
				dungeonMap[4][6] = 43; //Setting player icon for room 11
				memcpy(loadedRoom, roomEleven, sizeof(loadedRoom));
				//From 7 to 11 uses a bombable door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorFour = true;
				enemiesLoadedRoom = enemiesRoomEleven;
			}
			else if (charXLoc == 0) { //Going left to room six
				LoadRoom(roomSix, 6, charYLoc, charXLoc, false);
				dungeonMap[5][5] = 43; //Setting player icon for room 6
				memcpy(loadedRoom, roomSix, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomSix;
			}
			break;
		case 8: //From eight the player can only go to room nine. So it loads room nine.
			LoadRoom(roomNine, 9, charYLoc, charXLoc, false);
			dungeonMap[4][4] = 43; //Setting player icon for room 9
			memcpy(loadedRoom, roomNine, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomNine;
			break;
		case 9:
			if (charXLoc == 0) { //Going left to room eight
				LoadRoom(roomEight, 8, charYLoc, charXLoc, false);
				dungeonMap[4][3] = 43; //Setting player icon for room 8
				memcpy(loadedRoom, roomEight, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomEight;
			}
			else if (charXLoc == 13) { //Going right to room ten
				LoadRoom(roomTen, 10, charYLoc, charXLoc, false);
				dungeonMap[4][5] = 43; //Setting player icon for room 10
				memcpy(loadedRoom, roomTen, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomTen;
			}
			else if (charYLoc == 8) { //Going down to room five
				LoadRoom(roomFive, 5, charYLoc, charXLoc, false);
				dungeonMap[5][4] = 43; //Setting player icon for room 5
				memcpy(loadedRoom, roomFive, sizeof(loadedRoom));
				//From 9 to 5 uses a locked door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorTwo = true;
				enemiesLoadedRoom = enemiesRoomFive;
			}
			break;
		case 10:
			if (charYLoc == 0) { //Going up to room thirteen
				LoadRoom(roomThirteen, 13, charYLoc, charXLoc, false);
				dungeonMap[3][5] = 43; //Setting player icon for room 13
				memcpy(loadedRoom, roomThirteen, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomThirteen;
			}
			else if (charXLoc == 0) { //Going left to room nine
				LoadRoom(roomNine, 9, charYLoc, charXLoc, false);
				dungeonMap[4][4] = 43; //Setting player icon for room 9
				memcpy(loadedRoom, roomNine, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomNine;
			}
			else if (charXLoc == 13) { //Going right to room eleven
				LoadRoom(roomEleven, 11, charYLoc, charXLoc, false);
				dungeonMap[4][6] = 43; //Setting player icon for room 11
				memcpy(loadedRoom, roomEleven, sizeof(loadedRoom));
				//From 10 to 11 uses a locked door. This is the code to open the other side.
				loadedRoom[4][0] = 52;
				//Adds opened door to list
				openDoors.doorFive = true;
				enemiesLoadedRoom = enemiesRoomEleven;
			}
			else if (charYLoc == 8) { //Going down to room six
				LoadRoom(roomSix, 6, charYLoc, charXLoc, false);
				dungeonMap[5][5] = 43; //Setting player icon for room 6
				memcpy(loadedRoom, roomSix, sizeof(loadedRoom));
				//From 10 to 6 uses a bombable door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorThree = true;
				enemiesLoadedRoom = enemiesRoomSix;
			}
			break;
		case 11:
			if (charXLoc == 0) { //Going left to room ten
				LoadRoom(roomTen, 10, charYLoc, charXLoc, false);
				dungeonMap[4][5] = 43; //Setting player icon for room 10
				memcpy(loadedRoom, roomTen, sizeof(loadedRoom));
				//From 11 to 10 uses a locked door. This is the code to open the other side.
				loadedRoom[4][13] = 52;
				//Adds opened door to list
				openDoors.doorFive = true;
				enemiesLoadedRoom = enemiesRoomTen;
			}
			else if (charXLoc == 13) { //Going right to room twelve
				LoadRoom(roomTwelve, 12, charYLoc, charXLoc, false);
				dungeonMap[4][7] = 43; //Setting player icon for room 12
				memcpy(loadedRoom, roomTwelve, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomTwelve;
			}
			else if (charYLoc == 8) { //Going down to room seven
				LoadRoom(roomSeven, 7, charYLoc, charXLoc, false);
				dungeonMap[5][6] = 43; //Setting player icon for room 7
				memcpy(loadedRoom, roomSeven, sizeof(loadedRoom));
				//From 11 to 7 uses a bombable door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorFour = true;
				enemiesLoadedRoom = enemiesRoomSeven;
			}
			break;
		case 12:
			if (charYLoc == 0) { //Going up to room fourteen
				LoadRoom(roomFourteen, 14, charYLoc, charXLoc, false);
				dungeonMap[3][7] = 43; //Setting player icon for room 14
				memcpy(loadedRoom, roomFourteen, sizeof(loadedRoom));
				//From 12 to 14 uses a locked door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorSix = true;
				enemiesLoadedRoom = enemiesRoomFourteen;
			}
			else if (charXLoc == 0) { //Going left to room eleven
				LoadRoom(roomEleven, 11, charYLoc, charXLoc, false);
				dungeonMap[4][6] = 43; //Setting player icon for room 11
				memcpy(loadedRoom, roomEleven, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomEleven;
			}
			break;
		case 13:
			if (charYLoc == 0) { //Going up to room seventeen
				LoadRoom(roomSeventeen, 17, charYLoc, charXLoc, false);
				dungeonMap[2][5] = 43; //Setting player icon for room 17
				memcpy(loadedRoom, roomSeventeen, sizeof(loadedRoom));
				//From 13 to 17 uses a locked door. This is the code to open the other side.
				loadedRoom[8][6] = 51;
				loadedRoom[8][7] = 51;
				//Adds opened door to list
				openDoors.doorSeven = true;
				enemiesLoadedRoom = enemiesRoomSeventeen;
			}
			else if (charYLoc == 8) { //Going down to room ten
				LoadRoom(roomTen, 10, charYLoc, charXLoc, false);
				dungeonMap[4][5] = 43; //Setting player icon for room 10
				memcpy(loadedRoom, roomTen, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomTen;
			}
			break;
		case 14:
			if (charXLoc == 13) { //Going right to room fifteen
				LoadRoom(roomFifteen, 15, charYLoc, charXLoc, false);
				dungeonMap[3][8] = 43; //Setting player icon for room 15
				memcpy(loadedRoom, roomFifteen, sizeof(loadedRoom));
				enemiesLoadedRoom = enemiesRoomFifteen;
			}
			else if (charYLoc == 8) { //Going down to room twelve
				LoadRoom(roomTwelve, 12, charYLoc, charXLoc, false);
				dungeonMap[4][7] = 43; //Setting player icon for room 12
				memcpy(loadedRoom, roomTwelve, sizeof(loadedRoom));
				//From 14 to 12 uses a locked door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorSix = true;
				enemiesLoadedRoom = enemiesRoomTwelve;
			}
			break;
		case 15: //The only room the player can go to from 15 is 14. Loading that room
			LoadRoom(roomFourteen, 14, charYLoc, charXLoc, false);
			dungeonMap[3][7] = 43; //Setting player icon for room 14
			memcpy(loadedRoom, roomFourteen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomFourteen;
			break;
		case 16: //The only room the player can go to from 16 is 17. Loading that room

			ResetBladeTraps();

			LoadRoom(roomSeventeen, 17, charYLoc, charXLoc, false);
			dungeonMap[2][5] = 43; //Setting player icon for room 17
			memcpy(loadedRoom, roomSeventeen, sizeof(loadedRoom));
			//From 16 to 17 uses a locked door. This is the code to open the other side.
			loadedRoom[4][0] = 52;
			//Adds opened door to list
			openDoors.doorEight = true;
			enemiesLoadedRoom = enemiesRoomSeventeen;
			break;
		case 17:
			if (charXLoc == 0) { //Going left to room sixteen
				LoadRoom(roomSixteen, 16, charYLoc, charXLoc, false);
				dungeonMap[2][4] = 43; //Setting player icon for room 16
				memcpy(loadedRoom, roomSixteen, sizeof(loadedRoom));
				//From 17 to 16 uses a locked door. This is the code to open the other side.
				loadedRoom[4][13] = 52;
				//Adds opened door to list
				openDoors.doorEight = true;
				enemiesLoadedRoom = enemiesRoomSixteen;
			}
			else if (charYLoc == 8) { //Going down to room thirteen
				LoadRoom(roomThirteen, 13, charYLoc, charXLoc, false);
				dungeonMap[3][5] = 43; //Setting player icon for room 13
				memcpy(loadedRoom, roomThirteen, sizeof(loadedRoom));
				//From 17 to 13 uses a locked door. This is the code to open the other side.
				loadedRoom[0][6] = 51;
				loadedRoom[0][7] = 51;
				//Adds opened door to list
				openDoors.doorSeven = true;
				enemiesLoadedRoom = enemiesRoomThirteen;
			}
			break;
		case 18: //The only room the player can go to from 18 is 16. Loading that room
			LoadRoom(roomSixteen, 16, charYLoc, charXLoc, false);
			dungeonMap[2][4] = 43; //Setting player icon for room 16
			memcpy(loadedRoom, roomSixteen, sizeof(loadedRoom));
			//From 17 to 16 uses a locked door. This is the code to open the other side.
			loadedRoom[4][13] = 52;
			//Adds opened door to list
			openDoors.doorEight = true;
			enemiesLoadedRoom = enemiesRoomSixteen;
			break;
		default:
			cout << "Critcal Error: Could not change room. ChangeRoom()." << endl;
			Sleep(5000);
			exit(1);
			break;
		}
	}
	else if (room[charYLoc][charXLoc] == 47 && currentRoom == 16) {
		ResetBladeTraps();
		RunSound("stairs");
		memcpy(dungeonMap, masterDungeonMap, sizeof(dungeonMap)); //Resets dungeon map.
		allEnemiesCleared = false; //Resets the action door bool
		system("CLS");
		LoadRoom(roomEighteen, 18, charYLoc, charXLoc, false);
		memcpy(loadedRoom, roomEighteen, sizeof(loadedRoom));
	}
}

void LoadGame(bool loadFromSave, int saveNumber) { //Will need to be modified to allow the loading of Save files. Loading right from the master files may never happen again.
	if (saveFile == false) {

		enemiesRoomOne.resize(0);
		enemiesRoomTwo.resize(0);
		enemiesRoomThree.resize(0);
		enemiesRoomFour.resize(0);
		enemiesRoomFive.resize(0);
		enemiesRoomSix.resize(0);
		enemiesRoomSeven.resize(0);
		enemiesRoomEight.resize(0);
		enemiesRoomNine.resize(0);
		enemiesRoomTen.resize(0);
		enemiesRoomEleven.resize(0);
		enemiesRoomTwelve.resize(0);
		enemiesRoomThirteen.resize(0);
		enemiesRoomFourteen.resize(0);
		enemiesRoomFifteen.resize(0);
		enemiesRoomSixteen.resize(0);
		enemiesRoomSeventeen.resize(0);

		ifstream inFile;

		char input;
		int rows = 0; // Max 9 (8)
		int cols = 0; // Max 14 (13)

		//ROOM ONE
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\1-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room One" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {

			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\1-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\1-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\1-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room One From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}

		while (inFile >> input) {
			if (isspace(input) == false) {
				// 9 x 14
				roomOne[rows][cols] = input;

				if (input == 70) {
					enemiesRoomOne.push_back(Enemy("keese", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}

		inFile.close();
		//ROOM TWO
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\2-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Two" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\2-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\2-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\2-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Two From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomTwo[rows][cols] = input;
				//There are no enemies in this room.
				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}

		inFile.close();
		//ROOM THREE
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\3-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Three" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\3-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\3-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\3-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Three From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomThree[rows][cols] = input;

				if (input == 83) {
					enemiesRoomThree.push_back(Enemy("skeleton", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}

		inFile.close();

		//ROOM FOUR
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\4-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Four" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\4-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\4-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\4-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Four From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomFour[rows][cols] = input;

				if (input == 83) {
					enemiesRoomFour.push_back(Enemy("skeleton", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM FIVE
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\5-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Five" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\5-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\5-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\5-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Five From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomFive[rows][cols] = input;

				if (input == 70) {
					enemiesRoomFive.push_back(Enemy("keese", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM SIX
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\6-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Six" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\6-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\6-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\6-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Six From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomSix[rows][cols] = input;

				if (input == 83) {
					enemiesRoomSix.push_back(Enemy("skeleton", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM SEVEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\7-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Seven" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\7-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\7-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\7-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Seven From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomSeven[rows][cols] = input;

				if (input == 70) {
					enemiesRoomSeven.push_back(Enemy("keese", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM EIGHT
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\8-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eight" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\8-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\8-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\8-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eight From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomEight[rows][cols] = input;
				//There are no enemies in room eight
				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM NINE
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\9-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Nine" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\9-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\9-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\9-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Nine From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomNine[rows][cols] = input;

				if (input == 69) {
					enemiesRoomNine.push_back(Enemy("gel", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM TEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\10-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Ten" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\10-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\10-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\10-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Ten From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomTen[rows][cols] = input;

				if (input == 69) {
					enemiesRoomTen.push_back(Enemy("gel", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM ELEVEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\11-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eleven" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\11-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\11-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\11-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eleven From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomEleven[rows][cols] = input;

				if (input == 71) {
					enemiesRoomEleven.push_back(Enemy("goriya", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM TWELVE
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\12-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Twelve" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\12-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\12-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\12-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Twelve From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomTwelve[rows][cols] = input;
				//Room twelve does not use this system of loading in enemies from file
				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM THIRTEEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\13-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Thirteen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\13-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\13-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\13-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Thirteen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomThirteen[rows][cols] = input;

				if (input == 83) {
					enemiesRoomThirteen.push_back(Enemy("skeleton", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM FOURTEEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\14-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Fourteen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\14-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\14-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\14-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Fourteen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomFourteen[rows][cols] = input;
				//Room fourteen contains the boss
				if (input == 68) { //The Boss (Dragon)
					enemiesRoomFourteen.push_back(Enemy("dragon", rows, cols));
				}
				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM FIFTEEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\15-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Fifteen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\15-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\15-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\15-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Fifteen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomFifteen[rows][cols] = input;
				//Room fifteen does not have any enemies
				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM SIXTEEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\16-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Sixteen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\16-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\16-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\16-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Sixteen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomSixteen[rows][cols] = input;

				if (input == 43) { //43 ('+') for Blade Trap enemy
					enemiesRoomSixteen.push_back(Enemy("bladetrap", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM SEVENTEEN
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\17-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Seventeen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\17-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\17-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\17-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Seventeen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomSeventeen[rows][cols] = input;

				if (input == 71) {
					enemiesRoomSeventeen.push_back(Enemy("goriya", rows, cols));
				}

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();

		//ROOM EIGHTEEN (Item Room)
		rows = 0;
		cols = 0;
		if (loadFromSave == false) {
			inFile.open(mainDir + "rooms\\18-room.txt");
			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eighteen" << endl;
				cout << "Potential Fix: Delete " << mainDir << " and reinstall the essential files." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}
		}
		else {
			if (saveNumber == 1) {
				inFile.open(mainDir + "savedata\\1-save\\18-room.txt");
			}
			else if (saveNumber == 2) {
				inFile.open(mainDir + "savedata\\2-save\\18-room.txt");
			}
			else if (saveNumber == 3) {
				inFile.open(mainDir + "savedata\\3-save\\18-room.txt");
			}

			if (!inFile) {
				cout << "Critcal Error: Unable to load Room Eighteen From Save" << endl;
				cout << "Potential Fix: Create a New Save and Rewrite this one." << endl;
				cout << "Shutting down in 15 seconds." << endl;
				Sleep(15000);
				exit(1);
			}

		}
		while (inFile >> input) {
			if (isspace(input) == false) {
				roomEighteen[rows][cols] = input;

				cols++;
				if (cols == 14) {
					rows++;
					cols = 0;
				}
			}
		}
		inFile.close();
	}
}
  
void ResetGame(bool hardReset) { //Hardreset = true will restart the game. Hardreset = false will just respawn enemies and move the player the the dungeon start.
	
	if (hardReset == false) {
		charYLoc = startingLocY;
		charXLoc = startingLocX;
		currentRoom = startingRoom;
		LoadGame(false, 0);

		if (openDoors.doorOne == true) {
			roomTwo[0][6] = 51;
			roomTwo[0][7] = 51;
			roomFour[8][6] = 51;
			roomFour[8][7] = 51;
		}

		if (openDoors.doorTwo == true) {
			roomFive[0][6] = 51;
			roomFive[0][7] = 51;
			roomNine[8][6] = 51;
			roomNine[8][7] = 51;
		}

		if (openDoors.doorThree == true) {
			roomSix[0][6] = 51;
			roomSix[0][7] = 51;
			roomTen[8][6] = 51;
			roomTen[8][7] = 51;
		}

		if (openDoors.doorFour == true) {
			roomSeven[0][6] = 51;
			roomSeven[0][7] = 51;
			roomEleven[8][6] = 51;
			roomEleven[8][7] = 51;
		}

		if (openDoors.doorFive == true) {
			roomTen[4][13] = 52;
			roomEleven[4][0] = 52;
		}

		if (openDoors.doorSix == true) {
			roomTwelve[0][6] = 51;
			roomTwelve[0][7] = 51;
			roomFourteen[8][6] = 51;
			roomFourteen[8][7] = 51;
		}

		if (openDoors.doorSeven == true) {
			roomSeventeen[0][6] = 51;
			roomSeventeen[0][7] = 51;
			roomThirteen[8][6] = 51;
			roomThirteen[8][7] = 51;
		}

		if (openDoors.doorEight == true) {
			roomSixteen[4][13] = 52;
			roomSeventeen[4][0] = 52;
		}

		roomTwo[charYLoc][charXLoc] = 'P';
		LoadRoom(roomTwo, 2, charYLoc, charXLoc, true);
		memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));

		enemiesLoadedRoom = enemiesRoomTwo;

		dungeonMap[7][5] = 43;

	}
	else if (hardReset == true) {
		srand(time(0));
		ShowConsoleCursor();
		charYLoc = startingLocY;
		charXLoc = startingLocX;
		currentRoom = startingRoom;
		LoadGame(false, 0);
		roomTwo[charYLoc][charXLoc] = 'P';
		LoadRoom(roomTwo, 2, charYLoc, charXLoc, true);
		memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));

		enemiesLoadedRoom = enemiesRoomTwo;

		dungeonMap[7][5] = 43;

		gameRunning = true;

		counter = 0;

		swordActive = false;
		swordCounter = 0;
		swordDirection = 0;

		boomActive = false;

		arrowsLoadedRoom.clear();
		boomerangsLoadedRoom.clear();
		bombsInGame.clear();
		fireballsInGame.clear();

		//Resetting global variables
		dropped.roomOne = false;
		dropped.roomThree = false;
		dropped.roomSix = false;
		dropped.roomSeven = false;
		dropped.roomTen = false;
		dropped.roomEleven = false;
		dropped.roomTwelve = false;
		dropped.roomFourteen = false;
		dropped.roomSeventeen = false;

		openDoors.doorOne = false;
		openDoors.doorTwo = false;
		openDoors.doorThree = false;
		openDoors.doorFour = false;
		openDoors.doorFive = false;
		openDoors.doorSix = false;
		openDoors.doorSeven = false;
		openDoors.doorEight = false;

		maxWallMasters = 8;
		createdWallMasters = 0;
		wallMasterCounter = 0;
		keyW = false;
		keyA = false;
		keyS = false;
		keyD = false;
		keyB = false;
		keyPress = false;
		saveFile = false;
		lastDirection = 0; //Defaults to up.
		playerIsStunned = false;
		playerStunnedCounter = 0;
		enabledCheats = false;
		isInvincible = false;
		infBombs = false;
		infKeys = false;
		infCoins = false;
		disableWallMasters = false;
		roomNineBlockPushed = false;
		allEnemiesCleared = false; //Used to open the "action door" of the room once all enemies have been cleared.
		swordLocCols = 0;
		swordLocRows = 0;
		arrowCooldown = 0;
		currentLoadedSave = 0;

		playerScore = 0;

		if (isDevMode == true) {
			playerHealth = 99;
			playerKeys = 7;
			playerBombs = 99;
			playerCoins = 99;
			hasCompass = true;
			hasMap = true;
			hasBoomerang = true;
			hasBow = true;
		}
		else {
			playerHealth = 10;
			playerKeys = 0;
			playerBombs = 5;
			playerCoins = 0;
			hasCompass = false;
			hasMap = false;
			hasBoomerang = false;
			hasBow = false;
		}

	}
}
 
void DefaultAI(int room[9][14], int &currentCol, int &currentRow) {
	int value = 0;
	int countUp = 0;
	int countDown = 0;
	int countLeft = 0;
	int countRight = 0;
	bool wallUp = false;
	bool wallDown = false;
	bool wallLeft = false;
	bool wallRight = false;
	bool atAWall = false;

	if (room[currentCol][currentRow + 1] == 42) {
		wallRight = true;
		atAWall = true;
	}

	if (room[currentCol][currentRow - 1] == 42) {
		wallLeft = true;
		atAWall = true;
	}

	if (room[currentCol - 1][currentRow] == 42) {
		wallUp = true;
		atAWall = true;
	}

	if (room[currentCol + 1][currentRow] == 42) {
		wallDown = true;
		atAWall = true;
	}

	//cout << "Wall Up: " << wallUp << endl;
	//cout << "Wall Down: " << wallDown << endl;
	//cout << "Wall Left: " << wallLeft << endl;
	//cout << "Wall Right: " << wallRight << endl;
	//cout << "At a Wall: " << atAWall << endl;

	if(currentRow > charXLoc) { //Player is to the left
		if (wallLeft == true) {
			value = currentCol;
			value--;
			while (room[value][currentRow - 1] == 42) { //Calc route up
				countUp++;
				value--;
			}
			value = currentCol;
			value++;
			while (room[value][currentRow - 1] == 42) { //Calc route down
				countDown++;
				value++;
			}
			if (countUp > countDown) { //Go down
				currentCol++;
			}
			else { //Go up
				currentCol--;
			}
		}
		else {
			currentRow--;
		}
	}
	else if (currentRow < charXLoc) { //Player is to the right
		if (wallRight == true) {
			value = currentCol;
			value--;
			while (room[value][currentRow + 1] == 42) { //Calc route up
				countUp++;
				value--;
			}
			value = currentCol;
			value++;
			while (room[value][currentRow + 1] == 42) { //Calc route down
				countDown++;
				value++;
			}
			if (countUp > countDown) { //Go down
				currentCol++;
			}
			else { //Go up
				currentCol--;
			}
		}
		else {
			currentRow++;
		}
	}
	else if (currentCol < charYLoc) { //Player is to the down
		if (wallDown == true) {
			value = currentRow;
			value--;
			while (room[currentCol + 1][value] == 42) { //Calc route left
				countLeft++;
				value--;
			}
			value = currentRow;
			value++;
			while (room[currentCol + 1][value] == 42) { //Calc route right
				countRight++;
				value++;
			}
			if (countLeft < countRight) { //Go left
				currentRow--;
			}
			else { //Go right
				currentRow++;
			}
		}
		else {
			currentCol++;
		}
	}
	else if (currentCol > charYLoc) { //Player is to the up
		if (wallUp == true) {
			value = currentRow;
			value--;
			while (room[currentCol - 1][value] == 42) { //Calc route left
				countLeft++;
				value--;
			}
			value = currentRow;
			value++;
			while (room[currentCol - 1][value] == 42) { //Calc route right
				countRight++;
				value++;
			}
			if (countLeft < countRight) { //Go left
				currentRow--;
			}
			else { //Go right
				currentRow++;
			}
		}
		else {
			currentCol--;
		}
	}

} 
 
void KeeseAI(Enemy &eToMove, int &currentRow, int &currentCol) {

	if (eToMove.GetCollideWithWall() == true) {
		srand(time(0));
		int rN = rand() % 8;
		eToMove.SetCollideWithWall(false);
		if (rN == 0) {
			currentRow--;
			eToMove.SetCurrentDirection(0);
			return;
		}
		else if (rN == 1) {
			currentCol++;
			eToMove.SetCurrentDirection(1);
			return;
		}
		else if (rN == 2) {
			currentRow++;
			eToMove.SetCurrentDirection(2);
			return;
		}
		else if (rN == 3) {
			currentCol--;
			eToMove.SetCurrentDirection(3);
			return;
		}
		else if (rN == 4) {
			currentRow--;
			currentCol++;
			eToMove.SetCurrentDirection(4);
			return;
		}
		else if (rN == 5) {
			currentRow++;
			currentCol++;
			eToMove.SetCurrentDirection(5);
			return;
		}
		else if (rN == 6) {
			currentRow++;
			currentCol--;
			eToMove.SetCurrentDirection(6);
			return;
		}
		else if (rN == 7) {
			currentRow--;
			currentCol--;
			eToMove.SetCurrentDirection(7);
			return;
		}
	}
	else {
		if (eToMove.GetCurrentDirection() == 0) {
			currentRow--;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 1) {
			currentCol++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 2) {
			currentRow++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 3) {
			currentCol--;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 4) {
			currentRow--;
			currentCol++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 5) {
			currentRow++;
			currentCol++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 6) {
			currentRow++;
			currentCol--;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 7) {
			currentRow--;
			currentCol--;
			return;
		}
	}
}
 
void GoriyaAI(Enemy &eToMove, int &currentRow, int &currentCol) {
	bool isThrown = eToMove.GetBoomerangState();
	int enemyDir = eToMove.GetCurrentDirection();
	bool goingToThrow = false;
	bool throwLeft = false;
	bool throwRight = false;
	bool throwDown = false;
	bool throwUp = false;

	int constRow = currentRow;
	int constCol = currentCol;

	if (isThrown == true) {
		return;
	}

	if (constRow == charYLoc) {
		goingToThrow = true;
		if (constCol > charXLoc) { //Player is to the left
			throwLeft = true;
			int i = constCol;
			while (i > charXLoc) {
				i--;
				if (loadedRoom[constRow][i] != 48 && loadedRoom[constRow][i] != 80) {
					throwLeft = false;
				}
			}
		}
		else { //Player is to the right
			throwRight = true;
			int i = constCol;
			while (i < charXLoc) {
				i++;
				if (loadedRoom[constRow][i] != 48 && loadedRoom[constRow][i] != 80) {
					throwRight = false;
				}
			}
		}
	}
	else if (constCol == charXLoc) {
		goingToThrow = true;
		if (constRow > charYLoc) { //Player up
			throwUp = true;
			int i = constRow;
			while (i > charYLoc) {
				i--;
				if (loadedRoom[i][constCol] != 48 && loadedRoom[i][constCol] != 80) {
					throwUp = false;
				}
			}
		}
		else { //Player is below
			throwDown = true;
			int i = constRow;
			while (i < charYLoc) {
				i++;
				if (loadedRoom[i][constCol] != 48 && loadedRoom[i][constCol] != 80) {
					throwDown = false;
				}
			}
		}
	}

	//cout << "Current Dir: " << enemyDir << endl;

	if (goingToThrow == true) {
		if (throwLeft == true && enemyDir == 3) {
			if (loadedRoom[constRow][constCol - 1] == 80) {
				currentCol--;
				eToMove.SetCurrentDirection(3);
				return;
			}
			else if(playerIsStunned == false) {
				if (loadedRoom[constRow][constCol - 1] == 48) {
					boomerangsLoadedRoom.push_back(Boomerang(constRow, constCol - 1, currentRoom, enemyDir, eToMove, constRow, constCol));
					eToMove.SetBoomerangState(true);
					loadedRoom[constRow][constCol - 1] = 236;
					return;
				}
				else {
					eToMove.SetCurrentDirection(0);
					return;
				}
			}
			else {
				currentCol--;
				eToMove.SetCurrentDirection(3);
				return;
			}
		}
		else if (throwRight == true && enemyDir == 1) {
			if (loadedRoom[constRow][constCol + 1] == 80) {
				currentCol++;
				eToMove.SetCurrentDirection(1);
				return;
			}
			else if (playerIsStunned == false) {
				if (loadedRoom[constRow][constCol + 1] == 48) {
					boomerangsLoadedRoom.push_back(Boomerang(constRow, constCol + 1, currentRoom, enemyDir, eToMove, constRow, constCol));
					eToMove.SetBoomerangState(true);
					loadedRoom[constRow][constCol + 1] = 236;
					return;
				}
				else {
					eToMove.SetCurrentDirection(2);
					return;
				}
			}
			else {
				currentCol++;
				eToMove.SetCurrentDirection(1);
				return;
			}
		}
		else if (throwDown == true && enemyDir == 2) {
			if (loadedRoom[constRow + 1][constCol] == 80) {
				currentRow++;
				eToMove.SetCurrentDirection(2);
				return;
			}
			else if(playerIsStunned == false) {
				if (loadedRoom[constRow + 1][constCol] == 48) {
					boomerangsLoadedRoom.push_back(Boomerang(constRow + 1, constCol, currentRoom, enemyDir, eToMove, constRow, constCol));
					eToMove.SetBoomerangState(true);
					loadedRoom[constRow + 1][constCol] = 236;
					return;
				}
				else {
					eToMove.SetCurrentDirection(3);
					return;
				}
			}
			else {
				currentRow++;
				eToMove.SetCurrentDirection(2);
				return;
			}
		}
		else if (throwUp == true && enemyDir == 0) {
			if (loadedRoom[constRow - 1][constCol] == 80) {
				currentRow--;
				eToMove.SetCurrentDirection(0);
				return;
			}
			else if(playerIsStunned == false) {
				if (loadedRoom[constRow - 1][constCol] == 48) {
					boomerangsLoadedRoom.push_back(Boomerang(constRow - 1, constCol, currentRoom, enemyDir, eToMove, constRow, constCol));
					eToMove.SetBoomerangState(true);
					loadedRoom[constRow - 1][constCol] = 236;
					return;
				}
				else {
					eToMove.SetCurrentDirection(1);
					return;
				}
			}
			else {
				currentRow--;
				eToMove.SetCurrentDirection(0);
				return;
			}
		}
		else {
			//cout << "Player not in sight";
		}
	}

	if (eToMove.GetCollideWithWall() == true) {
		srand(time(0));
		int rN = rand() % 4;
		eToMove.SetCollideWithWall(false);
		if (rN == 0) {
			currentRow--;
			eToMove.SetCurrentDirection(0);
			return;
		}
		else if (rN == 1) {
			currentCol++;
			eToMove.SetCurrentDirection(1);
			return;
		}
		else if (rN == 2) {
			currentRow++;
			eToMove.SetCurrentDirection(2);
			return;
		}
		else if (rN == 3) {
			currentCol--;
			eToMove.SetCurrentDirection(3);
			return;
		}
	}
	else {
		if (eToMove.GetCurrentDirection() == 0) {
			currentRow--;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 1) {
			currentCol++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 2) {
			currentRow++;
			return;
		}
		else if (eToMove.GetCurrentDirection() == 3) {
			currentCol--;
			return;
		}
	}
}

void DragonAI(Enemy theEnemy, int &currentRow, int &currentCol) {

	int direction = theEnemy.GetCurrentDirection();

	if (currentCol == 10) { //Set the dragon to move forwards
		direction = 3;
	}
	else if (currentCol == 7) { //Set the dragon to move backwards
		direction = 1;
	}

	if (direction == 3) { //Move the dragon forwards
		currentCol--;
		//return;
	}
	else if (direction == 1) { //Move the dragon backwards
		currentCol++;
		//return;
	}
	else {
		cout << "Error. Dragon movement error. DragonAI()." << endl;
	}


	if (dragonShotTimer > 4) {
		dragonShotTimer = 0;
		int eRows = theEnemy.GetLocationRows();
		int eCols = theEnemy.GetLocationCols();

		int chance = rand() % 100 + 1;

		if (chance >= 50) {
			RunSound("bossScreamOne");
		}
		else {
			RunSound("bossScreamTwo");
		}

		if (direction == 3) { //Moving forwards
			if (charYLoc < eRows) { //The player is above the enemy.
				fireballsInGame.push_back(Fireball((eRows - 1), (eCols - 2), 1, false)); //Create new fireball upwards
			}
			else if (charYLoc > eRows) { //The player is below the enemy.
				fireballsInGame.push_back(Fireball((eRows + 1), (eCols - 2), 1, false)); //Create new fireball downwards
			}
			else if (charYLoc == eRows) { //The player is on the same level as the enemy.
				fireballsInGame.push_back(Fireball((eRows), (eCols - 2), 1, false)); //Create new fireball on the same level
			}
		}
		else { //Moving backwards
			if (charYLoc < eRows) { //The player is above the enemy.
				fireballsInGame.push_back(Fireball((eRows - 1), (eCols - 1), 1, false)); //Create new fireball upwards
			}
			else if (charYLoc > eRows) { //The player is below the enemy.
				fireballsInGame.push_back(Fireball((eRows + 1), (eCols - 1), 1, false)); //Create new fireball downwards
			}
			else if (charYLoc == eRows) { //The player is on the same level as the enemy.
				fireballsInGame.push_back(Fireball((eRows), (eCols - 1), 1, false)); //Create new fireball on the same level
			}
		}

	}
	else {
		dragonShotTimer++;
	}

}

void SmartAI(Enemy theEnemy, int &currentRow, int &currentCol) {

	int currentEnemyRows = theEnemy.GetLocationRows();
	int currentEnemyCols = theEnemy.GetLocationCols();
	int playerRows = charYLoc;
	int playerCols = charXLoc;
	int lastDir = theEnemy.GetLastDirection();

	double distanceUp = sqrt(pow(playerRows - (currentEnemyRows - 1), 2) + pow(playerCols - currentEnemyCols, 2));
	double distanceDown = sqrt(pow(playerRows - (currentEnemyRows + 1), 2) + pow(playerCols - currentEnemyCols, 2));
	double distanceLeft = sqrt(pow(playerRows - currentEnemyRows, 2) + pow(playerCols - (currentEnemyCols - 1), 2));
	double distanceRight = sqrt(pow(playerRows - currentEnemyRows, 2) + pow(playerCols - (currentEnemyCols + 1), 2));

	int bestDirection = -1;
	int theorBestDirection = -1;
	int secondBestDirection = -1;
	int theorSecondBestDirection = -1;
	int thirdBestDirection = -1;
	int theorThirdBestDirection = -1;
	int fourthBestDirection = -1;

	//Get the best direction
	if (distanceUp <= distanceDown && distanceUp <= distanceLeft && distanceUp <= distanceRight) {
		theorBestDirection = 0;
		if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
			bestDirection = 0;
		}
	}
	else if (distanceDown <= distanceUp && distanceDown <= distanceLeft && distanceDown <= distanceRight) {
		theorBestDirection = 2;
		if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
			bestDirection = 2;
		}
	}
	else if (distanceLeft <= distanceUp && distanceLeft <= distanceDown && distanceLeft <= distanceRight) {
		theorBestDirection = 3;
		if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
			bestDirection = 3;
		}
	}
	else if (distanceRight <= distanceUp && distanceRight <= distanceDown && distanceRight <= distanceLeft) {
		theorBestDirection = 1;
		if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
			bestDirection = 1;
		}
	}

	//If you couldnt get that.
	if (bestDirection == -1) {
		if (theorBestDirection == 0) {
			if (distanceDown <= distanceLeft && distanceDown <= distanceRight) {
				theorSecondBestDirection = 2;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					secondBestDirection = 2;
				}
			}
			else if (distanceLeft <= distanceDown && distanceLeft <= distanceRight) {
				theorSecondBestDirection = 3;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					secondBestDirection = 3;
				}
			}
			else if (distanceRight <= distanceDown && distanceRight <= distanceLeft) {
				theorSecondBestDirection = 1;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					secondBestDirection = 1;
				}
			}
		}
		else if (theorBestDirection == 1) {
			if (distanceUp <= distanceDown && distanceUp <= distanceLeft) {
				theorSecondBestDirection = 0;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					secondBestDirection = 0;
				}
			}
			else if (distanceDown <= distanceUp && distanceDown <= distanceLeft) {
				theorSecondBestDirection = 2;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					secondBestDirection = 2;
				}
			}
			else if (distanceLeft <= distanceUp && distanceLeft <= distanceDown) {
				theorSecondBestDirection = 3;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					secondBestDirection = 3;
				}
			}
		}
		else if (theorBestDirection == 2) {
			if (distanceUp <= distanceLeft && distanceUp <= distanceRight) {
				theorSecondBestDirection = 0;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					secondBestDirection = 0;
				}
			}
			else if (distanceLeft <= distanceUp && distanceLeft <= distanceRight) {
				theorSecondBestDirection = 3;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					secondBestDirection = 3;
				}
			}
			else if (distanceRight <= distanceUp && distanceRight <= distanceLeft) {
				theorSecondBestDirection = 1;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					secondBestDirection = 1;
				}
			}
		}
		else if (theorBestDirection == 3) {
			if (distanceUp <= distanceDown && distanceUp <= distanceRight) {
				theorSecondBestDirection = 0;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					secondBestDirection = 0;
				}
			}
			else if (distanceDown <= distanceUp && distanceDown <= distanceRight) {
				theorSecondBestDirection = 2;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					secondBestDirection = 2;
				}
			}
			else if (distanceRight <= distanceUp && distanceRight <= distanceDown) {
				theorSecondBestDirection = 1;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					secondBestDirection = 1;
				}
			}
		}
	}

	//If you really couldnt get that: Third best option AND fourth
	if (secondBestDirection == -1) {
		if (theorBestDirection == 0 && theorSecondBestDirection == 1) {
			if (distanceDown <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
			else if (distanceLeft <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}

			}
		}
		else if (theorBestDirection == 0 && theorSecondBestDirection == 2) {
			if (distanceLeft <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}
			}
			else if (distanceRight <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}
		else if (theorBestDirection == 0 && theorSecondBestDirection == 3) {
			if (distanceDown <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
			else if (distanceRight <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}

		else if (theorBestDirection == 1 && theorSecondBestDirection == 0) {
			if (distanceDown <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
			else if (distanceLeft <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}
			}
		}
		else if (theorBestDirection == 1 && theorSecondBestDirection == 2) {
			if (distanceUp <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceLeft <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}
			}
		}
		else if (theorBestDirection == 1 && theorSecondBestDirection == 3) {
			if (distanceUp <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceDown <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
		}

		else if (theorBestDirection == 2 && theorSecondBestDirection == 0) {
			if (distanceLeft <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}
			}
			else if (distanceRight <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}
		else if (theorBestDirection == 2 && theorSecondBestDirection == 1) {
			if (distanceUp <= distanceLeft) {
				fourthBestDirection = 3;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceLeft <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80) && lastDir != 1) {
					thirdBestDirection = 3;
				}
			}
		}
		else if (theorBestDirection == 2 && theorSecondBestDirection == 3) {
			if (distanceUp <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceRight <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}

		else if (theorBestDirection == 3 && theorSecondBestDirection == 0) {
			if (distanceDown <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
			else if (distanceRight <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}
		else if (theorBestDirection == 3 && theorSecondBestDirection == 1) {
			if (distanceUp <= distanceDown) {
				fourthBestDirection = 2;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceDown <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80) && lastDir != 0) {
					thirdBestDirection = 2;
				}
			}
		}
		else if (theorBestDirection == 3 && theorSecondBestDirection == 2) {
			if (distanceUp <= distanceRight) {
				fourthBestDirection = 1;
				if ((loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80) && lastDir != 2) {
					thirdBestDirection = 0;
				}
			}
			else if (distanceRight <= distanceUp) {
				fourthBestDirection = 0;
				if ((loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80) && lastDir != 3) {
					thirdBestDirection = 1;
				}
			}
		}
	}

	if (bestDirection != -1) {
		if (bestDirection == 0) {
			currentRow--;
			return;
		}
		else if (bestDirection == 1) {
			currentCol++;
			return;
		}
		else if (bestDirection == 2) {
			currentRow++;
			return;
		}
		else if (bestDirection == 3) {
			currentCol--;
			return;
		}
	}
	else if (secondBestDirection != -1) {
		if (secondBestDirection == 0) {
			currentRow--;
			return;
		}
		else if (secondBestDirection == 1) {
			currentCol++;
			return;
		}
		else if (secondBestDirection == 2) {
			currentRow++;
			return;
		}
		else if (secondBestDirection == 3) {
			currentCol--;
			return;
		}
	}
	else if (thirdBestDirection != -1) {
		if (thirdBestDirection == 0) {
			currentRow--;
			return;
		}
		else if (thirdBestDirection == 1) {
			currentCol++;
			return;
		}
		else if (thirdBestDirection == 2) {
			currentRow++;
			return;
		}
		else if (thirdBestDirection == 3) {
			currentCol--;
			return;
		}
	}
	else {
		if (fourthBestDirection == 0 && (loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows - 1][currentEnemyCols] == 80)) {
			currentRow--;
			return;
		}
		else if (fourthBestDirection == 1 && (loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols + 1] == 80)) {
			currentCol++;
			return;
		}
		else if (fourthBestDirection == 2 && (loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 48 || loadedRoom[currentEnemyRows + 1][currentEnemyCols] == 80)) {
			currentRow++;
			return;
		}
		else if (fourthBestDirection == 3 && (loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 48 || loadedRoom[currentEnemyRows][currentEnemyCols - 1] == 80)) {
			currentCol--;
			return;
		}
	}


}

void RunFireballs() {
	for (int i = 0; i < fireballsInGame.size(); i++) {
		if (fireballsInGame.at(i).GetSplitStatus() == false) {
			fireballsInGame.at(i).SetSplitStatus(true);

			int ballRows = fireballsInGame.at(i).GetLocationRows();
			int ballCols = fireballsInGame.at(i).GetLocationCols();

			fireballsInGame.push_back(Fireball(ballRows - 1, ballCols, 0, true));
			fireballsInGame.push_back(Fireball(ballRows + 1, ballCols, 2, true));

		}

		if (fireballsInGame.at(i).Move() == false) {
			int ballRows = fireballsInGame.at(i).GetLocationRows();
			int ballCols = fireballsInGame.at(i).GetLocationCols();
			int ballNumber = fireballsInGame.at(i).GetBallNumber();
			bool spread = fireballsInGame.at(i).GetSpread();

			if (spread == false) {
				if (ballNumber == 0) { //If its the top fireball
					loadedRoom[ballRows + 1][ballCols + 1] = 48;
				}
				else if (ballNumber == 1) { //If its the middle fireball
					loadedRoom[ballRows][ballCols + 1] = 48;
				}
				else if (ballNumber == 2) { //If its the bottom fireball
					loadedRoom[ballRows - 1][ballCols + 1] = 48;
				}
			}
			else {
				loadedRoom[ballRows][ballCols + 1] = 48;
			}

			fireballsInGame.erase(fireballsInGame.begin() + i);

		}

	}
}

void EnemiesCleared(bool allEnemiesCleared) {
	if (allEnemiesCleared == true) {
		switch (currentRoom) {
		case 1: //For dropping a key
			if (dropped.roomOne == false) {
				if (loadedRoom[7][9] == 48) {
					loadedRoom[7][9] = 'K';
					RunSound("keyAppear");
					dropped.roomOne = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 3: //For dropping a key
			if (dropped.roomThree == false) {
				if (loadedRoom[6][4] == 48) {
					loadedRoom[6][4] = 'K';
					RunSound("keyAppear");
					dropped.roomThree = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 5: //For opening a door
			loadedRoom[4][13] = 52;
			allEnemiesCleared = false;
			break;
		case 6: //For dropping a key
			if (dropped.roomSix == false) {
				if (loadedRoom[2][7] == 48) {
					loadedRoom[2][7] = 'K';
					RunSound("keyAppear");
					dropped.roomSix = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 7: //For dropping the compass
			if (dropped.roomSeven == false) {
				if (loadedRoom[4][11] == 48) {
					loadedRoom[4][11] = 'C';
					dropped.roomSeven = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 9: //For opening a door
			if (roomNineBlockPushed == true) { //MUST push the block in room nine in order to open the door.
				loadedRoom[4][0] = 52;
				RunSound("secretTheme");
				allEnemiesCleared = false;
				roomNineBlockPushed = false;
			}
			break;
		case 10: //For dropping the map
			if (dropped.roomTen == false) {
				if (loadedRoom[4][11] == 48) {
					loadedRoom[4][11] = 'M';
					dropped.roomTen = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 11: //For dropping the boomerang
			if (dropped.roomEleven == false) {
				if (loadedRoom[4][11] == 48) {
					loadedRoom[2][7] = 235; //Drop the Boomerang (235 is the item on ground version)
					dropped.roomEleven = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 12: //For dropping a key
			if (dropped.roomTwelve == false && createdWallMasters >= 8) {
				if (loadedRoom[7][9] == 48) {
					loadedRoom[7][9] = 'K';
					RunSound("keyAppear");
					dropped.roomTwelve = true;
					allEnemiesCleared = false;
				}
			}
			break;
		case 14: //For opening a door AND dropping a heart container
			if (dropped.roomFourteen == false) {
				loadedRoom[4][13] = 52;
				if (dropped.roomFourteen == false) {
					loadedRoom[4][11] = 'H';
					dropped.roomFourteen = true;
				}
				allEnemiesCleared = false;
			}
			break;
		case 17: //For dropping a key
			if (dropped.roomSeventeen == false) {
				if (loadedRoom[2][7] == 48) {
					loadedRoom[2][7] = 'K';
					RunSound("keyAppear");
					dropped.roomSeventeen = true;
				}
			}
			break;
		default:
			allEnemiesCleared = false;
			break;
		}
	}
}

void CheckDrop() {
	//Decides if action should be completed.

	if (enemiesLoadedRoom.size() == 0) {
		EnemiesCleared(true);
	}
}

void WallMasterAttack(Enemy theEnemy, int currentRow, int currentCol) {

	if (loadedRoom[currentRow][currentCol] == 80 && (currentRow != 0 && currentRow != 8) && (currentCol != 0 && currentCol != 13)) {
		if (disableWallMasters == false) {
			system("CLS");
			cout << "You have been grabbed by a Wallmaster!" << endl;
			cout << "It has returned you to the start of the dungeon!" << endl;
			if (isDevMode == false) { //Only reset the dungeon if not in dev mode.
				Sleep(2500);
				ResetGame(false); //Taking the player back to the start and soft resetting the dungeon because they were touched by a wallmaster.
			}
			else {
				Sleep(1000);
			}
			system("CLS");
		}
	}
	else if (loadedRoom[currentRow][currentCol] == 80) { //If the wallmaster is still in the wall, just attack the player, don't reset. We aren't going to be THAT mean.
		playerHealth--;
	}
}

void TryCreateWallMaster() {

	if (currentRoom != 12 || createdWallMasters >= 8) {
		return;
	}

	vector<int> possibleLocRows(6);
	vector<int> possibleLocCols(6);

	int disUp = 0;
	int disDown = 0;
	int disLeft = 0;
	int disRight = 0;

	int rows = charYLoc;
	int cols = charXLoc;

	while (loadedRoom[rows][charXLoc] != 49 && loadedRoom[rows][charXLoc] != 51 && loadedRoom[rows][charXLoc] != 61 && loadedRoom[rows][charXLoc] != 45 && loadedRoom[rows][charXLoc] != 87 && loadedRoom[rows][charXLoc] != 179 && loadedRoom[rows][charXLoc] != 66 && loadedRoom[rows][charXLoc] != 48) {
		disUp++;
		rows--;
	}

	rows = charYLoc;
	cols = charXLoc;
	while (loadedRoom[rows][charXLoc] != 49 && loadedRoom[rows][charXLoc] != 51 && loadedRoom[rows][charXLoc] != 61 && loadedRoom[rows][charXLoc] != 45 && loadedRoom[rows][charXLoc] != 87 && loadedRoom[rows][charXLoc] != 179 && loadedRoom[rows][charXLoc] != 66 && loadedRoom[rows][charXLoc] != 48) {
		disDown++;
		rows++;
	}

	rows = charYLoc;
	cols = charXLoc;
	while (loadedRoom[charYLoc][cols] != 50 && loadedRoom[charYLoc][cols] != 52 && loadedRoom[charYLoc][cols] != 124 && loadedRoom[charYLoc][cols] != 87 && loadedRoom[charYLoc][cols] != 196 && loadedRoom[charYLoc][cols] != 66 && loadedRoom[charYLoc][cols] != 48) {
		disLeft++;
		cols--;
	}

	rows = charYLoc;
	cols = charXLoc;
	while (loadedRoom[charYLoc][cols] != 50 && loadedRoom[charYLoc][cols] != 52 && loadedRoom[charYLoc][cols] != 124 && loadedRoom[charYLoc][cols] != 87 && loadedRoom[charYLoc][cols] != 196 && loadedRoom[charYLoc][cols] != 66 && loadedRoom[charYLoc][cols] != 48) {
		disRight++;
		cols++;
	}

	if (disUp <= disDown && disUp <= disLeft && disUp <= disRight) {
		possibleLocRows.at(0) = 0;
		possibleLocCols.at(0) = charXLoc - 1;

		possibleLocRows.at(1) = 0;
		possibleLocCols.at(1) = charXLoc;

		possibleLocRows.at(2) = 0;
		possibleLocCols.at(2) = charXLoc + 1;

		disUp = 999;
	}
	else if (disDown <= disUp && disDown <= disLeft && disDown <= disRight) {
		possibleLocRows.at(0) = 8;
		possibleLocCols.at(0) = charXLoc - 1;

		possibleLocRows.at(1) = 8;
		possibleLocCols.at(1) = charXLoc;

		possibleLocRows.at(2) = 8;
		possibleLocCols.at(2) = charXLoc + 1;

		disDown = 999;
	}
	else if (disLeft <= disUp && disLeft <= disDown && disLeft <= disRight) {
		possibleLocRows.at(0) = charYLoc - 1;
		possibleLocCols.at(0) = 0;

		possibleLocRows.at(1) = charYLoc;
		possibleLocCols.at(1) = 0;

		possibleLocRows.at(2) = charYLoc + 1;
		possibleLocCols.at(2) = 0;

		disLeft = 999;
	}
	else if (disRight <= disUp && disRight <= disDown && disRight <= disLeft) {
		possibleLocRows.at(0) = charYLoc - 1;
		possibleLocCols.at(0) = 13;

		possibleLocRows.at(1) = charYLoc;
		possibleLocCols.at(1) = 13;

		possibleLocRows.at(2) = charYLoc + 1;
		possibleLocCols.at(2) = 13;

		disRight = 999;
	}

	if (disUp <= disDown && disUp <= disLeft && disUp <= disRight) {
		possibleLocRows.at(3) = 0;
		possibleLocCols.at(3) = charXLoc - 1;

		possibleLocRows.at(4) = 0;
		possibleLocCols.at(4) = charXLoc;

		possibleLocRows.at(5) = 0;
		possibleLocCols.at(5) = charXLoc + 1;

		disUp = 999;
	}
	else if (disDown <= disUp && disDown <= disLeft && disDown <= disRight) {
		possibleLocRows.at(3) = 8;
		possibleLocCols.at(3) = charXLoc - 1;

		possibleLocRows.at(4) = 8;
		possibleLocCols.at(4) = charXLoc;

		possibleLocRows.at(5) = 8;
		possibleLocCols.at(5) = charXLoc + 1;

		disDown = 999;
	}
	else if (disLeft <= disUp && disLeft <= disDown && disLeft <= disRight) {
		possibleLocRows.at(3) = charYLoc - 1;
		possibleLocCols.at(3) = 0;

		possibleLocRows.at(4) = charYLoc;
		possibleLocCols.at(4) = 0;

		possibleLocRows.at(5) = charYLoc + 1;
		possibleLocCols.at(5) = 0;

		disLeft = 999;
	}
	else if (disRight <= disUp && disRight <= disDown && disRight <= disLeft) {
		possibleLocRows.at(3) = charYLoc - 1;
		possibleLocCols.at(3) = 13;

		possibleLocRows.at(4) = charYLoc;
		possibleLocCols.at(4) = 13;

		possibleLocRows.at(5) = charYLoc + 1;
		possibleLocCols.at(5) = 13;

		disRight = 999;
	}

	if (createdWallMasters < 8) {
		for (int i = 0; i < 6; i++) {
			if (possibleLocRows.at(i) >= 0 && possibleLocRows.at(i) <= 8 && possibleLocCols.at(i) >= 0 && possibleLocCols.at(i) <= 13) { //Checks for valid location
				if (loadedRoom[possibleLocRows.at(i)][possibleLocCols.at(i)] == 49 || loadedRoom[possibleLocRows.at(i)][possibleLocCols.at(i)] == 50) { //Checks to make sure its a wall piece.
					int chance = rand() % 100 + 1;

					if (chance <= 5) {
						loadedRoom[possibleLocRows.at(i)][possibleLocCols.at(i)] = 'W';

						enemiesLoadedRoom.push_back(Enemy("wallmaster", possibleLocRows.at(i), possibleLocCols.at(i)));

						createdWallMasters++;
					}
				}
			}
		}
	}

}

void BladeTrapAI() {
	if (currentRoom == 16) {

		//BLADE SET 0

		if (bladeSet.at(0) == 0) { //If blade set zero is not moving
			if (charYLoc == 1 && (charXLoc >= 2 && charXLoc <= 11)) {
				bladeSet.at(0) = 1; //Blade is now in moving state
			}
		}
		else if (bladeSet.at(0) == 1) { //If blade set zero is in moving pos

			if (bladeSingleMoving.at(0) == 4 || bladeSingleMoving.at(0) == 0) {
				bladeSingleMoving.at(0) = 0;
				if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() + 1] == 48) {

					loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(0).SetLocationCols(enemiesRoomSixteen.at(0).GetLocationCols() + 1);
					loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() + 1] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(0) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() + 1] == 43 || loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() + 1] == 50) {
					bladeSet.at(0) = 2;
				}
			}

			if (bladeSingleMoving.at(1) == 4 || bladeSingleMoving.at(1) == 0) {
				bladeSingleMoving.at(1) = 0;
				if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() - 1] == 48) {

					loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(1).SetLocationCols(enemiesRoomSixteen.at(1).GetLocationCols() - 1);
					loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() - 1] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(0) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() - 1] == 43 || loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() - 1] == 50) {
					bladeSet.at(0) = 2;
				}
			}
		}
		else if (bladeSet.at(0) == 2) { //Get to OG spots

			if (bladeReturnCounter.at(0) < 4) {
				bladeReturnCounter.at(0)++;
			}
			else {

				if (bladeSingleMoving.at(0) == 4 || bladeSingleMoving.at(0) == 0) {
					if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() - 1] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() - 1] != 50 && loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols() - 1] != 42) {

						loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(0).SetLocationCols(enemiesRoomSixteen.at(0).GetLocationCols() - 1);
						loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 43;
					}
				}

				if (bladeSingleMoving.at(1) == 4 || bladeSingleMoving.at(1) == 0) {
					if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() + 1] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() + 1] != 50 && loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols() + 1] != 42) {
						loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(1).SetLocationCols(enemiesRoomSixteen.at(1).GetLocationCols() + 1);
						loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 43;
					}

					if (enemiesRoomSixteen.at(0).GetLocationCols() == 1 && enemiesRoomSixteen.at(1).GetLocationCols() == 11) { //Both traps are in OG pos
						bladeSet.at(0) = 0;
						bladeSingleMoving.at(0) = 4;
						bladeSingleMoving.at(1) = 4;
					}
				}
				bladeReturnCounter.at(0) = 0;
			}
		}

		//BLADE SET 1

		if (bladeSet.at(1) == 0) { //If blade set one is not moving
			if (charXLoc == 11 && (charYLoc >= 2 && charYLoc <= 6)) {
				bladeSet.at(1) = 1; //Blade is now in moving state
			}
		}
		else if (bladeSet.at(1) == 1) { //If blade set one is in moving pos
			if (bladeSingleMoving.at(1) == 4 || bladeSingleMoving.at(1) == 1) {
				bladeSingleMoving.at(1) = 1;
				if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() + 1][enemiesRoomSixteen.at(1).GetLocationCols()] == 48) {

					loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(1).SetLocationRows(enemiesRoomSixteen.at(1).GetLocationRows() + 1);
					loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() + 1][enemiesRoomSixteen.at(1).GetLocationCols()] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(1) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() + 1][enemiesRoomSixteen.at(1).GetLocationCols()] == 43 || loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() + 1][enemiesRoomSixteen.at(1).GetLocationCols()] == 49) {
					bladeSet.at(1) = 2;
				}
			}
			if (bladeSingleMoving.at(3) == 4 || bladeSingleMoving.at(3) == 1) {
				bladeSingleMoving.at(3) = 1;
				if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() - 1][enemiesRoomSixteen.at(3).GetLocationCols()] == 48) {

					loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(3).SetLocationRows(enemiesRoomSixteen.at(3).GetLocationRows() - 1);
					loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() - 1][enemiesRoomSixteen.at(3).GetLocationCols()] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(1) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() - 1][enemiesRoomSixteen.at(3).GetLocationCols()] == 43 || loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() - 1][enemiesRoomSixteen.at(3).GetLocationCols()] == 49) {
					bladeSet.at(1) = 2;
				}
			}
		}
		else if (bladeSet.at(1) == 2) { //Get to OG spots

			if (bladeReturnCounter.at(1) < 4) {
				bladeReturnCounter.at(1)++;
			}
			else {
				if (bladeSingleMoving.at(1) == 4 || bladeSingleMoving.at(1) == 1) {
					if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() - 1][enemiesRoomSixteen.at(1).GetLocationCols()] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows() - 1][enemiesRoomSixteen.at(1).GetLocationCols()] != 49) {

						loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(1).SetLocationRows(enemiesRoomSixteen.at(1).GetLocationRows() - 1);
						loadedRoom[enemiesRoomSixteen.at(1).GetLocationRows()][enemiesRoomSixteen.at(1).GetLocationCols()] = 43;
					}
				}

				if (bladeSingleMoving.at(3) == 4 || bladeSingleMoving.at(3) == 1) {
					if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() + 1][enemiesRoomSixteen.at(3).GetLocationCols()] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows() + 1][enemiesRoomSixteen.at(3).GetLocationCols()] != 49) {
						loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(3).SetLocationRows(enemiesRoomSixteen.at(3).GetLocationRows() + 1);
						loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 43;
					}

					if (enemiesRoomSixteen.at(1).GetLocationRows() == 1 && enemiesRoomSixteen.at(3).GetLocationRows() == 7) { //Both traps are in OG pos
						bladeSet.at(1) = 0;
						bladeSingleMoving.at(1) = 4;
						bladeSingleMoving.at(3) = 4;
					}
				}

				bladeReturnCounter.at(1) = 0;
			}
		}
		
		//BLADE SET 2

		if (bladeSet.at(2) == 0) { //If blade set two is not moving
			if (charYLoc == 7 && (charXLoc >= 2 && charXLoc <= 11)) {
				bladeSet.at(2) = 1; //Blade is now in moving state
			}
		}
		else if (bladeSet.at(2) == 1) { //If blade set two is in moving pos

			if (bladeSingleMoving.at(2) == 4 || bladeSingleMoving.at(2) == 2) {
				bladeSingleMoving.at(2) = 2;
				if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() + 1] == 48) {

					loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(2).SetLocationCols(enemiesRoomSixteen.at(2).GetLocationCols() + 1);
					loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() + 1] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(2) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() + 1] == 43 || loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() + 1] == 50) {
					bladeSet.at(2) = 2;
				}
			}

			if (bladeSingleMoving.at(3) == 4 || bladeSingleMoving.at(3) == 2) {
				bladeSingleMoving.at(3) = 2;
				if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() - 1] == 48) {

					loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(3).SetLocationCols(enemiesRoomSixteen.at(3).GetLocationCols() - 1);
					loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() - 1] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(2) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() - 1] == 43 || loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() - 1] == 50) {
					bladeSet.at(2) = 2;
				}
			}
		}
		else if (bladeSet.at(2) == 2) { //Get to OG spots

			if (bladeReturnCounter.at(2) < 4) {
				bladeReturnCounter.at(2)++;
			}
			else {

				if (bladeSingleMoving.at(2) == 4 || bladeSingleMoving.at(2) == 2) {
					if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() - 1] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() - 1] != 50 && loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols() - 1] != 42) {

						loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(2).SetLocationCols(enemiesRoomSixteen.at(2).GetLocationCols() - 1);
						loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 43;
					}
				}

				if (bladeSingleMoving.at(3) == 4 || bladeSingleMoving.at(3) == 2) {
					if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() + 1] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() + 1] != 50 && loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols() + 1] != 42) {
						loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(3).SetLocationCols(enemiesRoomSixteen.at(3).GetLocationCols() + 1);
						loadedRoom[enemiesRoomSixteen.at(3).GetLocationRows()][enemiesRoomSixteen.at(3).GetLocationCols()] = 43;
					}

					if (enemiesRoomSixteen.at(2).GetLocationCols() == 1 && enemiesRoomSixteen.at(3).GetLocationCols() == 11) { //Both traps are in OG pos
						bladeSet.at(2) = 0;
						bladeSingleMoving.at(2) = 4;
						bladeSingleMoving.at(3) = 4;
					}
				}
				bladeReturnCounter.at(2) = 0;
			}
		}
		
		//BLADE SET 3
		
		if (bladeSet.at(3) == 0) { //If blade set three is not moving
			if (charXLoc == 1 && (charYLoc >= 2 && charYLoc <= 6)) {
				bladeSet.at(3) = 1; //Blade is now in moving state
			}
		}
		else if (bladeSet.at(3) == 1) { //If blade set three is in moving pos
			if (bladeSingleMoving.at(0) == 4 || bladeSingleMoving.at(0) == 3) {
				bladeSingleMoving.at(0) = 3;
				if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() + 1][enemiesRoomSixteen.at(0).GetLocationCols()] == 48) {

					loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(0).SetLocationRows(enemiesRoomSixteen.at(0).GetLocationRows() + 1);
					loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() + 1][enemiesRoomSixteen.at(0).GetLocationCols()] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(3) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() + 1][enemiesRoomSixteen.at(0).GetLocationCols()] == 43 || loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() + 1][enemiesRoomSixteen.at(0).GetLocationCols()] == 49) {
					bladeSet.at(3) = 2;
				}
			}
			if (bladeSingleMoving.at(2) == 4 || bladeSingleMoving.at(2) == 3) {
				bladeSingleMoving.at(2) = 3;
				if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() - 1][enemiesRoomSixteen.at(2).GetLocationCols()] == 48) {

					loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 48;
					enemiesRoomSixteen.at(2).SetLocationRows(enemiesRoomSixteen.at(2).GetLocationRows() - 1);
					loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 43;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() - 1][enemiesRoomSixteen.at(2).GetLocationCols()] == 80) {
					playerHealth--;
					RunSound("linkHurt");
					bladeSet.at(3) = 2;
				}
				else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() - 1][enemiesRoomSixteen.at(2).GetLocationCols()] == 43 || loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() - 1][enemiesRoomSixteen.at(2).GetLocationCols()] == 49) {
					bladeSet.at(3) = 2;
				}
			}
		}
		else if (bladeSet.at(3) == 2) { //Get to OG spots

			if (bladeReturnCounter.at(3) < 4) {
				bladeReturnCounter.at(3)++;
			}
			else {
				if (bladeSingleMoving.at(0) == 4 || bladeSingleMoving.at(0) == 3) {
					if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() - 1][enemiesRoomSixteen.at(0).GetLocationCols()] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows() - 1][enemiesRoomSixteen.at(0).GetLocationCols()] != 49) {

						loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(0).SetLocationRows(enemiesRoomSixteen.at(0).GetLocationRows() - 1);
						loadedRoom[enemiesRoomSixteen.at(0).GetLocationRows()][enemiesRoomSixteen.at(0).GetLocationCols()] = 43;
					}
				}

				if (bladeSingleMoving.at(2) == 4 || bladeSingleMoving.at(2) == 3) {
					if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() + 1][enemiesRoomSixteen.at(2).GetLocationCols()] == 80) {
						playerHealth--;
						RunSound("linkHurt");
					}
					else if (loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() + 1][enemiesRoomSixteen.at(2).GetLocationCols()] != 49) {
						loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows()][enemiesRoomSixteen.at(2).GetLocationCols()] = 48;
						enemiesRoomSixteen.at(2).SetLocationRows(enemiesRoomSixteen.at(2).GetLocationRows() + 1);
						loadedRoom[enemiesRoomSixteen.at(2).GetLocationRows() ][enemiesRoomSixteen.at(2).GetLocationCols()] = 43;
					}

					if (enemiesRoomSixteen.at(0).GetLocationRows() == 1 && enemiesRoomSixteen.at(2).GetLocationRows() == 7) { //Both traps are in OG pos
						bladeSet.at(3) = 0;
						bladeSingleMoving.at(0) = 4;
						bladeSingleMoving.at(2) = 4;
					}
				}

				bladeReturnCounter.at(3) = 0;
			}
		}

		enemiesLoadedRoom = enemiesRoomSixteen;
	}
}

void SwordHandler(int &swordDir) {
	if (lastDirection == 0) { //Looking up
		swordLocRows = charYLoc - 1;
		swordLocCols = charXLoc;
		loadedRoom[swordLocRows][swordLocCols] = 179;
		swordDir = 0;
	}
	else if (lastDirection == 1) { //Looking right
		swordLocRows = charYLoc;
		swordLocCols = charXLoc + 1;
		loadedRoom[swordLocRows][swordLocCols] = 196;
		swordDir = 1;
	}
	else if (lastDirection == 2) { //Looking down
		swordLocRows = charYLoc + 1;
		swordLocCols = charXLoc;
		loadedRoom[swordLocRows][swordLocCols] = 179;
		swordDir = 2;
	}
	else if (lastDirection == 3) { //Looking left
		swordLocRows = charYLoc;
		swordLocCols = charXLoc - 1;
		loadedRoom[swordLocRows][swordLocCols] = 196;
		swordDir = 3;
	}
}

Enemy EnemyMove(Enemy theEnemy) {
	int rows = theEnemy.GetLocationRows();
	int cols = theEnemy.GetLocationCols();
	int oldRows = rows;
	int oldCols = cols;
	int enemyChar = 69;

	int lastDirection = theEnemy.GetLastDirection();

	if (theEnemy.GetEnemyType() == "skeleton") {
		SmartAI(theEnemy, rows, cols);
		enemyChar = 83;
	}
	else if (theEnemy.GetEnemyType() == "keese") {
		KeeseAI(theEnemy, rows, cols);
		enemyChar = 70;
	}
	else if (theEnemy.GetEnemyType() == "wallmaster") {

		if (rows == 0) {
			rows = 1;
			cols = cols;
		}
		else if (rows == 8) {
			rows = 7;
			cols = cols;
		}
		else if (cols == 0) {
			rows = rows;
			cols = 1;
		}
		else if (cols == 13) {
			rows = rows;
			cols = 12;
		}
		else {
			SmartAI(theEnemy, rows, cols);
		}

		enemyChar = 87;
	}
	else if (theEnemy.GetEnemyType() == "bladetrap") {
		//BladeTrapAI();
		//enemyChar = 43;
		return theEnemy;
	}
	else if (theEnemy.GetEnemyType() == "gel") {
		SmartAI(theEnemy, rows, cols);
		enemyChar = 69;
	}
	else if (theEnemy.GetEnemyType() == "goriya") {
		GoriyaAI(theEnemy, rows, cols);
		enemyChar = 71;
	}
	else if (theEnemy.GetEnemyType() == "dragon") {
		DragonAI(theEnemy, rows, cols);
		enemyChar = -1;
	}


	if (oldRows != rows) {
		if (rows > oldRows) { //Went down
			theEnemy.SetCurrentDirection(2);
			theEnemy.SetLastDirection(2);
		}
		else if (rows < oldRows) { //Went up
			theEnemy.SetCurrentDirection(0);
			theEnemy.SetLastDirection(0);
		}
	}
	else if (oldCols != cols) {
		if (cols > oldCols) { //Went right
			theEnemy.SetCurrentDirection(1);
			theEnemy.SetLastDirection(1);
		}
		else if (cols < oldCols) { //Went left
			theEnemy.SetCurrentDirection(3);
			theEnemy.SetLastDirection(3);
		}
	}

	if (loadedRoom[rows][cols] == 80 && enemyChar != -1) { //If trying to collide with player NOT dragon
		RunSound("linkHurt");
		if (theEnemy.GetEnemyType() != "wallmaster") {
			playerHealth--;
		}
		else {
			WallMasterAttack(theEnemy, rows, cols);
		}
	}
	else if ((loadedRoom[rows][cols] == 80 || loadedRoom[rows + 1][cols] == 80 || loadedRoom[rows + 1][cols + 1] == 80) && enemyChar == -1) { //If trying to collide with player IS dragon
		playerHealth--;
		RunSound("linkHurt");
	}
	else if (loadedRoom[rows][cols] != 48) { //If trying to enter a NON empty space
		theEnemy.SetCollideWithWall(true);

		if (enemyChar == -1) {
			if (theEnemy.GetCurrentDirection() == 3) {
				theEnemy.SetCurrentDirection(1);
			}
			else {
				theEnemy.SetCurrentDirection(3);
			}
		}
	}
	else {
		if (enemyChar != -1) { //Means it is not the dragon
			if (theEnemy.GetEnemyType() != "wallmaster") { //Not wall master
				loadedRoom[oldRows][oldCols] = 48; //Sets old enemy location to blank
				loadedRoom[rows][cols] = enemyChar; //Sets new enemy location to enemy icon
				theEnemy.SetLocation(rows, cols);
			}
			else { //Is wallmaster
				//Sets old enemy location to either wall or empty space depending on location
				if (oldRows == 0 || oldRows == 8) {
					loadedRoom[oldRows][oldCols] = 49;
				}
				else if (oldCols == 0 || oldCols == 13) {
					loadedRoom[oldRows][oldCols] = 50;
				}
				else {
					loadedRoom[oldRows][oldCols] = 48;
				}

				loadedRoom[rows][cols] = enemyChar; //Sets new enemy location to enemy icon
				theEnemy.SetLocation(rows, cols);
			}
		}
		else { //It is the dragon
			loadedRoom[oldRows][oldCols] = 48; //Sets old dragon head to blank
			loadedRoom[oldRows + 1][oldCols] = 48; //Sets old dragon body one to blank
			loadedRoom[oldRows + 1][oldCols + 1] = 48; //Sets old dragon body two to blank

			loadedRoom[rows][cols] = 68; //Sets new dragon head
			loadedRoom[rows + 1][cols] = 100; //Sets new dragon body one
			loadedRoom[rows + 1][cols + 1] = 100; //Sets new dragon body two
			theEnemy.SetLocation(rows, cols);
		}
	}

	memcpy(loadedRoom, loadedRoom, sizeof(loadedRoom));
	return theEnemy;
}

void TryEnemyMove(Enemy &theEnemy) {
	int counter = theEnemy.GetMovementTimer();
	int mSpeed = theEnemy.GetMovementSpeed();
	bool stunned = theEnemy.GetStunned();
	int stCounter = theEnemy.GetStunnedCounter();

	if (stunned == true && theEnemy.GetEnemyType() != "dragon") {
		if (stCounter < 20) {
			theEnemy.IncreaseStunnedCounter();
		}
		else {
			theEnemy.SetStunned(false);
			theEnemy.ResetStunnedCounter();
		}
		return;
	}

	if (counter < mSpeed) {
		int newValue = theEnemy.GetMovementTimer() + 1;
		theEnemy.SetMovementTimer(newValue);
	}
	else if (counter >= mSpeed) {
		theEnemy.ResetMovementTimer();
		theEnemy = EnemyMove(theEnemy);
	}

} 

bool IsEnemy(int rows, int cols) {
	//Determines if the space that the player is attacking contains an enemy.
	if (loadedRoom[rows][cols] == 87 || loadedRoom[rows][cols] == 70 || loadedRoom[rows][cols] == 71 || loadedRoom[rows][cols] == 69 || loadedRoom[rows][cols] == 83 || loadedRoom[rows][cols] == 68) {
		return true;
	}
	else {
		return false;
	}
}

bool IsWalkable(int rows, int cols) {
	if (loadedRoom[charYLoc][charXLoc] == 48 || loadedRoom[charYLoc][charXLoc] == 51 || loadedRoom[charYLoc][charXLoc] == 52 || loadedRoom[charYLoc][charXLoc] == 75 || loadedRoom[charYLoc][charXLoc] == 67 || loadedRoom[charYLoc][charXLoc] == 77 || loadedRoom[charYLoc][charXLoc] == 179 || loadedRoom[charYLoc][charXLoc] == 196 || loadedRoom[charYLoc][charXLoc] == 235 || loadedRoom[charYLoc][charXLoc] == 72 || loadedRoom[charYLoc][charXLoc] == 47 || loadedRoom[charYLoc][charXLoc] == 98 || loadedRoom[charYLoc][charXLoc] == 82 || loadedRoom[charYLoc][charXLoc] == 'T') {
		return true;
	}
	else {
		return false;
	}
}

void UpdatePlayerScore(string enemyType) {

	if (enemyType == "gel") {
		playerScore = playerScore + 1;
	}
	else if (enemyType == "keese") {
		playerScore = playerScore + 5;
	}
	else if (enemyType == "skeleton") {
		playerScore = playerScore + 10;
	}
	else if (enemyType == "goriya") {
		playerScore = playerScore + 20;
	}
	else if (enemyType == "wallmaster") {
		playerScore = playerScore + 40;
	}
	else if (enemyType == "dragon") {
		playerScore = playerScore + 100;
	}

}

void GameInstaller() {
	//To be ran if the essential files are not found in the correct location. 

	//Test to be sure that files were placed on the DESKTOP
	ifstream testFile;
	testFile.open("C:\\Users\\" + windowsUsername + "\\Desktop\\EssentialFiles\\Maze Runner\\other\\titleScreen.txt");

	if (!testFile) {
		cout << endl;
		cout << "Critcal Error: Unable to install essential files" << endl;
		cout << "Please be sure that you placed the 'EssentialFiles' folder on your DESKTOP." << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		testFile.close();
		exit(1);
	}

	testFile.close();

	fs::remove_all("C:\\Users\\" + windowsUsername + "\\Documents\\Maze Runner"); //Deletes old files

	fs::copy("C:\\Users\\" + windowsUsername + "\\Desktop\\EssentialFiles\\Maze Runner", "C:\\Users\\" + windowsUsername + "\\Documents\\Maze Runner", fs::copy_options::recursive); //Copies in new files

	ifstream inFile;
	inFile.open("C:\\Users\\" + windowsUsername + "\\Documents\\Maze Runner\\" + version); //Trys to open version file in new location
	if (!inFile) {
		//If it fails (which could happen because the files that were trying to be installed were outdated)
		cout << endl;
		cout << "Critcal Error: Unable install essential files" << endl;
		cout << "Ensure that the essential files that you are installing" << endl;
		cout << "match the version of Maze Runner that you are running." << endl;
		cout << "You are running Maze Runner Version " << version << endl;
		cout << "Shutting down in 15 seconds." << endl;
		Sleep(15000);
		inFile.close();
		exit(1);
	}
	else {
		//If it does not fail.
		cout << endl;
		cout << "The game has been installed correctly. You may delete the essential files from your desktop." << endl;
		cout << "First time setup complete. Enjoy Maze Runner!" << endl;
		cout << "Loading game..." << endl;
		Sleep(4000);
		inFile.close();
	}
}

bool EssentialFilesCheck() {

	if (CheckDir(mainDir) == true) {
		return true;
	}
	else {
		return false;
	}

	ifstream inFile;
}

bool VersionCheck() {
	ifstream inFile;
	inFile.open(mainDir + version);
	if (!inFile) {
		inFile.close();
		return false;
	}
	else {
		inFile.close();
		return true;
	}
}

void RunSound(string soundType) {

	string sound;

	if (soundType == "swordSlash") {
		sound = mainDir + "sounds\\Sword_Slash.wav";
	}
	else if (soundType == "linkHurt") {
		sound = mainDir + "sounds\\Link_Hurt.wav";
	}
	else if (soundType == "arrow/boom") {
		sound = mainDir + "sounds\\Arrow_Boomerang.wav";
	}
	else if (soundType == "enemyDie") {
		sound = mainDir + "sounds\\Enemy_Die.wav";
	}
	else if (soundType == "getHeart") {
		sound = mainDir + "sounds\\Get_Heart.wav";
	}
	else if (soundType == "getItem") {
		sound = mainDir + "sounds\\Get_Item.wav";
	}
	else if (soundType == "getRupee") {
		sound = mainDir + "sounds\\Get_Rupee.wav";
	}
	else if (soundType == "keyAppear") {
		sound = mainDir + "sounds\\Key_Appear.wav";
	}
	else if (soundType == "bombBlow") {
		sound = mainDir + "sounds\\Bomb_Blow.wav";
	}
	else if (soundType == "bombDrop") {
		sound = mainDir + "sounds\\Bomb_Drop.wav";
	}
	else if (soundType == "doorUnlock") {
		sound = mainDir + "sounds\\Door_Unlock.wav";
	}
	else if (soundType == "bossHit") {
		sound = mainDir + "sounds\\Boss_Hit.wav";
	}
	else if (soundType == "bossScreamOne") {
		sound = mainDir + "sounds\\Boss_Scream1.wav";
	}
	else if (soundType == "bossScreamTwo") {
		sound = mainDir + "sounds\\Boss_Scream2.wav";
	}
	else if (soundType == "bossDie") {
		sound = mainDir + "sounds\\Boss_Scream3.wav";
	}
	else if (soundType == "secretTheme") {
		sound = mainDir + "sounds\\Secret_Theme.wav";
	}
	else if (soundType == "stairs") {
		sound = mainDir + "sounds\\Stairs.wav";
	}
	else if (soundType == "getBow") {
		sound = mainDir + "sounds\\Get_Bow.wav";
	}
	else if (soundType == "linkDie") {
		sound = mainDir + "sounds\\Link_Die.wav";
	}
	else if (soundType == "beatGame") {
		sound = mainDir + "sounds\\Beat_Game.wav";
	}
	else {
		cout << "Error: Could not find sound " << soundType << endl;
		sound = mainDir + "sounds\\Enemy_Die.wav";
	}

	PlaySoundA(sound.c_str(), NULL, SND_ALIAS | SND_ASYNC);

}

void ChangeScreenSize(int newSize) { //Newsize is an int between 0 and 2

	if (newSize == 0) { //Change to small size screen
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = 16;                  // Height
												//cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
	else if (newSize == 1) { //Change to medium size screen

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = 32;                  // Height
												//cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}
	else if (newSize == 2) { //Change to large size screen

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = 48;                  // Height
												//cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

}

void VerifyBombWalls() {
	if (currentRoom == 6) {
		if (openDoors.doorFour == false) {
			loadedRoom[0][6] = 45;
			loadedRoom[0][7] = 45;
		}
		else {
			loadedRoom[0][6] = 51;
			loadedRoom[0][7] = 51;
		}
	}
	else if (currentRoom == 7) {
		if (openDoors.doorFive == false) {
			loadedRoom[0][6] = 45;
			loadedRoom[0][7] = 45;
		}
		else {
			loadedRoom[0][6] = 51;
			loadedRoom[0][7] = 51;
		}
	}
	else if (currentRoom == 10) {
		if (openDoors.doorFour == false) {
			loadedRoom[8][6] = 45;
			loadedRoom[8][7] = 45;
		}
		else {
			loadedRoom[8][6] = 51;
			loadedRoom[8][7] = 51;
		}
	}
	else if (currentRoom == 11) {
		if (openDoors.doorFive == false) {
			loadedRoom[8][6] = 45;
			loadedRoom[8][7] = 45;
		}
		else {
			loadedRoom[8][6] = 51;
			loadedRoom[8][7] = 51;
		}
	}
}

int main() {


	//PRE BOOT CODE

	//Get windows username
	TCHAR name[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)name, &size);
	windowsUsername = CW2A(name);

	//Sets main directory
	mainDir = "C:\\Users\\" + windowsUsername + "\\Documents\\Maze Runner\\";

	//Verify Files

	if (EssentialFilesCheck() != true) {
		cout << "It appears that you have never play before. Running the first time setup now." << endl;
		cout << "For this to work correctly, you must assure that the essential files are on your DESKTOP." << endl;
		cout << "We will handle it from there!" << endl;
		Sleep(1000);
		cout << "Running the install process for v" << version << "." << endl;
		cout << endl;
		Sleep(1000);
		GameInstaller();
	}
	else if (VersionCheck() != true) {
		cout << "Your installed version does not match the version you are trying to run." << endl;
		cout << "For this to work correctly, you must assure that the essential files are on your DESKTOP." << endl;
		cout << "We will handle it from there!" << endl;
		Sleep(1000);
		cout << "Running the install process for v" << version << "." << endl;
		Sleep(1000);
		GameInstaller();
	}

	//Check title version is proper (this is just there to help me remember)
	if (version != "1.0") {
		cout << "Change the title version!" << endl;
		cout << "Game will start in 15 seconds" << endl;
		Sleep(15000);
	}

	//Resize console window
	//GetWindowRect(console, &r);
	//MoveWindow(console, r.left, r.top, 480, 295, FALSE);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT windowSize = { 0, 0, 54, 15 };

	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	//Set console title
	SetConsoleTitle(TEXT("Maze Runner - v1.0 - Created By Alex Gray - 2018"));

	//Prevent console resizing
	SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	//Remove console scroll bar
	HANDLE console2 = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console2, &csbi);
	COORD scrollbar = {
		csbi.srWindow.Right - csbi.srWindow.Left + 1,
		csbi.srWindow.Bottom - csbi.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(console2, scrollbar);

	//Change to med screen
	currentScreenSize = 1;
	ChangeScreenSize(currentScreenSize);

	//END PRE BOOT CODE


	bool booted = true;

	saveOneExist = CheckDir(mainDir + "savedata\\1-save");
	saveTwoExist = CheckDir(mainDir + "savedata\\2-save");
	saveThreeExist = CheckDir(mainDir + "savedata\\3-save");

	while (booted == true) {

		//Title Screen Code
		currentLoadedSave = 0;
		system("CLS");
		LoadMenus();
		ShowConsoleCursor();
		bool inTitle = true;
		int cursor = 0;
		PrintTitleScreen(titleScreen, 0);

		while (inTitle == true && isDevMode == false) {
			isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

			if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
				if (cursor == 0) {
					RunSound("bombDrop");
					Sleep(300);
					bool createNewGame = true;
					bool inOverwriteScreen = false;
					ResetGame(true); //Load in Master Room Files.
					system("CLS");
					cursor = 0;
					PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
					while (createNewGame == true) {

						isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

						if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 0) {
								cursor--;
								PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 3) {
								cursor++;
								PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
							cursor = 0;
							PrintTitleScreen(titleScreen, 0);
							createNewGame = false;
						}

						if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
							if (cursor == 0) {								
								if (saveOneExist == false) { //Create Save File 1 if it does not exist
									RunSound("bombDrop");
									currentLoadedSave = 1;
									SaveGame(currentLoadedSave);
									saveOneExist = true;
									createNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									RunSound("bombDrop");
									Sleep(300);
									inOverwriteScreen = true;
									cursor = 1;
									PrintOverwriteScreen(overwriteScreen, cursor);
									while (inOverwriteScreen == true) {

										isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

										if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 0) {
												cursor--;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 1) {
												cursor++;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
											RunSound("bombDrop");
											Sleep(300);
											system("CLS");
											cursor = 0;
											PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											inOverwriteScreen = false;
										}

										if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
											if (cursor == 0) { //The user selected YES
												RunSound("bombDrop");
												currentLoadedSave = 1;
												SaveGame(currentLoadedSave);
												saveOneExist = true;
												createNewGame = false;
												inTitle = false;
												Sleep(300);
											}
											else { //NO
												RunSound("bombDrop");
												Sleep(300);
												system("CLS");
												cursor = 0;
												PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											}

											inOverwriteScreen = false;
										}

										if (keyPress == true) {
											keyPress = false;
										}

									}
								}
							}
							else if (cursor == 1) {
								if (saveTwoExist == false) { //Create Save File 2 if it does not exist
									RunSound("bombDrop");
									currentLoadedSave = 2;
									SaveGame(currentLoadedSave);
									saveTwoExist = true;
									createNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									RunSound("bombDrop");
									Sleep(300);
									inOverwriteScreen = true;
									cursor = 1;
									PrintOverwriteScreen(overwriteScreen, cursor);
									while (inOverwriteScreen == true) {

										isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

										if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 0) {
												cursor--;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 1) {
												cursor++;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
											RunSound("bombDrop");
											Sleep(300);
											system("CLS");
											cursor = 0;
											PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											inOverwriteScreen = false;
										}

										if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
											if (cursor == 0) { //The user selected YES
												RunSound("bombDrop");
												currentLoadedSave = 2;
												SaveGame(currentLoadedSave);
												saveTwoExist = true;
												createNewGame = false;
												inTitle = false;
												Sleep(300);
											}
											else { //NO
												RunSound("bombDrop");
												Sleep(300);
												system("CLS");
												cursor = 0;
												PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											}

											inOverwriteScreen = false;
										}

										if (keyPress == true) {
											keyPress = false;
										}

									}
								}
							}
							else if (cursor == 2) {
								if (saveThreeExist == false) { //Create Save File 3 if it does not exist
									RunSound("bombDrop");
									currentLoadedSave = 3;
									SaveGame(currentLoadedSave);
									saveThreeExist = true;
									createNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									RunSound("bombDrop");
									Sleep(300);
									inOverwriteScreen = true;
									cursor = 1;
									PrintOverwriteScreen(overwriteScreen, cursor);
									while (inOverwriteScreen == true) {

										isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

										if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 0) {
												cursor--;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
											keyPress = true;
											if (cursor != 1) {
												cursor++;
												PrintOverwriteScreen(overwriteScreen, cursor);
												RunSound("getRupee");
												Sleep(300);
											}
										}

										if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
											RunSound("bombDrop");
											Sleep(300);
											system("CLS");
											cursor = 0;
											PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											inOverwriteScreen = false;
										}

										if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
											if (cursor == 0) { //The user selected YES
												RunSound("bombDrop");
												currentLoadedSave = 3;
												SaveGame(currentLoadedSave);
												saveThreeExist = true;
												createNewGame = false;
												inTitle = false;
												Sleep(300);
											}
											else { //NO
												RunSound("bombDrop");
												Sleep(300);
												system("CLS");
												cursor = 0;
												PrintSaveLoadScreen(SaveAndLoadScreen, cursor, true);
											}

											inOverwriteScreen = false;
										}

										if (keyPress == true) {
											keyPress = false;
										}

									}
								}
							}
							else if (cursor == 3) {
								RunSound("bombDrop");
								Sleep(300);
								system("CLS");
								cursor = 0;
								PrintTitleScreen(titleScreen, 0);
								createNewGame = false;
							}
						}

						if (keyPress == true) {
							keyPress = false;
						}

					}
				}
				else if (cursor == 1) {
					//Load A Save.
					RunSound("bombDrop");
					Sleep(300);
					ResetGame(true); //Load in Master Room Files.
					bool loadNewGame = true;
					system("CLS");
					cursor = 0;
					PrintSaveLoadScreen(SaveAndLoadScreen, cursor, false);
					while (loadNewGame == true) {

						isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

						if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 0) {
								cursor--;
								PrintSaveLoadScreen(SaveAndLoadScreen, cursor, false);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 3) {
								cursor++;
								PrintSaveLoadScreen(SaveAndLoadScreen, cursor, false);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
							cursor = 0;
							PrintTitleScreen(titleScreen, 0);
							loadNewGame = false;
						}

						if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
							if (cursor == 0) {
								//Load Save File 1
								if (saveOneExist == true) {
									RunSound("bombDrop");
									currentLoadedSave = 1;
									LoadGame(true, currentLoadedSave);
									LoadGameData(currentLoadedSave);
									loadNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									//Write code to display an error screen
									RunSound("linkHurt");
									Sleep(300);
								}
							}
							else if (cursor == 1) {
								//Load Save File 2
								if (saveTwoExist == true) {
									RunSound("bombDrop");
									currentLoadedSave = 2;
									LoadGame(true, currentLoadedSave);
									LoadGameData(currentLoadedSave);
									loadNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									//Write code to display an error screen
									RunSound("linkHurt");
									Sleep(300);
								}
							}
							else if (cursor == 2) {
								//Load Save File 3
								if (saveThreeExist == true) {
									RunSound("bombDrop");
									currentLoadedSave = 3;
									LoadGame(true, currentLoadedSave);
									LoadGameData(currentLoadedSave);
									loadNewGame = false;
									inTitle = false;
									Sleep(300);
								}
								else {
									//Write code to display an error screen
									RunSound("linkHurt");
									Sleep(300);
								}
							}
							else if (cursor == 3) {
								RunSound("bombDrop");
								Sleep(300);
								system("CLS");
								cursor = 0;
								PrintTitleScreen(titleScreen, 0);
								loadNewGame = false;
							}
						}

						if (keyPress == true) {
							keyPress = false;
						}
					}
				}
				else if (cursor == 2) {
					RunSound("bombDrop");
					Sleep(300);
					cursor = 0;
					PrintOptionsScreen(optionsScreen, cursor);
					bool inOptions = true;
					while (inOptions == true) {

						isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

						if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 0) {
								cursor--;
								PrintOptionsScreen(optionsScreen, cursor);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
							keyPress = true;
							if (cursor != 3) {
								cursor++;
								PrintOptionsScreen(optionsScreen, cursor);
								RunSound("getRupee");
								Sleep(300);
							}
						}

						if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
							cursor = 0;
							PrintTitleScreen(titleScreen, 0);
							inOptions = false;
						}

						if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
							if (cursor == 0) { //Display game controls.
								RunSound("bombDrop");
								Sleep(300);
								cursor = 0;
								PrintControlsScreen(controlsScreen, cursor);
								bool inControls = true;
								while (inControls == true) {

									isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

									if ((GetKeyState(VK_ESCAPE) & 0x8000 || GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
										RunSound("bombDrop");
										Sleep(300);
										system("CLS");
										cursor = 0;
										PrintOptionsScreen(optionsScreen, cursor);
										inControls = false;
									}

								}
							}
							else if (cursor == 1) { //Adjust Screen size.
								RunSound("bombDrop");
								Sleep(300);
								if (currentScreenSize == 2) {
									currentScreenSize = 0;
								}
								else {
									currentScreenSize++;
								}

								ChangeScreenSize(currentScreenSize);
								PrintOptionsScreen(optionsScreen, cursor);
							}
							else if (cursor == 2) { //Ask to delete all save data
								RunSound("bombDrop");
								Sleep(300);
								cursor = 1;
								PrintOverwriteScreen(overwriteScreen, cursor, false);
								bool askUser = true;
								while (askUser == true) {

									isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

									if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
										keyPress = true;
										if (cursor != 0) {
											cursor--;
											PrintOverwriteScreen(overwriteScreen, cursor, false);
											RunSound("getRupee");
											Sleep(300);
										}
									}

									if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
										keyPress = true;
										if (cursor != 1) {
											cursor++;
											PrintOverwriteScreen(overwriteScreen, cursor, false);
											RunSound("getRupee");
											Sleep(300);
										}
									}

									if (GetKeyState(VK_ESCAPE) & 0x8000 && isConsoleWindowFocused == true) {
										RunSound("bombDrop");
										Sleep(300);
										system("CLS");
										cursor = 0;
										PrintOptionsScreen(optionsScreen, cursor);
										askUser = false;
									}

									if ((GetKeyState(VK_RETURN) & 0x8000 || GetKeyState(VK_SPACE) & 0x8000) && isConsoleWindowFocused == true) {
										if (cursor == 0) { //The user selected YES
											RunSound("bombDrop");
											system("CLS");
											Sleep(250);
											DeleteSave(1);
											DeleteSave(2);
											DeleteSave(3);
											Sleep(250);
											cursor = 0;
											PrintOptionsScreen(optionsScreen, cursor);
										}
										else { //NO
											RunSound("bombDrop");
											Sleep(300);
											system("CLS");
											cursor = 0;
											PrintOptionsScreen(optionsScreen, cursor);
										}

										askUser = false;
									}

									if (keyPress == true) {
										keyPress = false;
									}

								}
							}
							else if(cursor == 3) { //Return to tile screen
								RunSound("bombDrop");
								Sleep(300);
								system("CLS");
								cursor = 0;
								PrintTitleScreen(titleScreen, 0);
								inOptions = false;
							}

						}

						if (keyPress == true) {
							keyPress = false;
						}

					}


				}
				else if (cursor == 3) {
					RunSound("bombDrop");
					Sleep(300);
					exit(1);
				}
			}

			if (((GetAsyncKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
				keyPress = true;
				if (cursor == 2) {
					cursor = 0;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
				else if (cursor == 3) {
					cursor = 1;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
			}

			if (((GetAsyncKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
				keyPress = true;
				if (cursor == 0) {
					cursor = 2;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
				else if (cursor == 1) {
					cursor = 3;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
			}

			if (((GetAsyncKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
				keyPress = true;
				if (cursor == 1) {
					cursor = 0;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
				else if (cursor == 3) {
					cursor = 2;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
			}

			if (((GetAsyncKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
				keyPress = true;
				if (cursor == 0) {
					cursor = 1;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
				else if (cursor == 2) {
					cursor = 3;
					PrintTitleScreen(titleScreen, cursor);
					RunSound("getRupee");
					Sleep(300);
				}
			}

			if (keyPress == true) {
				keyPress = false;
			}

		}
		//END title screen code
		memcpy(dungeonMap, masterDungeonMap, sizeof(dungeonMap));
		switch (currentRoom) {
		case 1:
			roomOne[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomOne, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomOne;
			dungeonMap[7][4] = 43;
			break;
		case 2:
			roomTwo[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomTwo, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomTwo;
			dungeonMap[7][5] = 43;
			break;
		case 3:
			roomThree[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomThree, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomThree;
			dungeonMap[7][6] = 43;
			break;
		case 4:
			roomFour[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomFour, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomFour;
			dungeonMap[6][5] = 43;
			break;
		case 5:
			roomFive[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomFive, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomFive;
			dungeonMap[5][4] = 43;
			break;
		case 6:
			roomSix[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomSix, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomSix;
			dungeonMap[5][5] = 43;
			break;
		case 7:
			roomSeven[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomSeven, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomSeven;
			dungeonMap[5][6] = 43;
			break;
		case 8:
			roomEight[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomEight, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomEight;
			dungeonMap[4][3] = 43;
			break;
		case 9:
			roomNine[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomNine, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomNine;
			dungeonMap[4][4] = 43;
			break;
		case 10:
			roomTen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomTen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomTen;
			dungeonMap[4][5] = 43;
			break;
		case 11:
			roomEleven[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomEleven, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomEleven;
			dungeonMap[4][6] = 43;
			break;
		case 12:
			roomTwelve[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomTwelve, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomTwelve;
			dungeonMap[4][7] = 43;
			break;
		case 13:
			roomThirteen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomThirteen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomThirteen;
			dungeonMap[3][5] = 43;
			break;
		case 14:
			roomFourteen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomFourteen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomFourteen;
			dungeonMap[3][7] = 43;
			break;
		case 15:
			roomFifteen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomFifteen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomFifteen;
			dungeonMap[3][8] = 43;
			break;
		case 16:
			roomSixteen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomSixteen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomSixteen;
			dungeonMap[2][4] = 43;
			break;
		case 17:
			roomSeventeen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomSeventeen, sizeof(loadedRoom));
			enemiesLoadedRoom = enemiesRoomSeventeen;
			dungeonMap[2][5] = 43;
			break;
		case 18:
			roomEighteen[charYLoc][charXLoc] = 'P';
			memcpy(loadedRoom, roomEighteen, sizeof(loadedRoom));
			dungeonMap[2][4] = 43;
			break;
		default:
			cout << "Critcal Error: Could not load room. (Before Game Launch)" << endl;
			Sleep(5000);
			exit(1);
			break;
		}

		system("CLS");

		if (isDevMode == true) { //Just auto start a file if dev mode is enabled
			ResetGame(true); //Load in Master Room Files.
			currentLoadedSave = 1;
			SaveGame(currentLoadedSave);
			saveOneExist = true;
		}

		int frames = 0;
		double overtime = 0;
		double nextUpdate = 0;
		double fps = 0;

		int movementCounter = 0;
		int gameCounter = 0;

		int wasPaused = 100;

		while (gameRunning == true) {

			isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

			PrintScreen(loadedRoom, dungeonMap);

			if (playerHealth <= 0 && isInvincible == false) { //If the player runs out of health
				//Displays when the player dies.
				string revive;
				bool comeBack = false;
				system("CLS");
				RunSound("linkDie");
				cout << "Game Over! Score: " << playerScore << endl;
				cout << "Thank You for Playing!" << endl;
				cout << "Press 'Esc' to Return to The Title Screen" << endl;
				cout << endl;
				cout << "Maze Runner v" << version << " - Developed By Alex Gray" << endl;

				while (comeBack == false) {
					if (GetKeyState(VK_ESCAPE) & 0x8000) {
						gameRunning = false;
						comeBack = true;
						DeleteSave(currentLoadedSave);
						RunSound("bombDrop");
						Sleep(300);
					}

					if (GetKeyState(VK_UP) & 0x8000) { //First Up
						while (comeBack == false) {
							if (GetKeyState(VK_ESCAPE) & 0x8000) {
								gameRunning = false;
								comeBack = true;
								DeleteSave(currentLoadedSave);
								RunSound("bombDrop");
								Sleep(300);
							}

							if (GetKeyState(VK_UP) & 0x8000) { //Second Up
								while (comeBack == false) {
									if (GetKeyState(VK_ESCAPE) & 0x8000) {
										gameRunning = false;
										comeBack = true;
										DeleteSave(currentLoadedSave);
										RunSound("bombDrop");
										Sleep(300);
									}

									if (GetKeyState(VK_DOWN) & 0x8000) { //First down
										while (comeBack == false) {
											if (GetKeyState(VK_ESCAPE) & 0x8000) {
												gameRunning = false;
												comeBack = true;
												DeleteSave(currentLoadedSave);
												RunSound("bombDrop");
												Sleep(300);
											}

											if (GetKeyState(VK_DOWN) & 0x8000) { //Second down
												while (comeBack == false) {
													if (GetKeyState(VK_ESCAPE) & 0x8000) {
														gameRunning = false;
														comeBack = true;
														DeleteSave(currentLoadedSave);
														RunSound("bombDrop");
														Sleep(300);
													}

													if (GetKeyState(VK_LEFT) & 0x8000) { //First Left
														while (comeBack == false) {
															if (GetKeyState(VK_ESCAPE) & 0x8000) {
																gameRunning = false;
																comeBack = true;
																DeleteSave(currentLoadedSave);
																RunSound("bombDrop");
																Sleep(300);
															}

															if (GetKeyState(VK_RIGHT) & 0x8000) { //First Right
																while (comeBack == false) {
																	if (GetKeyState(VK_ESCAPE) & 0x8000) {
																		gameRunning = false;
																		comeBack = true;
																		DeleteSave(currentLoadedSave);
																		RunSound("bombDrop");
																		Sleep(300);
																	}

																	if (GetKeyState(VK_LEFT) & 0x8000) { //Second Left
																		while (comeBack == false) {
																			if (GetKeyState(VK_ESCAPE) & 0x8000) {
																				gameRunning = false;
																				comeBack = true;
																				DeleteSave(currentLoadedSave);
																				RunSound("bombDrop");
																				Sleep(300);
																			}

																			if (GetKeyState(VK_RIGHT) & 0x8000) { //Second Right
																				while (comeBack == false) {
																					if (GetKeyState(VK_ESCAPE) & 0x8000) {
																						gameRunning = false;
																						comeBack = true;
																						DeleteSave(currentLoadedSave);
																						RunSound("bombDrop");
																						Sleep(300);
																					}

																					if (GetKeyState('B') & 0x8000) { //B
																						while (comeBack == false) {
																							if (GetKeyState(VK_ESCAPE) & 0x8000) {
																								gameRunning = false;
																								comeBack = true;
																								DeleteSave(currentLoadedSave);
																								RunSound("bombDrop");
																								Sleep(300);
																							}

																							if (GetKeyState('A') & 0x8000) { //A
																								cout << endl;
																								RunSound("keyAppear");
																								cout << "Press Enter To Continue." << endl;
																								while (comeBack == false) {
																									if (GetKeyState(VK_ESCAPE) & 0x8000) {
																										gameRunning = false;
																										comeBack = true;
																										DeleteSave(currentLoadedSave);
																										RunSound("bombDrop");
																										Sleep(300);
																									}

																									if (GetKeyState(VK_RETURN) & 0x8000) { //"Select"
																										enabledCheats = true;
																										playerHealth = 10;
																										system("CLS");
																										comeBack = true;
																										RunSound("secretTheme");
																										Sleep(1400);
																									}
																								}
																							}
																						}
																					}
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

			if (enabledCheats == true) {
				if (playerKeys < 0) {
					playerKeys = 0;
				}

				if (playerBombs < 0) {
					playerBombs = 0;
				}

				if (playerCoins < 0) {
					playerCoins = 0;
				}

				if (playerHealth < 1) {
					playerHealth = 1;
				}
			}

			//FPS calc
			++frames;
			overtime = clock() - nextUpdate;
			if (overtime > 0)
			{
				fps = frames / (float)(1 + (float)overtime / (float)CLOCKS_PER_SEC);
				frames = 0;
				nextUpdate = clock() + 1 * CLOCKS_PER_SEC;
			}

			int numLoops = 0;
			double ticksPerSec = 0;
			bool endLoop = false;

			//Stopping game speed calculation if the game was paused
			if (wasPaused <= 0) {
				gameRunSpeed = 0;

				while ((ticksPerSec < 5.5 || ticksPerSec > 6.0) && endLoop == false) {

					gameRunSpeed = gameRunSpeed + 0.02;

					ticksPerSec = fps / gameRunSpeed;
					numLoops++;

					if (numLoops > 6000) {
						endLoop = true;
					}
				}
			}
			else {
				gameRunSpeed = 10;
				wasPaused--;
			}


			if (isDevMode == true) {
				cout << "  FPS: " << round(fps) << "  ";
				cout << "Ticks/Sec: " << ticksPerSec << " ";
				cout << "Game Speed: " << setprecision(2) << fixed << gameRunSpeed << " ";
			}

			if (gameCounter >= (gameRunSpeed * 10)) {
				if (arrowCooldown > 0 && arrowCooldown < 5) {
					arrowCooldown++;
				}
				else {
					arrowCooldown = 0;
				}

				if (playerIsStunned == true) {
					keyPress = true;
					if (playerStunnedCounter < 20) {
						playerStunnedCounter++;
					}
					else {
						playerIsStunned = false;
						playerStunnedCounter = 0;
					}
				}

				BladeTrapAI();
				TryCreateWallMaster();
				for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
					TryEnemyMove(enemiesLoadedRoom.at(i));
				}

				//Boomerang code
				for (int i = 0; i < boomerangsLoadedRoom.size(); i++) {
					int rows = boomerangsLoadedRoom.at(i).GetLocationRows();
					int cols = boomerangsLoadedRoom.at(i).GetLocationCols();
					int dir = boomerangsLoadedRoom.at(i).GetTravelDirection();
					bool returning = boomerangsLoadedRoom.at(i).GetReturning();
					//cout << "Is Returning: " << returning << endl;
					//cout << "Thrown by enemy: " << boomerangsLoadedRoom.at(i).GetThrownByEnemy() << endl;
					if (counter >= 1) {
						RunSound("arrow/boom");
						counter = 0;
					}
					else {
						counter++;
					}
					if (returning == false) {
						if (dir == 0) { //Going up
							if (loadedRoom[rows - 1][cols] == 48) {
								loadedRoom[rows][cols] = 48;
								boomerangsLoadedRoom.at(i).SetLocation(rows - 1, cols);
								loadedRoom[rows - 1][cols] = 236;
							}
							else if (loadedRoom[rows - 1][cols] == 80) {
								playerIsStunned = true;
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else if (IsEnemy(rows - 1, cols) == true) {
								for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
									if (enemiesLoadedRoom.at(k).GetLocationRows() == (rows - 1) && enemiesLoadedRoom.at(k).GetLocationCols() == cols) {
										enemiesLoadedRoom.at(k).SetStunned(true);
									}
								}
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else {
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
						}
						else if (dir == 1) { //Going right
							if (loadedRoom[rows][cols + 1] == 48) {
								loadedRoom[rows][cols] = 48;
								boomerangsLoadedRoom.at(i).SetLocation(rows, cols + 1);
								loadedRoom[rows][cols + 1] = 236;
							}
							else if (loadedRoom[rows][cols + 1] == 80) {
								playerIsStunned = true;
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else if (IsEnemy(rows, cols + 1) == true) {
								for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
									if (enemiesLoadedRoom.at(k).GetLocationRows() == rows && enemiesLoadedRoom.at(k).GetLocationCols() == (cols + 1)) {
										enemiesLoadedRoom.at(k).SetStunned(true);
									}
								}
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else {
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
						}
						else if (dir == 2) { //Going down
							if (loadedRoom[rows + 1][cols] == 48) {
								loadedRoom[rows][cols] = 48;
								boomerangsLoadedRoom.at(i).SetLocation(rows + 1, cols);
								loadedRoom[rows + 1][cols] = 236;
							}
							else if (loadedRoom[rows + 1][cols] == 80) {
								playerIsStunned = true;
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else if (IsEnemy(rows + 1, cols) == true) {
								for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
									if (enemiesLoadedRoom.at(k).GetLocationRows() == (rows + 1) && enemiesLoadedRoom.at(k).GetLocationCols() == cols) {
										enemiesLoadedRoom.at(k).SetStunned(true);
									}
								}
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else {
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
						}
						else if (dir == 3) { //Going left
							if (loadedRoom[rows][cols - 1] == 48) {
								loadedRoom[rows][cols] = 48;
								boomerangsLoadedRoom.at(i).SetLocation(rows, cols - 1);
								loadedRoom[rows][cols - 1] = 236;
							}
							else if (loadedRoom[rows][cols - 1] == 80) {
								playerIsStunned = true;
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else if (IsEnemy(rows, cols - 1) == true) {
								for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
									if (enemiesLoadedRoom.at(k).GetLocationRows() == rows && enemiesLoadedRoom.at(k).GetLocationCols() == (cols - 1)) {
										enemiesLoadedRoom.at(k).SetStunned(true);
									}
								}
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
							else {
								boomerangsLoadedRoom.at(i).SetReturning(true);
							}
						}
					}
					else if (returning == true) {

						if (loadedRoom[rows][cols] == 236) {
							loadedRoom[rows][cols] = 48;
						}

						if (boomerangsLoadedRoom.at(i).GetThrownByEnemy() == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == boomerangsLoadedRoom.at(i).GetOriginRow() && enemiesLoadedRoom.at(k).GetLocationCols() == boomerangsLoadedRoom.at(i).GetOriginCol()) {

									if (rows != boomerangsLoadedRoom.at(i).GetOriginRow()) {
										if (rows < boomerangsLoadedRoom.at(i).GetOriginRow()) { //Object is above the enemy
											rows++;
										}
										else {
											rows--;
										}
									}

									if (cols != boomerangsLoadedRoom.at(i).GetOriginCol()) {
										if (cols < boomerangsLoadedRoom.at(i).GetOriginCol()) { //Object is to the left of the enemy
											cols++;
										}
										else {
											cols--;
										}
									}
								}
							}

						}
						else {
							if (rows != charYLoc) {
								if (rows < charYLoc) { //Object is above the player
									rows++;
								}
								else {
									rows--;
								}
							}

							if (cols != charXLoc) {
								if (cols < charXLoc) { //Object is to the left of the player
									cols++;
								}
								else {
									cols--;
								}
							}
						}

						if (loadedRoom[rows][cols] == 48) {
							boomerangsLoadedRoom.at(i).SetLocation(rows, cols);
							loadedRoom[rows][cols] = 236;
						}

						else if (IsEnemy(rows, cols) == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == rows && enemiesLoadedRoom.at(k).GetLocationCols() == cols) {
									boomerangsLoadedRoom.erase(boomerangsLoadedRoom.begin() + i);
									enemiesLoadedRoom.at(k).SetBoomerangState(false);
								}
							}
						}
						else if (loadedRoom[rows][cols] == 80) {
							boomerangsLoadedRoom.erase(boomerangsLoadedRoom.begin() + i);
							boomActive = false;
						}
						else {
							boomerangsLoadedRoom.at(i).SetLocation(rows, cols);
							//Just not visually updating the location.
						}

					}

				}

				//Arrow code
				for (int i = 0; i < arrowsLoadedRoom.size(); i++) {
					int rows = arrowsLoadedRoom.at(i).GetLocationRows();
					int cols = arrowsLoadedRoom.at(i).GetLocationCols();
					int dir = arrowsLoadedRoom.at(i).GetTravelDirection();

					loadedRoom[rows][cols] = 48;

					if (dir == 0) { //Going up
						if (loadedRoom[rows - 1][cols] == 48) {
							arrowsLoadedRoom.at(i).SetLocation(rows - 1, cols);
							loadedRoom[rows - 1][cols] = 94;
						}
						else if (IsEnemy(rows - 1, cols) == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == (rows - 1) && enemiesLoadedRoom.at(k).GetLocationCols() == cols) {
									if (enemiesLoadedRoom.at(k).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(k).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(k).GetLocationRows() == 0 || enemiesLoadedRoom.at(k).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(k).GetLocationCols() == 0 || enemiesLoadedRoom.at(k).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 50;
											}
											else {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											}
										}

										enemiesLoadedRoom.at(k).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
										CheckDrop();
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(k).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(k).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(k).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
											CheckDrop();
										}
									}
								}
							}
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
						else {
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
					}
					else if (dir == 1) { //Going right
						if (loadedRoom[rows][cols + 1] == 48) {
							arrowsLoadedRoom.at(i).SetLocation(rows, cols + 1);
							loadedRoom[rows][cols + 1] = 62;
						}
						else if (IsEnemy(rows, cols + 1) == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == rows && enemiesLoadedRoom.at(k).GetLocationCols() == (cols + 1)) {
									if (enemiesLoadedRoom.at(k).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(k).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(k).GetLocationRows() == 0 || enemiesLoadedRoom.at(k).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(k).GetLocationCols() == 0 || enemiesLoadedRoom.at(k).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 50;
											}
											else {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											}
										}

										enemiesLoadedRoom.at(k).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
										CheckDrop();
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(k).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(k).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(k).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
											CheckDrop();
										}
									}
								}
							}
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
						else {
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
					}
					else if (dir == 2) { //Going down
						if (loadedRoom[rows + 1][cols] == 48) {
							arrowsLoadedRoom.at(i).SetLocation(rows + 1, cols);
							loadedRoom[rows + 1][cols] = 118;
						}
						else if (IsEnemy(rows + 1, cols) == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == (rows + 1) && enemiesLoadedRoom.at(k).GetLocationCols() == cols) {
									if (enemiesLoadedRoom.at(k).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(k).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(k).GetLocationRows() == 0 || enemiesLoadedRoom.at(k).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(k).GetLocationCols() == 0 || enemiesLoadedRoom.at(k).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 50;
											}
											else {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											}
										}

										enemiesLoadedRoom.at(k).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
										CheckDrop();
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(k).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(k).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(k).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
											CheckDrop();
										}
									}
								}
							}
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
						else {
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
					}
					else if (dir == 3) { //Going left
						if (loadedRoom[rows][cols - 1] == 48) {
							arrowsLoadedRoom.at(i).SetLocation(rows, cols - 1);
							loadedRoom[rows][cols - 1] = 60;
						}
						else if (IsEnemy(rows, cols - 1) == true) {
							for (int k = 0; k < enemiesLoadedRoom.size(); k++) {
								if (enemiesLoadedRoom.at(k).GetLocationRows() == rows && enemiesLoadedRoom.at(k).GetLocationCols() == (cols - 1)) {
									if (enemiesLoadedRoom.at(k).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(k).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(k).GetLocationRows() == 0 || enemiesLoadedRoom.at(k).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(k).GetLocationCols() == 0 || enemiesLoadedRoom.at(k).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 50;
											}
											else {
												loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											}
										}

										enemiesLoadedRoom.at(k).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
										CheckDrop();
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(k).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(k).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows()][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(k).GetLocationRows() + 1][enemiesLoadedRoom.at(k).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(k).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + k);
											CheckDrop();
										}
									}
								}
							}
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
						else {
							arrowsLoadedRoom.erase(arrowsLoadedRoom.begin() + i);
						}
					}
				}

				//New Bomb Code
				for (int i = 0; i < bombsInGame.size(); i++) {
					int bombCounter = bombsInGame.at(i).GetTimer();
					int bombLocRows = bombsInGame.at(i).GetLocationRows();
					int bombLocCols = bombsInGame.at(i).GetLocationCols();
					int bombLocRoom = bombsInGame.at(i).GetRoomLocated();
					bool inWallExplode = bombsInGame.at(i).GetExplodeDoor();
					bool inWallHappen = false;

					if (bombLocRoom == currentRoom) { //Bomb is only active if the player is in that room.
						if (bombCounter > 15) { //Explode

							if (inWallExplode == false) {
								//Used to open bombable walls. Clears the other half.
								if (loadedRoom[bombLocRows + 1][bombLocCols] == 45) {
									loadedRoom[bombLocRows + 1][bombLocCols] = 51;

									if (loadedRoom[bombLocRows + 1][bombLocCols + 1] == 45) {
										loadedRoom[bombLocRows + 1][bombLocCols + 1] = 51;
									}
									else if (loadedRoom[bombLocRows + 1][bombLocCols - 1] == 45) {
										loadedRoom[bombLocRows + 1][bombLocCols - 1] = 51;
									}
									else {
										system("CLR");
										cout << "Error: Other half of bomb wall not found. OUTSIDE WALL. 0" << endl;
										Sleep(2000);
									}
								}
								else if (loadedRoom[bombLocRows - 1][bombLocCols] == 45) {
									loadedRoom[bombLocRows - 1][bombLocCols] = 51;

									if (loadedRoom[bombLocRows - 1][bombLocCols + 1] == 45) {
										loadedRoom[bombLocRows - 1][bombLocCols + 1] = 51;
									}
									else if (loadedRoom[bombLocRows - 1][bombLocCols - 1] == 45) {
										loadedRoom[bombLocRows - 1][bombLocCols - 1] = 51;
									}
									else {
										system("CLR");
										cout << "Error: Other half of bomb wall not found. OUTSIDE WALL. 1" << endl;
										Sleep(2000);
									}
								}
							}
							else {
								//Used to open bombable walls. Clears the other half. For when bombs are placed inside the wall piece.

								if (loadedRoom[bombLocRows][bombLocCols + 1] == 45 && bombLocCols == 6) {
									loadedRoom[bombLocRows][bombLocCols] = 51;
									loadedRoom[bombLocRows][bombLocCols + 1] = 51;
									inWallHappen = true;

									if (currentRoom == 6) {
										openDoors.doorFour = true;
									}
									else if (currentRoom == 7) {
										openDoors.doorFive = true;
									}
									else if (currentRoom == 10) {
										openDoors.doorFour = true;
									}
									else if (currentRoom == 11) {
										openDoors.doorFive = true;
									}

								}
								else if (loadedRoom[bombLocRows][bombLocCols - 1] == 45 && bombLocCols == 7) {
									loadedRoom[bombLocRows][bombLocCols] = 51;
									loadedRoom[bombLocRows][bombLocCols - 1] = 51;
									inWallHappen = true;

									if (currentRoom == 6) {
										openDoors.doorFour = true;
									}
									else if (currentRoom == 7) {
										openDoors.doorFive = true;
									}
									else if (currentRoom == 10) {
										openDoors.doorFour = true;
									}
									else if (currentRoom == 11) {
										openDoors.doorFive = true;
									}

								}
							}

							//Player detection

							if ((charYLoc == (bombLocRows + 1) && charXLoc == (bombLocCols)) || (charYLoc == (bombLocRows - 1) && charXLoc == (bombLocCols))) { //If the player is above or below the bomb
								playerHealth--;
								RunSound("linkHurt");
							}
							else if ((charYLoc == (bombLocRows) && charXLoc == (bombLocCols - 1)) || (charYLoc == (bombLocRows) && charXLoc == (bombLocCols + 1))) { //If the player is to the left or right of the bomb
								playerHealth--;
								RunSound("linkHurt");
							}
							else if ((charYLoc == (bombLocRows - 1) && charXLoc == (bombLocCols - 1)) || (charYLoc == (bombLocRows - 1) && charXLoc == (bombLocCols + 1))) { //If the player is to the upperleft or upperright of the bomb
								playerHealth--;
								RunSound("linkHurt");
							}
							else if ((charYLoc == (bombLocRows + 1) && charXLoc == (bombLocCols - 1)) || (charYLoc == (bombLocRows + 1) && charXLoc == (bombLocCols + 1))) { //If the player is to the lowerleft or lowerright of the bomb
								playerHealth--;
								RunSound("linkHurt");
							}

							//Enemy detection

							for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
								if ((enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows + 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols)) || (enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows - 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols))) { //If an enemy is above or below the bomb
									if (currentRoom != 16 && enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(i).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(i).GetLocationRows() == 0 || enemiesLoadedRoom.at(i).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(i).GetLocationCols() == 0 || enemiesLoadedRoom.at(i).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 50;
											}
										}

										enemiesLoadedRoom.at(i).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
										CheckDrop();
									}
									else if (enemiesLoadedRoom.at(i).GetEnemyType() == "dragon") {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(i).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
											CheckDrop();
										}
									}
								}
								else if ((enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols - 1)) || (enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols + 1))) { //If an enemy is to the left or right of the bomb
									if (currentRoom != 16 && enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(i).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(i).GetLocationRows() == 0 || enemiesLoadedRoom.at(i).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(i).GetLocationCols() == 0 || enemiesLoadedRoom.at(i).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 50;
											}
										}

										enemiesLoadedRoom.at(i).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
										CheckDrop();
									}
									else if (enemiesLoadedRoom.at(i).GetEnemyType() == "dragon") {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(i).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
											CheckDrop();
										}
									}
								}
								else if ((enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows - 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols - 1)) || (enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows - 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols + 1))) { //If an enemy is to the upperleft or upperright of the bomb
									if (currentRoom != 16 && enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(i).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(i).GetLocationRows() == 0 || enemiesLoadedRoom.at(i).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(i).GetLocationCols() == 0 || enemiesLoadedRoom.at(i).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 50;
											}
										}

										enemiesLoadedRoom.at(i).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
										CheckDrop();
									}
									else if (enemiesLoadedRoom.at(i).GetEnemyType() == "dragon") {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(i).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
											CheckDrop();
										}
									}
								}
								else if ((enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows + 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols - 1)) || (enemiesLoadedRoom.at(i).GetLocationRows() == (bombLocRows + 1) && enemiesLoadedRoom.at(i).GetLocationCols() == (bombLocCols + 1))) { //If an enemy is to the lowerleft or lowerright of the bomb
									if (currentRoom != 16 && enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										//This is to check if the wallmaster died in the wall.
										if (enemiesLoadedRoom.at(i).GetEnemyType() != "wallmaster") {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
										}
										else {
											if (enemiesLoadedRoom.at(i).GetLocationRows() == 0 || enemiesLoadedRoom.at(i).GetLocationRows() == 8) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 49;
											}
											else if (enemiesLoadedRoom.at(i).GetLocationCols() == 0 || enemiesLoadedRoom.at(i).GetLocationCols() == 13) {
												loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 50;
											}
										}

										enemiesLoadedRoom.at(i).RunDrop();
										enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
										CheckDrop();
									}
									else if (enemiesLoadedRoom.at(i).GetEnemyType() == "dragon") {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows()][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols()] = 48;
											loadedRoom[enemiesLoadedRoom.at(i).GetLocationRows() + 1][enemiesLoadedRoom.at(i).GetLocationCols() + 1] = 48;
											enemiesLoadedRoom.at(i).RunDrop();
											enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
											CheckDrop();
										}
									}
								}
							}
							RunSound("bombBlow");
							if (inWallExplode == false) {
								loadedRoom[bombLocRows][bombLocCols] = 48;
							}
							else if (loadedRoom[bombLocRows][bombLocCols] != 51 && (bombLocRows == 0 || bombLocRows == 8)) {
								loadedRoom[bombLocRows][bombLocCols] = 49;
							}
							else if (inWallHappen == false) {
								loadedRoom[bombLocRows][bombLocCols] = 50;
							}
							bombsInGame.erase(bombsInGame.begin() + i);
							VerifyBombWalls();
						}
						else {
							bombsInGame.at(i).IncrementTimer();
						}
					}

				}

				//Fireball Code
				if (ballCount > 1) {
					ballCount = 0;
					RunFireballs();
				}
				else {
					ballCount++;
				}

				if (swordDelay > 0) {
					swordDelay--;
				}

				if (swordActive == true) {
					swordDelay = 3;
					for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
						if (enemiesLoadedRoom.at(i).GetDieBySwordStatus() == true) {
							enemiesLoadedRoom.at(i).RunDrop();
							enemiesLoadedRoom.erase(enemiesLoadedRoom.begin() + i);
						}
					}
					if (swordCounter >= 2) {
						swordActive = false;
						swordCounter = 0;
						if (loadedRoom[swordLocRows][swordLocCols] != 72 && loadedRoom[swordLocRows][swordLocCols] != 82) { //No drop is there
							if (swordLocRows == 0 || swordLocRows == 8) { //If you put it in the top or bottom wall
								loadedRoom[swordLocRows][swordLocCols] = 49;
							}
							else if (swordLocCols == 0 || swordLocCols == 13) { //If you put it in the side walls
								loadedRoom[swordLocRows][swordLocCols] = 50;
							}
							else { //If you did not put it in any walls
								loadedRoom[swordLocRows][swordLocCols] = 48;
							}
						}
					}
					else {
						swordCounter++;
					}
				}

				gameCounter = 0;
			}

			gameCounter = gameCounter + 10;

			if (GetKeyState(VK_ESCAPE) & 0x8000 || isConsoleWindowFocused == false) {
				wasPaused = 30; //To state that the game WAS paused. Used for calculating game speed
				bool menuFlag = true;
				int menuCursorPos = 0;
				keyPress = true;
				system("CLS");
				PrintPauseMenu(menuCursorPos);
				RunSound("bombDrop");
				Sleep(300);
				while (menuFlag == true) {

					isConsoleWindowFocused = (GetConsoleWindow() == GetForegroundWindow()); //Update whether or not the window is in focus.

					if (((GetAsyncKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
						keyPress = true;
						if (menuCursorPos != 0) {
							menuCursorPos--;
							PrintPauseMenu(menuCursorPos);
							RunSound("getRupee");
							Sleep(300);
						}
					}

					if (((GetAsyncKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
						keyPress = true;
						if (menuCursorPos != 2) {
							menuCursorPos++;
							PrintPauseMenu(menuCursorPos);
							RunSound("getRupee");
							Sleep(300);
						}
					}

					if (((GetAsyncKeyState(VK_RETURN) & 0x8000) || (GetAsyncKeyState(VK_SPACE) & 0x8000)) && keyPress == false && isConsoleWindowFocused == true) {
						keyPress = true;
						if (menuCursorPos == 0) {
							menuFlag = false;
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
						}
						else if (menuCursorPos == 1) {
							//Enable Cheat menu
							bool cheatMenuFlag = true;
							int cheatMenuCursorPos = 0;
							int selectedMenu = 0;
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
							keyPress = true;
							PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);

							while (cheatMenuFlag == true) {

								if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false) {
									keyPress = true;
									if (cheatMenuCursorPos != 0) {
										cheatMenuCursorPos--;
										if (selectedMenu == 0) {
											PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);
										}
										else if (selectedMenu == 1) {
											PrintCheatMenu(cheatMenuTwo, cheatMenuCursorPos);
										}
										else if (selectedMenu == 2) {
											PrintCheatMenu(cheatMenuThree, cheatMenuCursorPos);
										}
										RunSound("getRupee");
										Sleep(300);
									}
									else if (selectedMenu != 0) {
										selectedMenu--;
										cheatMenuCursorPos = 2;
										if (selectedMenu == 0) {
											PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);
										}
										else if (selectedMenu == 1) {
											PrintCheatMenu(cheatMenuTwo, cheatMenuCursorPos);
										}
										else if (selectedMenu == 2) {
											PrintCheatMenu(cheatMenuThree, cheatMenuCursorPos);
										}
										RunSound("getRupee");
										Sleep(300);
									}
								}

								if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false) {
									keyPress = true;
									if (cheatMenuCursorPos != 2) {
										cheatMenuCursorPos++;
										if (selectedMenu == 0) {
											PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);
										}
										else if (selectedMenu == 1) {
											PrintCheatMenu(cheatMenuTwo, cheatMenuCursorPos);
										}
										else if (selectedMenu == 2) {
											PrintCheatMenu(cheatMenuThree, cheatMenuCursorPos);
										}
										RunSound("getRupee");
										Sleep(300);
									}
									else if (selectedMenu != 2) {
										selectedMenu++;
										cheatMenuCursorPos = 0;
										if (selectedMenu == 0) {
											PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);
										}
										else if (selectedMenu == 1) {
											PrintCheatMenu(cheatMenuTwo, cheatMenuCursorPos);
										}
										else if (selectedMenu == 2) {
											PrintCheatMenu(cheatMenuThree, cheatMenuCursorPos);
										}
										RunSound("getRupee");
										Sleep(300);
									}
								}

								if (((GetKeyState(VK_RETURN) & 0x8000) || (GetKeyState(VK_SPACE) & 0x8000)) && keyPress == false) {
									keyPress = true;

									if (selectedMenu == 0) {
										enabledCheats = true;
										if (cheatMenuCursorPos == 0) {
											if (isInvincible == false) {
												isInvincible = true;
											}
											else {
												isInvincible = false;
											}
										}
										else if (cheatMenuCursorPos == 1) {
											if (infBombs == false) {
												infBombs = true;
											}
											else {
												infBombs = false;
											}
										}
										else if (cheatMenuCursorPos == 2) {
											if (infKeys == false) {
												infKeys = true;
											}
											else {
												infKeys = false;
											}
										}
									}
									else if (selectedMenu == 1) {
										enabledCheats = true;
										if (cheatMenuCursorPos == 0) {
											if (infCoins == false) {
												infCoins = true;
											}
											else {
												infCoins = false;
											}
										}
										else if (cheatMenuCursorPos == 1) {
											if (hasBoomerang == false) {
												hasBoomerang = true;
											}
											else {
												hasBoomerang = false;
											}
										}
										else if (cheatMenuCursorPos == 2) {
											if (hasBow == false) {
												hasBow = true;
											}
											else {
												hasBow = false;
											}
										}
									}
									else if (selectedMenu == 2) {
										if (cheatMenuCursorPos == 0) {
											enabledCheats = true;
											if (hasMap == false || hasCompass == false) {
												hasMap = true;
												hasCompass = true;
											}
											else {
												hasMap = false;
												hasCompass = false;
											}
										}
										else if (cheatMenuCursorPos == 1) {
											enabledCheats = true;
											if (disableWallMasters == false) {
												disableWallMasters = true;
											}
											else {
												disableWallMasters = false;
											}
										}
										else if (cheatMenuCursorPos == 2) { //This is to exit, so no cheats are enabled here.
											cheatMenuFlag = false;
										}
									}

									if (selectedMenu == 0) {
										PrintCheatMenu(cheatMenuOne, cheatMenuCursorPos);
									}
									else if (selectedMenu == 1) {
										PrintCheatMenu(cheatMenuTwo, cheatMenuCursorPos);
									}
									else if (selectedMenu == 2) {
										PrintCheatMenu(cheatMenuThree, cheatMenuCursorPos);
									}

									if (cheatMenuFlag == true) { //If staying in cheat menu, play sound. Otherwise the sound outside the while loop will play
										RunSound("bombDrop");
										Sleep(300);
									}

								}

								if (GetKeyState(VK_ESCAPE) & 0x8000 && keyPress == false) {
									keyPress = true;
									cheatMenuFlag = false;
								}

								if (keyPress == true) {
									keyPress = false;
								}

							}
							RunSound("bombDrop");
							Sleep(300);
							system("CLS");
							PrintPauseMenu(menuCursorPos); //Because being booted out of the cheat menu.

						}
						else if (menuCursorPos == 2) {
							RunSound("bombDrop");
							system("CLS");
							Sleep(300);
							//Saves the rooms before saving the game to file.
							switch (currentRoom) {
							case 1:
								memcpy(roomOne, loadedRoom, sizeof(roomOne));
								enemiesRoomOne = enemiesLoadedRoom;
								break;
							case 2:
								memcpy(roomTwo, loadedRoom, sizeof(roomTwo));
								enemiesRoomTwo = enemiesLoadedRoom;
								break;
							case 3:
								memcpy(roomThree, loadedRoom, sizeof(roomThree));
								enemiesRoomThree = enemiesLoadedRoom;
								break;
							case 4:
								memcpy(roomFour, loadedRoom, sizeof(roomFour));
								enemiesRoomFour = enemiesLoadedRoom;
								break;
							case 5:
								memcpy(roomFive, loadedRoom, sizeof(roomFive));
								enemiesRoomFive = enemiesLoadedRoom;
								break;
							case 6:
								memcpy(roomSix, loadedRoom, sizeof(roomSix));
								enemiesRoomSix = enemiesLoadedRoom;
								break;
							case 7:
								memcpy(roomSeven, loadedRoom, sizeof(roomSeven));
								enemiesRoomSeven = enemiesLoadedRoom;
								break;
							case 8:
								memcpy(roomEight, loadedRoom, sizeof(roomEight));
								enemiesRoomEight = enemiesLoadedRoom;
								break;
							case 9:
								memcpy(roomNine, loadedRoom, sizeof(roomNine));
								enemiesRoomNine = enemiesLoadedRoom;
								break;
							case 10:
								memcpy(roomTen, loadedRoom, sizeof(roomTen));
								enemiesRoomTen = enemiesLoadedRoom;
								break;
							case 11:
								memcpy(roomEleven, loadedRoom, sizeof(roomEleven));
								enemiesRoomEleven = enemiesLoadedRoom;
								break;
							case 12:
								memcpy(roomTwelve, loadedRoom, sizeof(roomTwelve));
								//Room twelve does not use the enemy system
								break;
							case 13:
								memcpy(roomThirteen, loadedRoom, sizeof(roomThirteen));
								enemiesRoomThirteen = enemiesLoadedRoom;
								break;
							case 14:
								memcpy(roomFourteen, loadedRoom, sizeof(roomFourteen));
								enemiesRoomFourteen = enemiesLoadedRoom;
								break;
							case 15:
								memcpy(roomFifteen, loadedRoom, sizeof(roomFifteen));
								enemiesRoomFifteen = enemiesLoadedRoom;
								break;
							case 16:
								memcpy(roomSixteen, loadedRoom, sizeof(roomSixteen));
								enemiesRoomSixteen = enemiesLoadedRoom;
								break;
							case 17:
								memcpy(roomSeventeen, loadedRoom, sizeof(roomSeventeen));
								enemiesRoomSeventeen = enemiesLoadedRoom;
								break;
							case 18:
								memcpy(roomEighteen, loadedRoom, sizeof(roomEighteen));
								break;
							default:
								cout << "Critcal Error: Could not save room" << endl;
								Sleep(5000);
								exit(1);
								break;
							}
							SaveGame(currentLoadedSave);
							gameRunning = false;
							menuFlag = false;
						}
					}

					if (GetAsyncKeyState(VK_ESCAPE) & 0x8000 && keyPress == false && isConsoleWindowFocused == true) {
						keyPress = true;

						menuFlag = false;
						RunSound("bombDrop");
						Sleep(300);
						system("CLS");
					}

					if (keyPress == true) {
						keyPress = false;
					}

				}
			}

			if (movementCounter >= (gameRunSpeed * 10)) {

				if ((GetKeyState('Q') & 0x8000) && hasBow == true) {
					if ((playerCoins >= 1 || infCoins == true) && arrowCooldown == 0) {
						if (lastDirection == 0) { //Going up
							if (loadedRoom[charYLoc - 1][charXLoc] == 48) {
								arrowsLoadedRoom.push_back(Arrow(charYLoc - 1, charXLoc, currentRoom, lastDirection));
								loadedRoom[charYLoc - 1][charXLoc] = 94;
								playerCoins--;
								arrowCooldown = 1;
								RunSound("arrow/boom");
							}
						}
						else if (lastDirection == 1) { //Going right
							if (loadedRoom[charYLoc][charXLoc + 1] == 48) {
								arrowsLoadedRoom.push_back(Arrow(charYLoc, charXLoc + 1, currentRoom, lastDirection));
								loadedRoom[charYLoc][charXLoc + 1] = 62;
								playerCoins--;
								arrowCooldown = 1;
								RunSound("arrow/boom");
							}
						}
						else if (lastDirection == 2) { //Going down
							if (loadedRoom[charYLoc + 1][charXLoc] == 48) {
								arrowsLoadedRoom.push_back(Arrow(charYLoc + 1, charXLoc, currentRoom, lastDirection));
								loadedRoom[charYLoc + 1][charXLoc] = 118;
								playerCoins--;
								arrowCooldown = 1;
								RunSound("arrow/boom");
							}
						}
						else if (lastDirection == 3) { //Going left
							if (loadedRoom[charYLoc][charXLoc - 1] == 48) {
								arrowsLoadedRoom.push_back(Arrow(charYLoc, charXLoc - 1, currentRoom, lastDirection));
								loadedRoom[charYLoc][charXLoc - 1] = 60;
								playerCoins--;
								arrowCooldown = 1;
								RunSound("arrow/boom");
							}
						}
					}
				}

				if ((GetKeyState('E') & 0x8000) && boomActive == false && hasBoomerang == true) {
					if (lastDirection == 0) { //Going up
						if (loadedRoom[charYLoc - 1][charXLoc] == 48) {
							boomerangsLoadedRoom.push_back(Boomerang(charYLoc - 1, charXLoc, currentRoom, lastDirection));
							loadedRoom[charYLoc - 1][charXLoc] = 236;
							boomActive = true;
						}
					}
					else if (lastDirection == 1) { //Going right
						if (loadedRoom[charYLoc][charXLoc + 1] == 48) {
							boomerangsLoadedRoom.push_back(Boomerang(charYLoc, charXLoc + 1, currentRoom, lastDirection));
							loadedRoom[charYLoc][charXLoc + 1] = 236;
							boomActive = true;
						}
					}
					else if (lastDirection == 2) { //Going down
						if (loadedRoom[charYLoc + 1][charXLoc] == 48) {
							boomerangsLoadedRoom.push_back(Boomerang(charYLoc + 1, charXLoc, currentRoom, lastDirection));
							loadedRoom[charYLoc + 1][charXLoc] = 236;
							boomActive = true;
						}
					}
					else if (lastDirection == 3) { //Going left
						if (loadedRoom[charYLoc][charXLoc - 1] == 48) {
							boomerangsLoadedRoom.push_back(Boomerang(charYLoc, charXLoc - 1, currentRoom, lastDirection));
							loadedRoom[charYLoc][charXLoc - 1] = 236;
							boomActive = true;
						}
					}
				}

				if ((GetKeyState(VK_SPACE) & 0x8000) && swordActive == false && swordDelay == 0) {
					//keyPress = true;
					if (lastDirection == 2) { //Looking down
						if (IsEnemy(charYLoc + 1, charXLoc)) {//If the player is attacking an enemy
							for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
								if (enemiesLoadedRoom.at(i).GetLocationRows() == (charYLoc + 1) && enemiesLoadedRoom.at(i).GetLocationCols() == charXLoc) {

									if (enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
										}
									}
								}
							}
						}

						if (IsEnemy(charYLoc + 1, charXLoc) || loadedRoom[charYLoc + 1][charXLoc] == 48) { //Valid targets for the sword
							RunSound("swordSlash");
							swordActive = true;
							SwordHandler(swordDirection);
						}
					}
					if (lastDirection == 0) { //Looking up
						if (IsEnemy(charYLoc - 1, charXLoc)) { //If the player is attacking an enemy
							for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
								if (enemiesLoadedRoom.at(i).GetLocationRows() == (charYLoc - 1) && enemiesLoadedRoom.at(i).GetLocationCols() == charXLoc) {

									if (enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
										}
									}
								}
							}
						}

						if (IsEnemy(charYLoc - 1, charXLoc) || loadedRoom[charYLoc - 1][charXLoc] == 48) { //Valid targets for the sword
							RunSound("swordSlash");
							swordActive = true;
							SwordHandler(swordDirection);
						}
					}
					if (lastDirection == 1) { //Looking right
						if (IsEnemy(charYLoc, charXLoc + 1)) { //If the player is attacking an enemy
							for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
								if (enemiesLoadedRoom.at(i).GetLocationRows() == charYLoc && enemiesLoadedRoom.at(i).GetLocationCols() == (charXLoc + 1)) {

									if (enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
										}
									}
								}
							}
						}

						if (IsEnemy(charYLoc, charXLoc + 1) || loadedRoom[charYLoc][charXLoc + 1] == 48) { //Valid targets for the sword
							RunSound("swordSlash");
							swordActive = true;
							SwordHandler(swordDirection);
						}
					}
					if (lastDirection == 3) { //Looking left
						if (IsEnemy(charYLoc, charXLoc - 1)) { //If the player is attacking an enemy
							for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
								if (enemiesLoadedRoom.at(i).GetLocationRows() == charYLoc && enemiesLoadedRoom.at(i).GetLocationCols() == (charXLoc - 1)) {

									if (enemiesLoadedRoom.at(i).GetEnemyType() != "dragon") {
										enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
									}
									else {
										int dragonHealth = enemiesLoadedRoom.at(i).GetEnemyHealth();
										if (dragonHealth > 1) {
											enemiesLoadedRoom.at(i).SetHealth(dragonHealth - 1);
										}
										else {
											enemiesLoadedRoom.at(i).SetDieBySwordStatus(true);
										}
									}
								}
							}
						}
						if (IsEnemy(charYLoc, charXLoc - 1) || loadedRoom[charYLoc][charXLoc - 1] == 48) { //Valid targets for the sword
							RunSound("swordSlash");
							swordActive = true;
							SwordHandler(swordDirection);
						}
					}
				}

				if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && keyPress == false) {
					charYLoc = charYLoc - 1;
					charXLoc = charXLoc;
					lastDirection = 0;
					if (IsWalkable(charYLoc, charXLoc) == true) {
						//Handling sword movement
						if (swordActive == true) {
							swordLocRows = swordLocRows - 1;
							swordLocCols = swordLocCols;
							if (loadedRoom[swordLocRows][swordLocCols] == 48 || loadedRoom[swordLocRows][swordLocCols] == 179 || loadedRoom[swordLocRows][swordLocCols] == 196 || loadedRoom[swordLocRows][swordLocCols] == 80) {
								keyW = true;
								keyPress = true;
								loadedRoom[swordLocRows + 1][swordLocCols] = 48;
							}
							else {
								//Reset Sword Loc
								swordLocRows = swordLocRows + 1;
								swordLocCols = swordLocCols;
								//Reset Player Loc
								charYLoc = charYLoc + 1;
								charXLoc = charXLoc;
							}
						}
						else {
							keyW = true;
							keyPress = true;
						}
					}
					else if ((loadedRoom[charYLoc][charXLoc] == 61 || loadedRoom[charYLoc][charXLoc] == 124) && (playerKeys >= 1 || infKeys == true)) { //For opening locked doors.
						keyW = true;
						keyPress = true;
						playerKeys--;
						RunSound("doorUnlock");
					}
					else if (loadedRoom[charYLoc][charXLoc] == 63) { //For pushable blocks
						if (currentRoom == 9) {
							charYLoc = charYLoc + 1;
							charXLoc = charXLoc;
						}
						else if (currentRoom == 16) {
							loadedRoom[charYLoc - 1][charXLoc] = 42;
							RunSound("secretTheme");
							keyW = true;
							keyPress = true;
						}
					}
					else {
						charYLoc = charYLoc + 1;
						charXLoc = charXLoc;
					}
				}
				if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && keyPress == false) {
					charYLoc = charYLoc;
					charXLoc = charXLoc - 1;
					lastDirection = 3;
					if (IsWalkable(charYLoc, charXLoc) == true) {
						//Handling sword movement
						if (swordActive == true) {
							swordLocRows = swordLocRows;
							swordLocCols = swordLocCols - 1;
							if (loadedRoom[swordLocRows][swordLocCols] == 48 || loadedRoom[swordLocRows][swordLocCols] == 179 || loadedRoom[swordLocRows][swordLocCols] == 196 || loadedRoom[swordLocRows][swordLocCols] == 80) {
								keyA = true;
								keyPress = true;
								loadedRoom[swordLocRows][swordLocCols + 1] = 48;
							}
							else {
								//Reset Sword Loc
								swordLocRows = swordLocRows;
								swordLocCols = swordLocCols + 1;
								//Reset Player Loc
								charYLoc = charYLoc;
								charXLoc = charXLoc + 1;
							}
						}
						else {
							keyA = true;
							keyPress = true;
						}
					}
					else if ((loadedRoom[charYLoc][charXLoc] == 61 || loadedRoom[charYLoc][charXLoc] == 124) && (playerKeys >= 1 || infKeys == true)) { //For opening locked doors.
						keyA = true;
						keyPress = true;
						playerKeys--;
						RunSound("doorUnlock");
					}
					else if (loadedRoom[charYLoc][charXLoc] == 63) { //For pushable blocks
						if (currentRoom == 9) {
							if (loadedRoom[charYLoc][charXLoc - 1] == 48) {
								loadedRoom[charYLoc][charXLoc - 1] = 42;
								keyA = true;
								keyPress = true;
								roomNineBlockPushed = true;
							}
							else {
								charYLoc = charYLoc;
								charXLoc = charXLoc + 1;
							}
						}
						else if (currentRoom == 16) {
							loadedRoom[charYLoc][charXLoc - 1] = 42;
							RunSound("secretTheme");
							keyA = true;
							keyPress = true;
						}
					}
					else {
						//Reset Player Loc
						charYLoc = charYLoc;
						charXLoc = charXLoc + 1;
					}
				}
				if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && keyPress == false) {
					charYLoc = charYLoc + 1;
					charXLoc = charXLoc;
					lastDirection = 2;
					if (IsWalkable(charYLoc, charXLoc) == true) {
						//Handling sword movement
						if (swordActive == true) {
							swordLocRows = swordLocRows + 1;
							swordLocCols = swordLocCols;
							if (loadedRoom[swordLocRows][swordLocCols] == 48 || loadedRoom[swordLocRows][swordLocCols] == 179 || loadedRoom[swordLocRows][swordLocCols] == 196 || loadedRoom[swordLocRows][swordLocCols] == 80) {
								keyS = true;
								keyPress = true;
								loadedRoom[swordLocRows - 1][swordLocCols] = 48;
							}
							else {
								//Reset Sword Loc
								swordLocRows = swordLocRows - 1;
								swordLocCols = swordLocCols;
								//Reset Player Loc
								charYLoc = charYLoc - 1;
								charXLoc = charXLoc;
							}
						}
						else {
							keyS = true;
							keyPress = true;
						}

					}
					else if ((loadedRoom[charYLoc][charXLoc] == 61 || loadedRoom[charYLoc][charXLoc] == 124) && (playerKeys >= 1 || infKeys == true)) { //For opening locked doors.
						keyS = true;
						keyPress = true;
						playerKeys--;
						RunSound("doorUnlock");
					}
					else if (loadedRoom[charYLoc][charXLoc] == 63) { //For pushable blocks
						if (currentRoom == 9) {
							charYLoc = charYLoc - 1;
							charXLoc = charXLoc;
						}
						else if (currentRoom == 16) {
							loadedRoom[charYLoc + 1][charXLoc] = 42;
							RunSound("secretTheme");
							keyS = true;
							keyPress = true;
						}
					}
					else {
						//Reset Player Loc
						charYLoc = charYLoc - 1;
						charXLoc = charXLoc;
					}
				}
				if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && keyPress == false) {
					charYLoc = charYLoc;
					charXLoc = charXLoc + 1;
					lastDirection = 1;
					if (IsWalkable(charYLoc, charXLoc) == true) {
						//Handling sword movement
						if (swordActive == true) {
							swordLocRows = swordLocRows;
							swordLocCols = swordLocCols + 1;
							if (loadedRoom[swordLocRows][swordLocCols] == 48 || loadedRoom[swordLocRows][swordLocCols] == 179 || loadedRoom[swordLocRows][swordLocCols] == 196 || loadedRoom[swordLocRows][swordLocCols] == 80) {
								keyD = true;
								keyPress = true;
								loadedRoom[swordLocRows][swordLocCols - 1] = 48;
							}
							else {
								//Reset Sword Loc
								swordLocRows = swordLocRows;
								swordLocCols = swordLocCols - 1;
								//Reset Player Loc
								charYLoc = charYLoc;
								charXLoc = charXLoc - 1;
							}
						}
						else {
							keyD = true;
							keyPress = true;
						}

					}
					else if ((loadedRoom[charYLoc][charXLoc] == 61 || loadedRoom[charYLoc][charXLoc] == 124) && (playerKeys >= 1 || infKeys == true)) { //For opening locked doors.
						keyD = true;
						keyPress = true;
						playerKeys--;
						RunSound("doorUnlock");
					}
					else if (loadedRoom[charYLoc][charXLoc] == 63) { //For pushable blocks
						if (currentRoom == 9) {
							charYLoc = charYLoc;
							charXLoc = charXLoc - 1;
						}
						else if (currentRoom == 16) { //This isnt allowed in room 16 because its an illegal move for the block
													  //Reset Player Loc
							charYLoc = charYLoc;
							charXLoc = charXLoc - 1;
						}
					}
					else {
						//Reset Player Loc
						charYLoc = charYLoc;
						charXLoc = charXLoc - 1;
					}
				}

				if (GetAsyncKeyState('B') & 0x8000 && keyB == false) { //Player is trying to deploy a bomb.
					if (playerBombs > 0 || infBombs == true) {
						if (lastDirection == 0 && loadedRoom[charYLoc - 1][charXLoc] == 48) {
							playerBombs--;
							loadedRoom[charYLoc - 1][charXLoc] = 66;
							bombsInGame.push_back(Bomb(charYLoc - 1, charXLoc, currentRoom));
							RunSound("bombDrop");
						}
						else if (lastDirection == 1 && loadedRoom[charYLoc][charXLoc + 1] == 48) {
							playerBombs--;
							loadedRoom[charYLoc][charXLoc + 1] = 66;
							bombsInGame.push_back(Bomb(charYLoc, charXLoc + 1, currentRoom));
							RunSound("bombDrop");
						}
						else if (lastDirection == 2 && loadedRoom[charYLoc + 1][charXLoc] == 48) {
							playerBombs--;
							loadedRoom[charYLoc + 1][charXLoc] = 66;
							bombsInGame.push_back(Bomb(charYLoc + 1, charXLoc, currentRoom));
							RunSound("bombDrop");
						}
						else if (lastDirection == 3 && loadedRoom[charYLoc][charXLoc - 1] == 48) {
							playerBombs--;
							loadedRoom[charYLoc][charXLoc - 1] = 66;
							bombsInGame.push_back(Bomb(charYLoc, charXLoc - 1, currentRoom));
							RunSound("bombDrop");
						}
						//The great idea of, "Lets let bombs be placed ON the wall". 
						else if (lastDirection == 0 && (loadedRoom[charYLoc - 1][charXLoc] == 49 || loadedRoom[charYLoc - 1][charXLoc] == 45)) {
							playerBombs--;
							loadedRoom[charYLoc - 1][charXLoc] = 66;
							bombsInGame.push_back(Bomb(charYLoc - 1, charXLoc, currentRoom, true));
							RunSound("bombDrop");
						}
						else if (lastDirection == 1 && loadedRoom[charYLoc][charXLoc + 1] == 50) {
							playerBombs--;
							loadedRoom[charYLoc][charXLoc + 1] = 66;
							bombsInGame.push_back(Bomb(charYLoc, charXLoc + 1, currentRoom, true));
							RunSound("bombDrop");
						}
						else if (lastDirection == 2 && (loadedRoom[charYLoc + 1][charXLoc] == 49 || loadedRoom[charYLoc + 1][charXLoc] == 45)) {
							playerBombs--;
							loadedRoom[charYLoc + 1][charXLoc] = 66;
							bombsInGame.push_back(Bomb(charYLoc + 1, charXLoc, currentRoom, true));
							RunSound("bombDrop");
						}
						else if (lastDirection == 3 && loadedRoom[charYLoc][charXLoc - 1] == 50) {
							playerBombs--;
							loadedRoom[charYLoc][charXLoc - 1] = 66;
							bombsInGame.push_back(Bomb(charYLoc, charXLoc - 1, currentRoom, true));
							RunSound("bombDrop");
						}
					}
				}

			}

			movementCounter = movementCounter + 10;

			if (keyPress == true) {
				movementCounter = 0;
				MovePlayer(loadedRoom, charYLoc, charXLoc, swordActive, swordDirection);
				if (loadedRoom[charYLoc][charXLoc] == 51 || loadedRoom[charYLoc][charXLoc] == 52) { //If the room is being changed. Reset the boomerangs/arrows/fireballs BEFORE saving to the room vector.
					for (int i = 0; i < boomerangsLoadedRoom.size(); i++) {
						if (loadedRoom[boomerangsLoadedRoom.at(i).GetLocationRows()][boomerangsLoadedRoom.at(i).GetLocationCols()] == 236) {
							loadedRoom[boomerangsLoadedRoom.at(i).GetLocationRows()][boomerangsLoadedRoom.at(i).GetLocationCols()] = 48;
						}
					}
					boomerangsLoadedRoom.clear();
					boomActive = false;
					for (int i = 0; i < enemiesLoadedRoom.size(); i++) {
						enemiesLoadedRoom.at(i).SetBoomerangState(false);
					}

					for (int i = 0; i < arrowsLoadedRoom.size(); i++) {
						loadedRoom[arrowsLoadedRoom.at(i).GetLocationRows()][arrowsLoadedRoom.at(i).GetLocationCols()] = 48;
					}
					arrowsLoadedRoom.clear();

					fireballsInGame.clear();

				}
				switch (currentRoom) {
				case 1:
					memcpy(roomOne, loadedRoom, sizeof(roomOne));
					enemiesRoomOne = enemiesLoadedRoom;
					break;
				case 2:
					memcpy(roomTwo, loadedRoom, sizeof(roomTwo));
					enemiesRoomTwo = enemiesLoadedRoom;
					break;
				case 3:
					memcpy(roomThree, loadedRoom, sizeof(roomThree));
					enemiesRoomThree = enemiesLoadedRoom;
					break;
				case 4:
					memcpy(roomFour, loadedRoom, sizeof(roomFour));
					enemiesRoomFour = enemiesLoadedRoom;
					break;
				case 5:
					memcpy(roomFive, loadedRoom, sizeof(roomFive));
					enemiesRoomFive = enemiesLoadedRoom;
					break;
				case 6:
					memcpy(roomSix, loadedRoom, sizeof(roomSix));
					enemiesRoomSix = enemiesLoadedRoom;
					break;
				case 7:
					memcpy(roomSeven, loadedRoom, sizeof(roomSeven));
					enemiesRoomSeven = enemiesLoadedRoom;
					break;
				case 8:
					memcpy(roomEight, loadedRoom, sizeof(roomEight));
					enemiesRoomEight = enemiesLoadedRoom;
					break;
				case 9:
					memcpy(roomNine, loadedRoom, sizeof(roomNine));
					enemiesRoomNine = enemiesLoadedRoom;
					break;
				case 10:
					memcpy(roomTen, loadedRoom, sizeof(roomTen));
					enemiesRoomTen = enemiesLoadedRoom;
					break;
				case 11:
					memcpy(roomEleven, loadedRoom, sizeof(roomEleven));
					enemiesRoomEleven = enemiesLoadedRoom;
					break;
				case 12:
					memcpy(roomTwelve, loadedRoom, sizeof(roomTwelve));
					enemiesRoomTwelve = enemiesLoadedRoom;
					break;
				case 13:
					memcpy(roomThirteen, loadedRoom, sizeof(roomThirteen));
					enemiesRoomThirteen = enemiesLoadedRoom;
					break;
				case 14:
					memcpy(roomFourteen, loadedRoom, sizeof(roomFourteen));
					enemiesRoomFourteen = enemiesLoadedRoom;
					break;
				case 15:
					memcpy(roomFifteen, loadedRoom, sizeof(roomFifteen));
					enemiesRoomFifteen = enemiesLoadedRoom;
					break;
				case 16:
					memcpy(roomSixteen, loadedRoom, sizeof(roomSixteen));
					//enemiesLoadedRoom = enemiesRoomSixteen; //Special code for reseting blade traps
					break;
				case 17:
					memcpy(roomSeventeen, loadedRoom, sizeof(roomSeventeen));
					enemiesRoomSeventeen = enemiesLoadedRoom;
					break;
				case 18:
					memcpy(roomEighteen, loadedRoom, sizeof(roomEighteen));
					break;
				default:
					cout << "Critcal Error: Could not save room" << endl;
					Sleep(5000);
					exit(1);
					break;
				}
				ChangeRoom(loadedRoom, charYLoc, charXLoc);
				keyPress = false;
				keyB = false;
				CheckDrop();
			}
		}

	}
	return 0;
}