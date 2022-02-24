#include<iostream>
#include <string>
using namespace std;

//forward declarations
void enterToContinue();
void clearScreen();

class Board { //board class
    char board[8][8];
public:

    Board() {// constructor sets the board empty
        resetBoard();
    }

    void resetBoard() { //sets all squares to empty
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = 69;
    }

//In the previous version I used two different methods  for displaying the board
//in this version I use one display board for both firing and placing ships
    void displayBoard(bool isFiring) { //input tells function if the display is for firing or placing ships
        cout << "   A--B--C--D--E--F--G--H" << endl;
        cout << "--------------------------" << endl;
        for (int row = 0; row < 8; row++) {
            cout << (char) (row + 49) << "|";
            for (int col = 0; col < 8; col++) {
                char fs = board[row][col];
                if (isFiring) //if firing don't display ships
                    cout << " " << ((fs == 69 || fs == 83) ? ' ' : fs) << " ";
                else // if placing display ships
                    cout << " " << (fs == 69 ? ' ' : fs) << " ";
            }
            cout << endl;
            cout << "--------------------------" << endl;
        }
        cout << endl;
    }

    int getSquareState(int x, int y) {//return state of the given square
        return board[y][x];
    }

    bool shipLeft() { //check if any ships left
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (board[i][j] == 83) //if any square with S left
                    return true; //return true
        return false; //otherwise return false
    }

    bool fireAtSquare(int x, int y) { //fires at the given coordinates
        x -= 65;
        y -= 49;
        int st = board[y][x];// get the state of the square
        if (st == 69 || st == 83) { //if the square is empty or a ship
            switch (st) {
                case 69://if the square is empty
                    board[y][x] = 77; //set status to miss
                    cout << "It's a miss!" << endl;
                    break;
                case 83://if the square is ship
                    board[y][x] = 72;//set the state to hit
                    cout << "It's a hit!" << endl;
                    break;
            }
            enterToContinue();//display output
            return true; //if the square state is changed
        } else
            return false; //if the state of the square is miss or hit
    }

    bool placeShip(int x, int y, int dir, int size) {
        x -= 65; //translate the x and y coordimates to array indexes A(64) to 0
        y -= 49; //'0'(49) to 0
        switch (dir) {
            case 69: //East direction
                if (x + size < 9) {//checks if the ship is larger than the east border
                    for (int i = 0; i < size; i++) //checks if the ship from starting x coordinate are in the array boundaries
                        if (getSquareState(x + i, y) != 69)
                            return false;
                    for (int i = 0; i < size; i++)
                        board[y][x + i] = 83;
                    return true;
                } else
                    return false;
            case 87: //West direction
                if (x - size > -2) { //checks if the ship from starting x coordinate are in the array boundaries
                    for (int i = 0; i < size; i++) {
                        if (getSquareState(x - i, y) != 69)
                            return false;
                    }
                    for (int i = 0; i < size; i++)
                        board[y][x - i] = 83;//place the ship
                    return true;
                } else
                    return false;
            case 83: //South direction
                if (y + size < 9) {//checks if the ship from starting y coordinate are in the array boundaries
                    for (int i = 0; i < size; i++)
                        if (getSquareState(x, y + i) != 69) //checks if the there are ships east of start square
                            return false;
                    for (int i = 0; i < size; i++)
                        board[y + i][x] = 83;//place the ship
                    return true;
                } else
                    return false;
            case 78: //North direction
                if (y - size > -2) {//checks if the ship from starting y coordinate are in the array boundaries
                    for (int i = 0; i < size; i++)
                        if (getSquareState(x, y - i) != 69)//checks if the there are ships east of start square
                            return false;
                    for (int i = 0; i < size; i++)
                        board[y - i][x] = 83;//place the ship
                    return true;
                } else
                    return false;
            default:
                cout << "incorrect option" << endl;
                return false;
        }
    }

};//board class ends

class Player {
    string name; //player name
    Board board; //player board
public:

    Player() {
    }

    void setName(string name) { //sets name
        this->name = name;
    }

    string getName() { //gets name
        return name;
    }

    bool fire(int x, int y) { //fires at the player's board at the given location
        return board.fireAtSquare(x, y);
    }

    bool checkLost() { //check if the player lost
        return !board.shipLeft();
    }

    bool placeShip(int x, int y, int dir, int size) { //place ship on the player,s board at the given location
        return board.placeShip(x, y, dir, size);
    }

    void displayBoard(bool isFiring) { //display players
        board.displayBoard(isFiring);
    }
};

void clearScreen() { //clears screen in Bash
    cout << "-------------------" << '\n';
}

void enterToContinue() {//stops before screen clears to make sure users read output
    cout << "Press enter to continue!";
    cin.clear();
    cin.get();
}

void getCoordinates(char& x, char& y) {//improved with a string input
    while (true) {//loop until correct input
        string temp;
        cout << "Enter the coordinates [A-H][1-8]: ";
        cin>>temp;//takes single line input e.g. A1
        if (temp.size() == 2) {//checks if the input is 2 chars long
            x = temp[0]; //reads x from string
            y = temp[1]; //reads y from string
            if ((x < 65 || x > 72) || (y < 49 || y > 56))//checks x and y
                cout << "Invalid entry! Please try again." << endl;
            else
                break;
        }
    }
}

void addShipsMenu(Player& p) { // input is the player who is placing the ships
	//moving ships under this menu makes better code no need for globals
    int ships[] = {2, 3, 3, 4, 5}; //ship sizes
    string names[] = {"Destroyer", "Submarine", "Cruiser", "Battleship", "Carrier"};//ship types
    char x = 0;
    char y = 0;
    char dir = 0;
    clearScreen();
    cout << "------------------------------\n" <<
            "\t Add Ship\n" <<
            "------------------------------\n" <<
            "        No Name      Squares\n" <<
            "------------------------------\n" <<
            "        1  Carrier      5\n" <<
            "        1  Battleship	4\n" <<
            "        1  Cruiser 	3\n" <<
            "        1  Submarine	3\n" <<
            "        1  Destroyer	2\n" <<
            "------------------------------" << endl;
    for (int i = 0; i < 5; i++) { //for all 5 ships
        while (true) {//loop until the current ship placed
            cout << p.getName() << " add a " << names[i] << " size: " << ships[i] << endl;
            getCoordinates(x, y);
            cout << "Enter the direction (N/E/S/W): ";
            cin>>dir;
            if (dir == 69 || dir == 78 || dir == 83 || dir == 87) {
                if (p.placeShip(x, y, dir, ships[i])) {//call player to add the ship to his board
                    clearScreen();
                    p.displayBoard(false);
                    break;
                } else
                    cout << "Invalid entry! Please try again." << endl;
            } else
                cout << "Invalid entry! Please try again." << endl;
        }

    }
    cout << "All ships are added!" << endl;
    enterToContinue();
}

int displayMainMenu() {//main menu
    char menu_selection = 0;
    clearScreen();
    cout << "------------------------------\n" <<
            "         Battleships\n" <<
            "         Main Menu\n" <<
            "------------------------------\n" <<
            "         1. Start Game\n" <<
            "         2. Exit Game\n" << endl;

    while (menu_selection < 49 || menu_selection > 50) {
        cout << "Enter menu selection(1-2): ";
        cin>>menu_selection;

        if (menu_selection < 49 || menu_selection > 50)
            cout << "Invalid entry! Please try again." << endl;
    }
    return menu_selection;
}

void displayFireMenu(Player* p) { //input user who is fired at (not firing)
    char x = 0; //A to H
    char y = 0; //1 to 8
    p->displayBoard(true); //display player board
    while (true) {
        getCoordinates(x, y);//get coordinates to fire from the player who is firing
        if (p->fire(x, y)) //fire at the player's board
            break;
        else
            cout << "You already fired at " << x << y << endl;
    }
}

string getPlayerName(int no) {
    cout << "Enter name ";
    string name;
    clearScreen();
    cin>>name;
    return name;
}

int main() {
    int turn = 0;
    Player players[2];// Players array
    Player *shooting = &players[1]; //player firing
    Player *defending = &players[0]; //player fired at
    int menu_selection = displayMainMenu(); //display main menu
    if (menu_selection == '1') { //play game option
        players[0].setName(getPlayerName(1)); //set names
        players[1].setName(getPlayerName(2));
        if (turn == 0) {
            addShipsMenu(players[0]); //place ships
            addShipsMenu(players[1]);
        }
        do { //do until a player wins
            clearScreen();
            turn++; //increase turn after every firing
            swap(shooting, defending); //swap the player's using std::swap (swap what pointers point to)
            cout << "Turn: " << turn << endl;
            cout << shooting->getName() << " enter coordinates to fire\n" << endl;
            displayFireMenu(defending); //display the board of the defending player (fired at)
        } while (!defending->checkLost()); //check if the defending player lost
        clearScreen();
        defending->displayBoard(true); //display the defending players board since he lost
        cout << shooting->getName() << " you won!" << endl; //Declare the winner
    } else //option 2
        cout << "Goodbye!" << endl;
    return 0;//exit game
}
