#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <windows.h>
#include "Snake.cpp"
#include<fstream>
#include<string>

using namespace std;

//VARIABLES
map<int, map<int, char> > frame;

float speed = 10.0;

int i, j;

int width = 50, height = 20;

Snake snake;

int headX, headY;

int bodyX, bodyY;

int fruitX, fruitY;

char head = '\351';
char bodyPart = 'o';
char fruit = '\242';


int score;

bool gameOver;

bool direction; // 0 means vertical, 1 means horizontal
bool whichward; // In vertical: 0 means right 1 means left.
				// In horizontal: 0 means up and 1 means down

//FUNCTION DECLARATION
bool Login();
void setUp(); // Settings
void addBody(); // Increase size when eat fruit
void update(); // Update the movement using queue
void generateFruit(); // Generate the fruit
void changeDirection(char dir); // Input of movement
void moveHead(); // Do the movement
void printMap(); // UI

//MAIN FUNCTION
int main()
{
    int choice;
    cout << "1: Register \n2: Login\nYour choice: "; cin >> choice;
    if(choice == 1){
        string username, password;
        cout << "Choose a username: "; cin >> username;
        cout << "Choose a password: "; cin >> password;

        ofstream file;
        file.open("d:\\CODES AND STUFFS\\SnakeGame\\" + username + ".txt");
        file << username << endl << password;
        file.close();
        main();
    }
    else if(choice == 2){
        bool status = Login();

        if(!status){
            cout << "False Login!" << endl;
            system("pause");
            main();
        }
        else{
            cout << "Successfully Login!" << endl;
            setUp();
            generateFruit();

            do {
                update();
                if (_kbhit()) {
                    changeDirection(_getch());
                }
            } while(gameOver == false);

            cout << "Game Over.\n\n";
            return 0;
        }
    }
}

bool Login()
{
    string username, password, checkUN, checkPW;
    cout << "Enter username: "; cin >> username;
    cout << "Enter password: "; cin >> password;

    ifstream read("d:\\CODES AND STUFFS\\SnakeGame\\"  + username + ".txt");
    getline(read, checkUN);
    getline(read, checkPW);

    if(checkUN == username && checkPW == password){
        return true;
    }
    else return false;

}

void setUp() {
    score = 0;

    for(i = 0; i < width; i++)
        frame[0][i] = '\262';
    for(i = 1; i < height - 1; i++){
        frame[i][0] = '\262';
        for(j = 1; j < width - 1; j++)
            frame[i][j] = ' ';
        frame[i][j] = '\262';
    }

    for(i = 0; i < width; i++)
		frame[height-1][i] = '\262';



    srand(time(NULL));
    headX = rand() % (width - 5);
    headY = rand() % (height - 5);

    frame[headY][headX] = head;

    snake.enqueue(headX - 2, headY);
    frame[headY][headX - 2] = bodyPart;
    snake.enqueue(headX - 1, headY);
    frame[headY][headX - 1] = bodyPart;

    printMap();

	direction = 0;
	whichward = 0;

	gameOver = false;

	Sleep(1000/speed);
}

void addBody() {
    moveHead();

    printMap();

    Sleep(1000/speed);
}

void update() {
    moveHead();

    snake.dequeue(bodyX,bodyY);
    frame[bodyY][bodyX] = ' ';

    printMap();
    Sleep(1000/speed);
}

void generateFruit() {
    fruitX = rand() % width;
    fruitY = rand() % height;
    if(frame[fruitY][fruitX] == ' ')
        frame[fruitY][fruitX] = fruit;
    else generateFruit();
 }

void changeDirection(char dir) {
    if((dir == 'w' || dir == 's') && direction == 0){
        if(dir == 'w')
            whichward = 0;
        else
            whichward = 1;
        direction = 1;
    }
    else if((dir == 'a' || dir == 'd') && direction == 1){
        if(dir == 'a')
            whichward = 1;
        else
            whichward = 0;
        direction = 0;
    }
}

void moveHead() {
    snake.enqueue(headX, headY);
    frame[headY][headX] = bodyPart;
    if(direction == 0 && whichward == 0)
        headX++;
    if(direction == 0 && whichward == 1)
        headX--;
    if(direction == 1 && whichward == 0)
        headY--;
    if(direction == 1 && whichward == 1)
        headY++;

    if(headX == fruitX && headY == fruitY){
        score++;
        addBody();
        generateFruit();
    }
    if(frame[headY][headX] == '\262'
        || frame[headY][headX] == bodyPart)
        gameOver = true;

    frame[headY][headX] = head;
}

void printMap() {
	system("cls");
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
				cout << frame[i][j];
			}
		if (i == 10)
			cout << "    Score: " << score;
		cout << endl;
	}
}
