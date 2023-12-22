#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <conio.h>
#include <string>
#include <windows.h>
#include <ctime>
#include <stack>
using namespace std;

class Team {
public:
    string teamName;
    int totalPlayers;
    int teamId;
    Team* next;

    Team(int teamId, const string& teamName, int totalPlayers)
        : teamName(teamName), totalPlayers(totalPlayers), teamId(teamId), next(nullptr) {}
};

class Game {
public:
    string gameName;
    int gameId;
    Game* next;

    Game(int gameId, const string& gameName)
        : gameName(gameName), gameId(gameId), next(nullptr) {}
};

class Player {
public:
    string playerName;
    int goalsScored;
    int assists;
    int playerId;
    Team* team;
    Player* next;
    Game* registeredGame; 

    Player(int playerId, const string& playerName, Team* team, int goalsScored, int assists)
        : playerName(playerName), goalsScored(goalsScored), assists(assists), playerId(playerId), team(team), next(nullptr), registeredGame(nullptr) {}
};

stack<int> menuStack;

void insertTeam(int teamId, const string& teamName, int totalPlayers);
void insertPlayer(int playerId, const string& playerName, int teamId, int goalsScored, int assists);
void updatePlayer(int playerId, int goalsScored, int assists);
void deletePlayer(int playerId);
int countPlayers();
void displayTeams();
void displayPlayers(int teamId);
void displayPlayerStats(int playerId);
void insertGame(int gameId, const string& gameName);
void displayGames();
void displayPlayersForGame(int gameId);  
Game* getGameById(int gameId);  
void registerPlayerForGame(int playerId, int gameId);
Player* getPlayerById(int playerId);
void displayPlayersByTeamName(const string& teamName);
Team* getTeamById(int teamId);


void clearScreen();
void printMessage(const string& message);
void printBreak(int lines);
void printTab(int tabs);
void setColor(int color);
void printWelcome();
void loadingBar();
void printMiddle();

Team* teamHead = nullptr;
Player* playerHead = nullptr;
Game* gameHead = nullptr;

bool performLogin() {
    string username, password;
    
    clearScreen();
    cout << "ENTER USERNAME: ";
    cin >> username;
    

    cout << "ENTER PASSWORD: ";
    cin >> password;

    if (username == "sumaiyah" && password == "fast123") {
        printMessage("Login successful!");
        Sleep(1000);
        return true;
    } else {
        printMessage("Invalid credentials. Please try again.");
        Sleep(1000);
        return false;
    }
}

void findTopPlayersForTeam(int teamId, int numPlayers) {
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> playerHeap;

    for (Player* temp = playerHead; temp != nullptr; temp = temp->next) {
        if (temp->team->teamId == teamId) {
            playerHeap.push({temp->goalsScored, temp->playerName});
        }
    }

    printBreak(1);
    cout << "Top " << numPlayers << " Players for Team ID " << teamId << " (Descending Order):\n";
    cout << "=================================================\n";

    int count = 0;
    while (!playerHeap.empty() && count < numPlayers) {
        cout << "Player: " << playerHeap.top().second << " - Goals: " << playerHeap.top().first << "\n";
        playerHeap.pop();
        count++;
    }

    if (count == 0) {
        printMessage("No players found for the specified team ID.");
    }

    cout << "\nPress Enter to continue...";
    getchar();
}

void savePlayerStatsToFile() {
	//cout<<"hi1";
    ofstream outFile("player_stats.txt");

    if (!outFile.is_open()) {
        cout << "Error opening file for writing.\n";
        return;
    }
    Player* temp = playerHead;
    outFile << "PID" << " S"<< " A" <<"\n";
    while (temp != nullptr) {
    //	cout<<"hi";
    	
        outFile << temp->playerId << " " << temp->goalsScored << " " << temp->assists << "\n";
        temp = temp->next;
    }
    outFile.close();
    cout << "Player stats saved to file successfully.\n";
}
void loadPlayerStatsFromFile() {
    ifstream inFile("player_stats.txt");

    if (!inFile.is_open()) {
        cout << "Error opening file for reading.\n";
        return;
    }
    int playerId, goals, assists;
    while (inFile >> playerId >> goals >> assists) {
        Player* player = getPlayerById(playerId);
        if (player != nullptr) {
            player->goalsScored = goals;
            player->assists = assists;
        }
    }
    inFile.close();
    cout << "Player stats loaded from file successfully.\n";
}

int main() {
    system("title ..................League of Glory Management System.......................");
    system("mode con: cols=80 lines=30");

    loadingBar();
    clearScreen();
    bool loggedIn = false;
    do {
        loggedIn = performLogin();
    } while (!loggedIn);

    printWelcome();
    Sleep(300);
    clearScreen();
	
    insertTeam(1, "TROJANS", 5);
    insertTeam(2, "SPARTANS", 5);
    insertTeam(3, "VIKINGS", 5);
	insertTeam(4, "ROMANS", 5);
	insertTeam(5, "OTTOMANS",5);
	
    for (int teamId = 1; teamId <= 5; ++teamId) {
        for (int i = 1; i <= 5; ++i) {
            int playerId = teamId * 100 + i; 
            string playerName = "Player " + to_string(i);
            int goalsScored = rand() % 5 + 1; 
            int assists = rand() % 3 + 1; 

            insertPlayer(playerId, playerName, teamId, goalsScored, assists);
        }
    }

    insertGame(1, "Football");
    insertGame(2, "Basketball");
    insertGame(3, "Badminton");
    insertGame(4, "Table Tennis");
    insertGame(5, "Cricket");
    insertGame(6, "Hockey");
    insertGame(7, "Volleyball");
    insertGame(8, "Golf");
    insertGame(9, "Tennis");
    insertGame(10, "Ludo");
    
    int mainMenuChoice;
    do {
     printBreak(1);
     	cout << "************ M A I N    M E N U ************"<<endl;
        cout << "===> 1. Team Management\n";
        cout << "===> 2. Player Management\n";
        cout << "===> 3. Game Management\n";
        cout << "===> 4. Printing Top 5 Players By Team ID\n";
        cout << "===> 5. Register For a Game\n";
        cout << "===> 6. Display Players Registered For Each Game\n";
        cout << "===> 7. Exit\n";
        cout << "Select option (1-7): ";
		loadPlayerStatsFromFile();
        int mainMenuChoice;
        cin >> mainMenuChoice;

        menuStack.push(mainMenuChoice);
    switch (mainMenuChoice) {
        case 1:
            int teamMenuChoice;
            do {
                clearScreen();
                printTab(4);
                cout << "1. Display Teams\n";
                printTab(4);
                cout << "2. Main Menu\n";
                printTab(4);
                cout << "Select option (1-2): ";
                cin >> teamMenuChoice;
				menuStack.push(teamMenuChoice);
                switch (teamMenuChoice) {
                    case 1:
                        clearScreen();
                        displayTeams();
                        printBreak(2);
                        cout << "1. <-- Back\n";
                        printTab(4);
                        cout << "Select option: ";
                        int backToTeamMenu;
                        cin >> backToTeamMenu;
                        if (backToTeamMenu == 1) {
                            break;
                        } else {
                            printMessage("Invalid option. Please try again.");
                        }
                        	
                        break;
                    case 2:
                        break;
                    default:
                        printMessage("Invalid option. Please try again.");
                }
            } while (teamMenuChoice != 2);
            break;

        case 2:
            int playerMenuChoice;
            do {
                clearScreen();
                printTab(4);
                cout << "1. Display Players\n";
                printTab(4);
                cout << "2. Main Menu\n";
                printTab(4);
                cout << "Select option (1-2): ";
                cin >> playerMenuChoice;
				menuStack.push(playerMenuChoice);

                switch (playerMenuChoice) {
                    case 1:
                        clearScreen();
                        displayPlayers(0);

                        cout << "\n1. Display Player Stats\n";
		                cout << "2. Delete Player\n";
		                cout << "3. <== BACK\n";
		                cout << "Select option (1-3): ";
		                int playerSubMenuChoice;
		                cin >> playerSubMenuChoice;
                        switch (playerSubMenuChoice) {
                            case 1:
                                clearScreen();
                                printTab(4);
                                cout << "Enter Player ID: ";
                                int playerId;
                                cin >> playerId;
                                clearScreen();
                                displayPlayerStats(playerId);
                                printBreak(2);
                                cout << "1. <-- Back\n";
                                printTab(4);
                                cout << "Select option: ";
                                int backToPlayerSubMenu;
                                cin >> backToPlayerSubMenu;
                                
                                if (backToPlayerSubMenu == 1) {
                                    break;
                                } else {
                                    printMessage("Invalid option. Please try again.");
                                }
                                break;
                            case 2:
                                clearScreen();
                                printTab(4);
                                cout << "Enter Player ID to delete: ";
                                int playerToDelete;
                                cin >> playerToDelete;
                                deletePlayer(playerToDelete);
                                printMessage("Player deleted successfully.");
                                printBreak(2);
                                cout << "1. <-- Back\n";
                                printTab(4);
                                cout << "Select option: ";
                                int backToDeletePlayer;
                                cin >> backToDeletePlayer;
                                
                                if (backToDeletePlayer == 1) {
                                    break;
                                } else {
                                    printMessage("Invalid option. Please try again.");
                                }
                                break;
//                            case 3:
//                                clearScreen();
//                        		cout << "Enter Team ID to find top players: ";
//                        		int teamIdToSearch;
//                        		cin >> teamIdToSearch;
//                        		findTopPlayersForTeam(teamIdToSearch, 5);
//                        		break;
                    		case 3:
                        		break;
                    			default:
                        		printMessage("Invalid option. Please try again.");
                				}
                	break;
            case 2:
                break;
            default:
                printMessage("Invalid option. Please try again.");
        }
    } while (playerMenuChoice != 2);
    break;

        case 3:
             int gameMenuChoice;
    do {
        clearScreen();
        printTab(4);
        cout << "1. Display Games\n";
        printTab(4);
        cout << "2. Main Menu\n";
        printTab(4);
        cout << "Select option (1-2): ";
        cin >> gameMenuChoice;
         menuStack.push(gameMenuChoice);

                switch (gameMenuChoice) {
                    case 1:
                        clearScreen();
                        displayGames();
                        printBreak(2);
                        cout << "1. <-- Back\n";
                        printTab(4);
                        cout << "Select option: ";
                        int backToGameMenu;
                        cin >> backToGameMenu;
                        
                        if (backToGameMenu == 1) {
                            break;
                        } else {
                            printMessage("Invalid option. Please try again.");
                        }
                        break;
                     case 2:
                     	break;
                    	default:
                        
						printMessage("Invalid option. Please try again.");
		}
    } while (gameMenuChoice != 2);
    break;

        case 4:
        	clearScreen();
            cout << "Enter Team ID to find top players: ";
			int teamIdToSearch;
            cin >> teamIdToSearch;
            findTopPlayersForTeam(teamIdToSearch, 5);
            break;	
        case 5:
        	 clearScreen();
                        printTab(4);
                        cout << "Enter your Player ID: ";
                        int playerId;
                        cin >> playerId;
                        printBreak(1);
                        displayGames();
                        printBreak(1);
                        cout << "Enter Game ID to register: ";
                        int gameId;
                        cin >> gameId;
                        registerPlayerForGame(playerId, gameId);
                        break;
        	case 6:
                        clearScreen();
                displayPlayersForGame(gameId);
                cout << "\n";
            break;
        		case 7:
            clearScreen();
            printMiddle();
            printTab(4);
            cout << "Thank You For Using Our System\n\n\n\n\n\n\n";
             while (!menuStack.empty()) {
                    menuStack.pop();
                }
                savePlayerStatsToFile();
                
                break;

        default:
            printMessage("Invalid option. Please try again.");
    }
} while (mainMenuChoice != 7);
    return 0;
}

void registerPlayerForGame(int playerId, int gameId) {
    Player* player = getPlayerById(playerId);
    Game* game = getGameById(gameId);
    if (player == nullptr || game == nullptr) {
        printMessage("Player or game not found.");
        cout << "Player ID: " << playerId << ", Game ID: " << gameId << "\n";
        cout << "Player: " << (player ? player->playerName : "null") << ", Game: " << (game ? game->gameName : "null") << "\n";
        return;
    }

    player->registeredGame = game;
    cout << "Player with ID " << playerId << " and Name " << player->playerName << " registered for the game " << game->gameName << ".\n";
    printMessage("Congratulations! You have successfully registered for the game.");
    cout<<endl;
}


Player* getPlayerById(int playerId) {
    Player* temp = playerHead;
    while (temp != nullptr) {
        if (temp->playerId == playerId) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr; 
}


void insertTeam(int teamId, const std::string& teamName, int totalPlayers) {
    Team* newTeam = new Team(teamId, teamName, totalPlayers);

    if (teamHead == nullptr) {
        teamHead = newTeam;
    } else {
        Team* temp = teamHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newTeam;
    }
}

void insertPlayer(int playerId, const string& playerName, int teamId, int goalsScored, int assists) {
    Team* team = getTeamById(teamId);
    if (team == nullptr) {
        printMessage("Team not found. Player not inserted.");
        return;
    }

    Player* newPlayer = new Player(playerId, playerName, team, goalsScored, assists);

    if (playerHead == nullptr) {
        playerHead = newPlayer;
    } else {
        Player* temp = playerHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newPlayer;
    }
}


void updatePlayer(int playerId, int goalsScored, int assists) {
    Player* temp = playerHead;
    while (temp != nullptr) {
        if (temp->playerId == playerId) {
            temp->goalsScored += goalsScored;
            temp->assists += assists;
            break;
        }
        temp = temp->next;
    }
}

void deletePlayer(int playerId) {
    Player* current = playerHead;
    Player* prev = nullptr;

    while (current != nullptr && current->playerId != playerId) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        printMessage("Player not found.");
        return;
    }

    if (prev == nullptr) {
        playerHead = current->next;
    } else {
        prev->next = current->next;
    }

    delete current;
}

int countPlayers() {
    Player* temp = playerHead;
    int count = 0;

    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    return count;
}

void displayTeams() {
    Team* temp = teamHead;
    printBreak(1);
    cout << "==============================================================================\n";
    cout << "| " << setw(10) << "Team ID" << " | " << setw(30) << "Team Name" << " | " << setw(15) << "Total Players" << " |\n";
    cout << "==============================================================================\n";

    while (temp != nullptr) {
        cout << "| " << setw(10) << temp->teamId << " | " << setw(30) << temp->teamName << " | " << setw(15) << temp->totalPlayers << " |\n";
        temp = temp->next;
    }

    cout << "==============================================================================\n";
}

// ...

void displayPlayers(int teamId) {
    Player* temp = playerHead;
    printBreak(1);
    cout << "============================================================================================\n";
    cout << "| " << setw(10) << "Player ID" << " | " << setw(30) << "Player Name" << " | " << setw(15) << "Team Name" << " | " << setw(10) << "Goals Scored" << " | " << setw(10) << "Assists" << " |\n";
    cout << "============================================================================================\n";

    while (temp != nullptr) {
        if (teamId == 0 || temp->team->teamId == teamId) {
            cout << "| " << setw(10) << temp->playerId << " | " << setw(30) << temp->playerName << " | " << setw(15) << temp->team->teamName << " | " << setw(10) << temp->goalsScored << "   | " << setw(10) << temp->assists << " |\n";
        }
        temp = temp->next;
    }

    cout << "============================================================================================\n";
}


void displayPlayerStats(int playerId) {
    printBreak(1);
    cout << "==========================================================================================================\n";
    cout << "| " << setw(15) << "Player ID" << " | " << setw(30) << "Player Name" << " | " << setw(15) << "Team ID" << " | " << setw(15) << "Goals Scored" << " | " << setw(15) << "Assists" << " |\n";
    cout << "==========================================================================================================\n";

    Player* temp = playerHead;
    bool found = false;

    while (temp != nullptr) {
        if (temp->registeredGame != nullptr) {
            cout << "| " << setw(15) << temp->playerId << " | " << setw(30) << temp->playerName << " | " << setw(15) << temp->team->teamId << " | " << setw(15) << temp->goalsScored << " | " << setw(15) << temp->assists << " |\n";
            found = true;
        }
        temp = temp->next;
    }

    cout << "==========================================================================================================\n";

    if (!found) {
        printMessage("No players found who are registered for any game.");
    }
}



void insertGame(int gameId, const std::string& gameName) {
	Game* newGame = new Game(gameId, gameName);
	if (gameHead == nullptr) 
	{
	    gameHead = newGame;
	} 
	else 
	{
	    Game* temp = gameHead;
	    while (temp->next != nullptr) {
	        temp = temp->next;
	    }
	    temp->next = newGame;
	}
}

Team* getTeamById(int teamId) {
    Team* temp = teamHead;
    while (temp != nullptr) {
        if (temp->teamId == teamId) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr; 
}

	string getTeamNameById(int teamId) {
    Team* temp = teamHead;
    while (temp != nullptr) {
        if (temp->teamId == teamId) {
            return temp->teamName;
        }
        temp = temp->next;
    }
    return ""; 
}

void displayGames() {
    Game* temp = gameHead;
    printBreak(1);
    cout << "===============================================\n";
    cout << "| " << std::setw(10) << "Game ID" << " | " << std::setw(30) << "Game Name" << " |\n";
    cout << "===============================================\n";
    
    while (temp != nullptr) {
        std::cout << "| " << std::setw(10) << temp->gameId << " | " << std::setw(30) << temp->gameName << " |\n";
        temp = temp->next;
    }

    cout << "===============================================\n";
}

	
	Game* getGameById(int gameId);

void displayPlayersForGame(int gameId) {
    Game* game = getGameById(gameId);
    if (game == nullptr) {
        printMessage("Game not found.");
        return;
    }

    printBreak(1);
    cout << "=====================================================================================================================\n";
    cout << "| " << setw(15) << "Player ID" << " | " << setw(30) << "Player Name" << " | " << setw(30) << "Team Name" << " | " << setw(30) << "Game Name" << " |\n";
    cout << "=====================================================================================================================\n";

    for (Player* tempPlayer = playerHead; tempPlayer != nullptr; tempPlayer = tempPlayer->next) {
        if (tempPlayer->registeredGame != nullptr && tempPlayer->registeredGame->gameId == gameId) {
            cout << "| " << setw(15) << tempPlayer->playerId << " | " << setw(30) << tempPlayer->playerName << " | " << setw(30) << tempPlayer->team->teamName << " | " << setw(30) << tempPlayer->registeredGame->gameName << " |\n";
        }
    }

    cout << "=====================================================================================================================\n";
}

Game* getGameById(int gameId) {
    Game* temp = gameHead;
    while (temp != nullptr) {
        if (temp->gameId == gameId) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr; 
}

	void clearScreen() {
	system("cls");
	}
	
	void printMessage(const std::string& message) {
	cout << message << "\n";
	}
	
	void printBreak(int lines) {
	for (int i = 0; i < lines; ++i) {
	cout << "\n";
	}
	}
	
	void printTab(int tabs) {
	for (int i = 0; i < tabs; ++i) {
	cout << "\t";
	}
	}
	
	void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
	
void printWelcome() {
    clearScreen();
    printMiddle();
    setColor(15); 
    cout << "\n\n\n\n\n\n\n";
    printTab(4);
    setColor(11); 
    cout << "    WELCOME TO THE LEAGUE OF GLORY MANAGEMENT SYSTEM\n";
    setColor(15); 
    Sleep(900);
    setColor(10); 
    cout << "\n                                    Loading ";
   for (int i = 0; i < 25; ++i) {
        cout << char(219);
        Sleep(50);
    }
    setColor(15); 
    cout << "\n";
}
	
	void loadingBar() {
    printBreak(1);
    cout << "Loading: ";
    for (int i = 0; i < 25; ++i) {
        cout << char(219);
        Sleep(50);
    }
    printBreak(2);
}
	
	void printMiddle() {
	printTab(4);
	for (int i = 0; i < 10; ++i) {
	cout << "\n";
	}
}
