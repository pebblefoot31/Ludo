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


    int num = 0;
    int prev = 0;
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
        Pawn p = {-1, 56, 0};
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
    int max_dist = -2; // Start below the base index of -1
    int ind = -1;

    for (int i = 0; i < (int)v.size(); i++) {
        // Only consider pawns that are out of the base and not finished
        if (v[i].pathIndex > max_dist && !v[i].finished && v[i].pathIndex != -1) {
            max_dist = v[i].pathIndex;
            ind = i;
        }
    }

    // If no pawns are on the board, this returns -1
    // Your take_turn function should check for this!
    return ind;
}

void take_turn(vector<Pawn>& player, vector<int> roll, vector<pair<float, float>>& player_path) {
    if (roll.empty()) return;

    int curr = roll[0];
    int second = (roll.size() > 1) ? roll[1] : 0;
    int play = -1;

    if (curr == 6) {
        play = open_player(player); // Try to get someone out

        if (play != -1) {
            // SUCCESS: A new pawn enters the board
            player[play].pathIndex = 0;
            player[play].dist = 56; // Steps left to reach index 55

            // Apply the bonus roll if it doesn't overshoot
            if (second > 0 && player[play].dist >= second) {
                player[play].pathIndex += second;
                player[play].dist -= second;
            }
        } else {
            // FAIL: No one in base, so move the closest player to home instead
            play = find_closest_player(player);
            int total = curr + second;
            if (play != -1) {
                // Ensure we don't move past the final space
                total = min(total, player[play].dist);

                if (player[play].dist >= total) {
                    player[play].pathIndex += total;
                    player[play].dist -= total;
                }
            }
        }
    } else {
        // Standard turn (Not a 6)
        play = find_closest_player(player);
        int total = curr + second;
        if (play != -1) {
            // Ensure we don't move past the final space
            total = min(total, player[play].dist);

            if (player[play].dist >= total) {
                player[play].pathIndex += total;
                player[play].dist -= total;
            }
        }
    }

    // Update finished flag
    if (play != -1 && player[play].dist == 0) {
        player[play].finished = 1;
    }
}

void capture_pawn(vector<Pawn>& player, vector<pair<float,float>>& player_path,
                  vector<Pawn>& opp, vector<pair<float,float>>& opp_path) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // CRITICAL: Only check coordinates if BOTH pawns are actually on the board
            if (player[i].pathIndex >= 0 && opp[j].pathIndex >= 0) {

                // Extra safety: ensure index doesn't exceed path vector size
                if (player[i].pathIndex < (int)player_path.size() &&
                    opp[j].pathIndex < (int)opp_path.size()) {

                    float p_x = player_path[player[i].pathIndex].first;
                    float p_y = player_path[player[i].pathIndex].second;

                    float o_x = opp_path[opp[j].pathIndex].first;
                    float o_y = opp_path[opp[j].pathIndex].second;

                    if ((p_x == o_x) && (p_y == o_y)) {
                        // Reset opponent to base
                        opp[j].pathIndex = -1;
                        opp[j].dist = 57;
                    }
                }
            }
        }
    }
}


bool has_won(const vector<Pawn>& player) {

    int finished_count = 0;
    for (int i = 0; i < 4; i++) {
        if (player[i].finished) {
            finished_count++;
        }
    }

    return (finished_count == 4);
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

        // Add a small offset based on the pawn index 'p' so they don't overlap perfectly
        // Add a small offset based on the pawn index 'p' so they don't overlap perfectly
        if (player[p].pathIndex == -1) {
            cout << "   pts " << start[p].first << " " << start[p].second << endl;
        } else {
            float offset = p * 0.1;
            cout << "   pts " << path[player[p].pathIndex].first + offset << " " << path[player[p].pathIndex].second + offset << endl;
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

    for (int i = 0; i < rolls.size(); i++) {

        if (i%4 == 0) {
           take_turn(red, rolls[i], red_path);
           capture_pawn(red, red_path, green, green_path);
           capture_pawn(red, red_path, yellow, yellow_path);
           capture_pawn(red, red_path, blue, blue_path);

           if (has_won(red)) {
               break;
           }
        }

        else if (i%4 == 1) {
           take_turn(green, rolls[i], green_path);
           capture_pawn(green, green_path, red, red_path);
           capture_pawn(green, green_path, yellow, yellow_path);
           capture_pawn(green, green_path, blue, blue_path);

           if (has_won(green)) {
               break;
           }

        }

        else if (i%4 == 2) {
           take_turn(yellow, rolls[i], yellow_path);
           capture_pawn(yellow, yellow_path, green, green_path);
           capture_pawn(yellow, yellow_path, red, red_path);
           capture_pawn(yellow, yellow_path, blue, blue_path);

           if (has_won(yellow)) {
               break;
           }

        }

        else if (i%4 == 3) {
           take_turn(blue, rolls[i], blue_path);
           capture_pawn(blue, blue_path, green, green_path);
           capture_pawn(blue, blue_path, yellow, yellow_path);
           capture_pawn(blue, blue_path, red, red_path);

           if (has_won(blue)) {
               break;
           }
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
