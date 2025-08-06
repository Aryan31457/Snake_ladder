#include <bits/stdc++.h>
using namespace std;

class Player {
public:
    string name;
    int position;
    Player(string n) : name(n), position(1) {}
};

class SnakeAndLadder {
    int boardSize;
    int numPlayers;
    int numSnakes;
    int numLadders;
    vector<Player> players;
    map<int, int> snakes;
    map<int, int> ladders;
    int difficulty; // 1: Easy, 2: Medium, 3: Hard
    bool customPositions;
public:
    SnakeAndLadder(int b, int p, int s, int l, int d, bool custom) : boardSize(b), numPlayers(p), numSnakes(s), numLadders(l), difficulty(d), customPositions(custom) {
        setupPlayers();
        if (customPositions) {
            inputSnakesLadders();
        } else {
            setupSnakesLadders();
        }
        displaySnakesLadders();
        displayBoard();
    }

    void setupPlayers() {
        for (int i = 1; i <= numPlayers; ++i) {
            string pname = "Player" + to_string(i);
            players.emplace_back(pname);
        }
    }

    void setupSnakesLadders() {
        srand(time(0));
        set<int> used;
        // Place snakes
        for (int i = 0; i < numSnakes; ++i) {
            int head, tail;
            do {
                head = rand() % (boardSize - 2) + 2;
                tail = rand() % (head - 1) + 1;
            } while (snakes.count(head) || ladders.count(head) || used.count(head) || used.count(tail) || head <= tail);
            snakes[head] = tail;
            used.insert(head);
            used.insert(tail);
        }
        // Place ladders
        for (int i = 0; i < numLadders; ++i) {
            int bottom, top;
            do {
                bottom = rand() % (boardSize - 2) + 1;
                top = rand() % (boardSize - bottom) + bottom + 1;
            } while (ladders.count(bottom) || snakes.count(bottom) || used.count(bottom) || used.count(top) || top <= bottom);
            ladders[bottom] = top;
            used.insert(bottom);
            used.insert(top);
        }
    }

    void inputSnakesLadders() {
        set<int> used;
        cout << "Enter snakes (head tail), head > tail, positions between 2 and " << boardSize-1 << ":\n";
        for (int i = 0; i < numSnakes; ++i) {
            int head, tail;
            while (true) {
                cout << "Snake " << (i+1) << ": ";
                cin >> head >> tail;
                if (head <= tail || head <= 1 || head >= boardSize || tail < 1 || tail >= boardSize || used.count(head) || used.count(tail) || snakes.count(head) || ladders.count(head) || ladders.count(tail)) {
                    cout << "Invalid! Head must be > tail, unique, and within range. Try again.\n";
                } else {
                    snakes[head] = tail;
                    used.insert(head);
                    used.insert(tail);
                    break;
                }
            }
        }
        cout << "Enter ladders (bottom top), top > bottom, positions between 1 and " << boardSize-1 << ":\n";
        for (int i = 0; i < numLadders; ++i) {
            int bottom, top;
            while (true) {
                cout << "Ladder " << (i+1) << ": ";
                cin >> bottom >> top;
                if (top <= bottom || bottom < 1 || top <= 1 || top > boardSize || used.count(bottom) || used.count(top) || ladders.count(bottom) || snakes.count(bottom) || snakes.count(top)) {
                    cout << "Invalid! Top must be > bottom, unique, and within range. Try again.\n";
                } else {
                    ladders[bottom] = top;
                    used.insert(bottom);
                    used.insert(top);
                    break;
                }
            }
        }
    }

    void displaySnakesLadders() {
        cout << "\nSnakes (head -> tail):\n";
        for (auto &s : snakes) {
            cout << s.first << " -> " << s.second << "\n";
        }
        cout << "Ladders (bottom -> top):\n";
        for (auto &l : ladders) {
            cout << l.first << " -> " << l.second << "\n";
        }
        cout << endl;
    }

    void displayBoard() {
        cout << "\nBoard Layout (S=Snake, L=Ladder):\n";
        int width = min(10, boardSize); // for display formatting
        for (int i = 1; i <= boardSize; ++i) {
            string cell = to_string(i);
            if (snakes.count(i)) cell += "(SH)";
            if (find_if(snakes.begin(), snakes.end(), [&](const pair<int,int>& s){return s.second==i;}) != snakes.end()) cell += "(ST)";
            if (ladders.count(i)) cell += "(LB)";
            if (find_if(ladders.begin(), ladders.end(), [&](const pair<int,int>& l){return l.second==i;}) != ladders.end()) cell += "(LT)";
            cout << setw(8) << cell;
            if (i % width == 0 || i == boardSize) cout << "\n";
        }
        cout << endl;
    }

    int rollDice() {
        int maxRoll = 6;
        if (difficulty == 2) maxRoll = 8;
        else if (difficulty == 3) maxRoll = 10;
        return rand() % maxRoll + 1;
    }

    void play() {
        bool won = false;
        int turn = 0;
        while (!won) {
            Player &player = players[turn % numPlayers];
            cout << player.name << "'s turn. Press Enter to roll the dice...";
            cin.ignore();
            int roll = rollDice();
            cout << player.name << " rolled a " << roll << "!\n";
            player.position += roll;
            if (player.position > boardSize) player.position = boardSize;
            if (snakes.count(player.position)) {
                cout << "Oh no! " << player.name << " landed on a snake! Down to " << snakes[player.position] << "\n";
                player.position = snakes[player.position];
            } else if (ladders.count(player.position)) {
                cout << "Yay! " << player.name << " climbed a ladder to " << ladders[player.position] << "\n";
                player.position = ladders[player.position];
            }
            cout << player.name << " is now at position " << player.position << "\n\n";
            if (player.position == boardSize) {
                cout << player.name << " wins!\n";
                won = true;
            }
            turn++;
        }
    }
};

int main() {
    int board, players, snakes, ladders, diff;
    char custom;
    cout << "Enter board size (e.g., 100): ";
    cin >> board;
    cout << "Enter number of players: ";
    cin >> players;
    cout << "Enter number of snakes: ";
    cin >> snakes;
    cout << "Enter number of ladders: ";
    cin >> ladders;
    cout << "Select difficulty (1: Easy, 2: Medium, 3: Hard): ";
    cin >> diff;
    cout << "Do you want to enter custom snake and ladder positions? (y/n): ";
    cin >> custom;
    cin.ignore();
    SnakeAndLadder game(board, players, snakes, ladders, diff, (custom == 'y' || custom == 'Y'));
    game.play();
    return 0;
}