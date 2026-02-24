#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;

struct Pawn {
    int pathIndex, dist, finished;
};

//rules are if you roll a 6 you get the next roll too (ignoring triple 6 roll)
void read_rolls(string file_name, vector<vector<int>>& v) {

    ifstream file(file_name);
    if (!file.is_open()) {
        return;
    } 

    string s;


    int num;
    int prev;
    vector<int> r;

    while (getline(file,s)) {
        num = stoi(s);
        
        if (prev==6 && r.size() < 2) {
            r.push_back(num);
            v.push_back(r);
            r.clear();

        } else {
            if (num == 6) {
               r.push_back(num); 
            } else {
                r.push_back(num);
                v.push_back(r);
                r.clear();
            }
        }

        prev = num;
    }
    
}

void init_player(vector<Pawn>& v) {
   
    for (int i = 0; i < 4; i++) {
        Pawn p = {-1, 57, 0};
        v.push_back(p);

    }
}

int open_player(vector<Pawn>& v) {

    for (int i =0; i < v.size(); i++) {
        if (v[i].pathIndex == -1)
            return i;
    }

    return -1;
}

int find_closest_player(vector<Pawn>& v) {
    
    int max = 0;
    int ind = -1;

    for (int i = 0; i < v.size(); i++) {
        if (v[i].pathIndex > max && !(v[i].finished))
            max = v[i].pathIndex;
            ind = i;
    }

    return ind;

}

void take_turn(vector<Pawn>& player, vector<int> roll) {

    int curr = roll[0];
    int total = roll[0];
    int play = 0;

    if (curr == 6) {
         if ((play = open_player(player)) != -1) {
            player[play].pathIndex += roll[1]; 
            player[play].dist -= roll[1];
    
         //make the roll full and play the player that is closest to goal
         } else {
            play = find_closest_player(player);
            total += roll[1];
            if (player[play].dist >= total) {
                player[play].pathIndex += total; //update path
                player[play].dist -= total; //update distance from goal
                
            }
         }
    }
    else {

        //play the player that is closest to goal
        play = find_closest_player(player);
        
        if (player[play].dist >= total) {
            player[play].pathIndex += total; //update path
            player[play].dist -= total; //update distance from goal
            
        }
    }

    if (player[play].dist == 0) {
        player[play].finished = 1;
    }
}

void print_positions(vector<Pawn>& player, vector<pair<int,int>>& start, vector<pair<float,float>>& path, string color) {

    for (int p = 0; p < 4; p++) {

        cout << "newcurve marktype diamond marksize 0.5 0.5 \n";

        if (color == "red") {
            cout << "linetype solid cfill 1 0 0\n"; 
        } else if (color == "green") {
            cout << "linetype solid cfill 0 1 0\n"; 
        } else if (color == "blue") {
            cout << "linetype solid cfill 0 0 1\n"; 
        } else if (color == "yellow") {
            cout << "linetype solid cfill 255 255 0\n"; 
        } 

        if (player[p].pathIndex == -1) {
            cout << "   pts " << start[p].first << " " << start[p].second << endl;
        } else {
            cout << "   pts " << path[player[p].pathIndex].first << " " << path[player[p].pathIndex].second << endl;
        }
    }
    
}

int main(int argc, char *argv[]) {
    
    // RED PATH: Enters (1.5, 8.5), moves Right
    vector<pair<float, float>> red_path = {
        {1.5, 8.5}, {2.5, 8.5}, {3.5, 8.5}, {4.5, 8.5}, {5.5, 8.5}, // Arm 1
        {6.5, 9.5}, {6.5, 10.5}, {6.5, 11.5}, {6.5, 12.5}, {6.5, 13.5}, {6.5, 14.5}, // Arm 2 down
        {7.5, 14.5}, // Bottom edge
        {8.5, 14.5}, {8.5, 13.5}, {8.5, 12.5}, {8.5, 11.5}, {8.5, 10.5}, {8.5, 9.5}, // Arm 3 up
        {9.5, 8.5}, {10.5, 8.5}, {11.5, 8.5}, {12.5, 8.5}, {13.5, 8.5}, {14.5, 8.5}, // Arm 4 right
        {14.5, 7.5}, // Right edge
        {14.5, 6.5}, {13.5, 6.5}, {12.5, 6.5}, {11.5, 6.5}, {10.5, 6.5}, {9.5, 6.5}, // Arm 5 left
        {8.5, 5.5}, {8.5, 4.5}, {8.5, 3.5}, {8.5, 2.5}, {8.5, 1.5}, {8.5, 0.5}, // Arm 6 up
        {7.5, 0.5}, // Top edge
        {6.5, 0.5}, {6.5, 1.5}, {6.5, 2.5}, {6.5, 3.5}, {6.5, 4.5}, {6.5, 5.5}, // Arm 7 down
        {5.5, 6.5}, {4.5, 6.5}, {3.5, 6.5}, {2.5, 6.5}, {1.5, 6.5}, {0.5, 6.5}, // Arm 8 left
        {0.5, 7.5}, // Left edge (turn home)
        {1.5, 7.5}, {2.5, 7.5}, {3.5, 7.5}, {4.5, 7.5}, {5.5, 7.5}, {6.5, 7.5}  // HOME
    };
    
    // BLUE PATH: Enters (6.5, 13.5), moves Up
    vector<pair<float, float>> blue_path = {
        {6.5, 13.5}, {6.5, 12.5}, {6.5, 11.5}, {6.5, 10.5}, {6.5, 9.5},
        {5.5, 8.5}, {4.5, 8.5}, {3.5, 8.5}, {2.5, 8.5}, {1.5, 8.5}, {0.5, 8.5},
        {0.5, 7.5},
        {0.5, 6.5}, {1.5, 6.5}, {2.5, 6.5}, {3.5, 6.5}, {4.5, 6.5}, {5.5, 6.5},
        {6.5, 5.5}, {6.5, 4.5}, {6.5, 3.5}, {6.5, 2.5}, {6.5, 1.5}, {6.5, 0.5},
        {7.5, 0.5},
        {8.5, 0.5}, {8.5, 1.5}, {8.5, 2.5}, {8.5, 3.5}, {8.5, 4.5}, {8.5, 5.5},
        {9.5, 6.5}, {10.5, 6.5}, {11.5, 6.5}, {12.5, 6.5}, {13.5, 6.5}, {14.5, 6.5},
        {14.5, 7.5},
        {14.5, 8.5}, {13.5, 8.5}, {12.5, 8.5}, {11.5, 8.5}, {10.5, 8.5}, {9.5, 8.5},
        {8.5, 9.5}, {8.5, 10.5}, {8.5, 11.5}, {8.5, 12.5}, {8.5, 13.5}, {8.5, 14.5},
        {7.5, 14.5},
        {7.5, 13.5}, {7.5, 12.5}, {7.5, 11.5}, {7.5, 10.5}, {7.5, 9.5}, {7.5, 8.5}
    };
    
    // GREEN PATH: Enters (8.5, 1.5), moves Down
    vector<pair<float, float>> green_path = {
        {8.5, 1.5}, {8.5, 2.5}, {8.5, 3.5}, {8.5, 4.5}, {8.5, 5.5},
        {9.5, 6.5}, {10.5, 6.5}, {11.5, 6.5}, {12.5, 6.5}, {13.5, 6.5}, {14.5, 6.5},
        {14.5, 7.5},
        {14.5, 8.5}, {13.5, 8.5}, {12.5, 8.5}, {11.5, 8.5}, {10.5, 8.5}, {9.5, 8.5},
        {8.5, 9.5}, {8.5, 10.5}, {8.5, 11.5}, {8.5, 12.5}, {8.5, 13.5}, {8.5, 14.5},
        {7.5, 14.5},
        {6.5, 14.5}, {6.5, 13.5}, {6.5, 12.5}, {6.5, 11.5}, {6.5, 10.5}, {6.5, 9.5},
        {5.5, 8.5}, {4.5, 8.5}, {3.5, 8.5}, {2.5, 8.5}, {1.5, 8.5}, {0.5, 8.5},
        {0.5, 7.5},
        {0.5, 6.5}, {1.5, 6.5}, {2.5, 6.5}, {3.5, 6.5}, {4.5, 6.5}, {5.5, 6.5},
        {6.5, 5.5}, {6.5, 4.5}, {6.5, 3.5}, {6.5, 2.5}, {6.5, 1.5}, {6.5, 0.5},
        {7.5, 0.5},
        {7.5, 1.5}, {7.5, 2.5}, {7.5, 3.5}, {7.5, 4.5}, {7.5, 5.5}, {7.5, 6.5}
    };
    
    // YELLOW PATH: Enters (13.5, 6.5), moves Left
    vector<pair<float, float>> yellow_path = {
        {13.5, 6.5}, {12.5, 6.5}, {11.5, 6.5}, {10.5, 6.5}, {9.5, 6.5},
        {8.5, 5.5}, {8.5, 4.5}, {8.5, 3.5}, {8.5, 2.5}, {8.5, 1.5}, {8.5, 0.5},
        {7.5, 0.5},
        {6.5, 0.5}, {6.5, 1.5}, {6.5, 2.5}, {6.5, 3.5}, {6.5, 4.5}, {6.5, 5.5},
        {5.5, 6.5}, {4.5, 6.5}, {3.5, 6.5}, {2.5, 6.5}, {1.5, 6.5}, {0.5, 6.5},
        {0.5, 7.5},
        {0.5, 8.5}, {1.5, 8.5}, {2.5, 8.5}, {3.5, 8.5}, {4.5, 8.5}, {5.5, 8.5},
        {6.5, 9.5}, {6.5, 10.5}, {6.5, 11.5}, {6.5, 12.5}, {6.5, 13.5}, {6.5, 14.5},
        {7.5, 14.5},
        {8.5, 14.5}, {8.5, 13.5}, {8.5, 12.5}, {8.5, 11.5}, {8.5, 10.5}, {8.5, 9.5},
        {9.5, 8.5}, {10.5, 8.5}, {11.5, 8.5}, {12.5, 8.5},
        {13.5, 7.5}, {12.5, 7.5}, {11.5, 7.5}, {10.5, 7.5}, {9.5, 7.5}, {8.5, 7.5}
    };

    vector<Pawn> red;
    vector<Pawn> blue;
    vector<Pawn> green;
    vector<Pawn> yellow;
    
    //initialize pawns vectors
    string name = argv[1];
    vector< vector<int>> rolls;

    read_rolls(name, rolls);

    init_player(red);
    init_player(blue);
    init_player(green);
    init_player(yellow);


    int count = 0;
    int play;

    for (int i = 0; i < rolls.size()-1; i++) {

        if (i%4 == 0) {
           take_turn(red, rolls[i]);
        }

        else if (i%4 == 1) {
           take_turn(green, rolls[i]);
        }

        else if (i%4 == 2) {
           take_turn(yellow, rolls[i]);
        }
        else if (i%4 == 3) {
           take_turn(blue, rolls[i]);
        }
    }

    std::vector<std::pair<int, int>> blue_start = {{2, 4}, {4, 2}, {2, 2}, {4, 4}};
    std::vector<std::pair<int, int>> green_start = {{11, 13}, {11, 11}, {13, 11}, {13, 13}};
    std::vector<std::pair<int, int>> red_start = {{2, 11}, {4, 11}, {2, 13}, {4, 13}};
    std::vector<std::pair<int, int>> yellow_start = {{11, 2}, {11, 4}, {13, 2}, {13, 4}};

    // pipe final locations into file
    print_positions(red, red_start, red_path, "red");
    print_positions(green, green_start, green_path, "green");
    print_positions(yellow, yellow_start, yellow_path, "yellow");
    print_positions(blue, blue_start, blue_path, "blue");

    return 0;
}
