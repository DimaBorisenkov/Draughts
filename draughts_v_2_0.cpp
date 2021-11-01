#include<string>
#include<iostream>
#include<vector>
using namespace std;


class Checker{
    bool color;
    bool isKing = false;
public:
    Checker(bool c, bool i){
        color = c;
        isKing = i;
    }
    Checker(bool c){
        color = c;
    }
    Checker(){}
    bool getColor(){
        return color;
    }
    void transformToKing(){
        isKing = true;
    }
    bool getIsKing(){
        return isKing;
    }
    void setColor(bool newColor){
        color = newColor;
    }
};

class Field{
    bool isEmpty = true;
    bool color;
    Checker checker;
public:
    Field(bool c, Checker check){
        isEmpty = false;
        color = c;
        checker = check;
    }
    Field(bool c){
        color = c;
    }
    Field(){}
    
    Checker getChecker(){
        return checker;
    }
    void setColor(bool newColor){
        color = newColor;
    }
    bool getColor(){
        return color;
    }
    void makeFree(){
        isEmpty = true;
        checker = new Checker();
    }
    bool getIsEmpty(){
        return isEmpty;
    }
    void setChecker(bool checkerColor){
        checker.setColor(checkerColor);
        isEmpty = false;
    }
    bool setStatus(int status_num){
        switch (status_num){
        case 0:
            this->makeFree();
            break;
        case 1: // black checker
            isEmpty = false;
            checker.setColor(0);
            break;
        case 2: // white checker
            isEmpty = false;
            checker.setColor(1);
            break;
        case 3: // black king
            isEmpty = false;
            checker.setColor(0);
            checker.transformToKing();
            break;
        case 4: //white king
            isEmpty = false;
            checker.setColor(1);
            checker.transformToKing();
            break;
        
        default:
            cout << "Not allowed status" << '\n';
            return 1;
            break;
        }
        return 0;
    }
};

class Position{
private:
    string id;
    vector <int> marks;
    vector <string> suggested_moves;
public:
    Position(){}
    Position(string number){
        id = number;
    }
    string getPositionId(){
        return id;
    }
    int getBestMark(){
        return marks[0];
    }
    string getBestMove(){
        return suggested_moves[0];
    }
    
    
};


Field fields [64];
string start_position = "222222222222000000001111111111111";
vector <Position> positions;

bool turn_color = true; //white are first to move
int white_checkers_alive = 12;
int black_checkers_alive = 12;
int sum_of_checkers = 0;


int convert_field(string  symbol_field_name){
    int i = -1;
    if(symbol_field_name.length() == 2){
        int j = 0,k = 0;
        switch (symbol_field_name[0]){
        case 'a':
            j = 0;
            break;
        case 'b':
            j = 1;
            break;
        case 'c':
            j = 2;
            break;
        case 'd':
            j = 3;
            break;
        case 'e':
            j = 4;
            break;
        case 'f':
            j = 5;
            break;
        case 'g':
            j = 6;
            break;
        case 'h':
            j = 7;
            break;
        default:
            cout << "WrongLetterError" << '\n';
            break;
        }
        if((symbol_field_name[1] - '0') > -1 && (symbol_field_name[1] - '0') < 9){
            k = symbol_field_name[1] - '0' - 1;
        }else{
            cout << "WrongNumberError" << '\n';
        }
        i = k*8 + j;   
    }else{
        cout << "WrongInputFormatError" << '\n';
    }
    return i;
}

pair<int, int> convert(string symbol_command){
    int i = -1, j = -1;
    if(symbol_command[2] == '-' || symbol_command[2] == ':'){
        i = convert_field(symbol_command.substr(0,2));
        j = convert_field(symbol_command.substr(3,2));
    }else{
        cout << "WrongInputFormat" << '\n';
    }
    return make_pair(i,j);
}

void move(Field *field_1, Field *field_2){
    field_2->setChecker(field_1->getChecker().getColor());
    field_1->makeFree();
}

bool silentMove(string moveMessage){
        pair <int, int> inputFields = convert(moveMessage);
        Field *field_1 = &fields[inputFields.first];
        Field *field_2 = &fields[inputFields.second];
        int diff = inputFields.first - inputFields.second;
        bool diffIscorrect = false;
        if(turn_color == 0){
            if((diff == 9 && inputFields.first%8 != 0) || (diff == 7 && inputFields.first%8 != 7)){
                diffIscorrect = true;
            }
        }else{
            if((diff == -9 && inputFields.first%8 != 7) || (diff == -7 && inputFields.first%8 != 0)){
                diffIscorrect = true;
            }
        }
        if(diffIscorrect){
            if(field_1->getColor() == 0){
                if(field_1->getIsEmpty() == false){
                    if(field_1->getChecker().getColor() == turn_color){
                        if(field_2->getIsEmpty() == true){
                                move(field_1,field_2);
                            }else{
                                cout << "Field entered second isn't empty" << '\n';
                                return 1;
                            }
                    }else{
                        cout << "Checker on the first entered field is wrong color" << '\n';
                        return 1;
                    }
                }else{
                    cout << "First entered field is empty" << '\n';
                    return 1;
                }
            }else{
                cout << "Checkers play only with black cells" << '\n';
                return 1;
            }
        }else{
            cout << "Can't move so" << '\n';
            return 1;
        }
    return 0;
}

bool monoAttack(string moveMessage){
    if(moveMessage[2] == ':'){
        pair <int, int> inputFields = convert(moveMessage);
        if(inputFields.first == -1 || inputFields.second == -1){
            return 1;
        }
        Field *field_1 = &fields[inputFields.first];
        Field *field_2 = &fields[inputFields.second];
        int diff = inputFields.first - inputFields.second;
        Field *middle_field = &fields[inputFields.second + (diff/2)];
        bool diffIsCorrect = false;
        if((diff == 18 && inputFields.first%8 > 1) || (diff == 14 && inputFields.first%8 < 6)){
            diffIsCorrect = true;
        }
        if((diff == -18 && inputFields.first%8 < 6) || (diff == -14 && inputFields.first%8 > 1)){
            diffIsCorrect = true;
        }
        if(diffIsCorrect){
            if(field_1->getColor() == 0){
                if(field_1->getIsEmpty() == false){
                    if(field_1->getChecker().getColor() == turn_color){
                            if(field_2->getIsEmpty() == true){
                                if(middle_field->getIsEmpty() == false){
                                    if(middle_field->getChecker().getColor() != turn_color){
                                        middle_field->makeFree();
                                        if(middle_field->getChecker().getColor()){
                                            white_checkers_alive--;
                                        }else{
                                            black_checkers_alive--;
                                        }
                                        move(field_1, field_2);
                                    }else{
                                        cout << "Friendly fire!" << '\n';
                                        return 1;
                                    }
                                }else{
                                    cout << "No checker to attack" << '\n';
                                    return 1;
                                }
                            }else{
                                cout << "Field entered second isn't empty" << '\n';
                                return 1;
                            }
                    }else{
                        cout << "Checker on the first entered field is wrong color" << '\n';
                        return 1;
                    }
                }else{
                    cout << "First entered field is empty" << '\n';
                    return 1;
                }
            }else{
                cout << "Checkers play only with black cells" << '\n';
                return 1;
            }
        }else{
            cout << "Can't move so" << '\n';
            return 1;
        }
    }else{
        cout << "InputFormatError" << '\n';
        return 1;
    }
    return 0;
}

bool multipleAttack(string my_input){
    for (int i = 0; i < my_input.length(); i+=3){
        if(monoAttack(my_input.substr(i, 5)) == 1){
            return 1;
        }
    }
    return 0;
}

bool attack(string my_input){
    if(my_input.length() > 5){
        cout << "multipleAttack" << '\n';
        if(multipleAttack(my_input) == 1){
            return 1;
        }
    }else{
        cout << "monoAttack" << '\n';
        if(monoAttack(my_input) == 1){
            return 1;
        }
    }
    return 0;
}


bool turn(){
    if (turn_color){
        cout << "white to move: ";
    }else{
         cout << "black to move: ";
    }
    sum_of_checkers = white_checkers_alive + black_checkers_alive; 
    string my_input;
    cin >> my_input;
    if((my_input.length()-2)%3 == 0){
        if(my_input.find(':') == 2){
            cout << "attack" << '\n';
            if(attack(my_input) == 1){
                if(sum_of_checkers != (white_checkers_alive + black_checkers_alive)){
                    turn_color = !turn_color;
                }
                return 1;
            }
        }else if(my_input.find('-') == 2 && my_input.length() == 5){
            cout << "silent move" << '\n';                
            if(silentMove(my_input) == 1){
                return 1;
            }
        }else{
            cout << "InputFormatError" << '\n';
            return 1;
        }
    }else{
        cout << "InputFormatError" << '\n';
        return 1;
    }
    turn_color = !turn_color;
    return 0;
}

bool position_convert(string position){
    for(int i = 0; i < (position.length()-1);i++){
        int n = 2*i;
        if(i%8 > 3){ n++;}
        if(fields[n].setStatus(position[i]- '0') == 1){
            return 1;
        }
    }
    return 0;
}

void fields_initialization(){
    for(int j = 0; j < 8;j++){
        for (int k = 0; k < 8; k++){
            int i = j*8 + k;
            bool color = 1;
            if(j%2 == 0){
                if(k%2 == 0){
                    color = 0;
                }
            }else{
                if(k%2 == 1){
                    color = 0;
                }
            }
            fields[i].setColor(color);
            // if(color == 0){
            //     if(i < 24){
            //         fields[i].setChecker(true);
            //     }else if(i > 40){
            //         fields[i].setChecker(false);
            //     }
            // }
        }
    }
}
 
void showFields(){
    cout << '\n' << "   ---------------------------------"<< '\n';
    for(int j = 7; j > -1; j--){
        cout << " " << j+1;
        for(int k = 0; k < 8;k++){
            int i = j*8 + k;
            string sign = "0";
            if(fields[i].getIsEmpty()){
                if(fields[i].getColor() == 1){
                    sign = " ";
                }
            }else{
                if(fields[i].getChecker().getColor() == 1){
                    sign = "W";
                }else{
                    sign = "B";
                }
            }
            cout << " | " << sign;
        }
        cout << " |" << '\n' << "   ---------------------------------"<< '\n';
    }
    cout << "  /  a | b | c | d | e | f | g | h " << '\n' << "   ---------------------------------"<< '\n';
}

void game(){
    while (white_checkers_alive > 0 && black_checkers_alive > 0){
        showFields();
        turn();
    }
    showFields();
    if(white_checkers_alive == 0){
        cout << "BLACK WON!" << '\n';
    }else{
        cout << "WHITE WON!" << '\n';
    }
}

int main(int argc, char const *argv[]){
    fields_initialization();
    position_convert(start_position);
    game();
    return 0;
}
