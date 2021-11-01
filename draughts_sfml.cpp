#include<string>
#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
using namespace std;

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
    void changePositionIdSymbol(int index, char new_value){
        id[index] = new_value;
    }
    // int getBestMark(){
    //     //sort
    //     return marks[0];
    // }

    // string getBestMove(){
    //     //sort
    //     return suggested_moves[0];
    // }
    
    
};


Position *position = new Position("111111111111444444440000000000000");
vector <Position> positions;
vector <Position> countedPositions;

bool turn_color = true; //white are first to move
int white_checkers_alive = 12;
int black_checkers_alive = 12;

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
        if(k%2 == 1){
            if(j%2 == 1){
                i = 4*k - 1 + ((j+1)/2);
            }
        }else if(j%2 == 0){
                i = 4*k + (j/2);
        }
    }else{
        cout << "WrongInputFormatError" << '\n';
    }
    return i;
}


bool monoAttack(string moveCommand){
    pair <int, int> fields  = make_pair(convert_field(moveCommand.substr(0,2)), convert_field(moveCommand.substr(3,2)));
    if(fields.first == -1 || fields.second == -1){
        return 1;
    }
    string pos_str = position->getPositionId();
    turn_color = pos_str[32] - 48;
    if(pos_str[fields.second] != '4'){
        return 1;
    }
    if(pos_str[fields.first] - 48 != (int)turn_color || pos_str[fields.first] - 48 != ((int)turn_color) + 2){
        return 1;
    }
    
    int diff = fields.second - fields.first;    
    bool moveIsAllowed = false;
    if( turn_color == 1){
        if((diff == 7 && fields.first%8 != 0) || (diff == 9 && fields.first%8 != 7)){
            diff =fields.first + (diff+1)/2;
            switch (pos_str[diff] - 48){
            case 0:
            case 2:
                moveIsAllowed = true;
                break;            
            default:
                break;
            }
        }
    }else{
        if((diff == -7 && fields.first%8 != 7) || (diff == -9 && fields.first%8 != 0)){
            diff =fields.first + (diff-1)/2;
            switch (pos_str[diff] - 48){
            case 1:
            case 3:
                moveIsAllowed = true;
                break;            
            default:
                break;
            }
        }
    }
    
    if(moveIsAllowed){
        position->changePositionIdSymbol(fields.second, (pos_str[fields.first]-48));
        position->changePositionIdSymbol(fields.first,'4');
        position->changePositionIdSymbol(diff,'4');
        if(turn_color == 1){
            black_checkers_alive--;
            position->changePositionIdSymbol(32,0);
        }else{
            white_checkers_alive--;
            position->changePositionIdSymbol(32,1);
        }
        return 0;
    }
    return 1;
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
    for(int i = 2; i < my_input.length()-2; i+=3){
        if(my_input[i] != ':'){
            return 1;
        }
    }
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

 
bool showFields(Position *position){
    //system("clear");
    cout << '\n' << "   ---------------------------------"<< '\n';
    for(int j = 7; j > -1; j--){
        cout << " " << j+1;
        for(int k = 0; k < 8;k++){
            int i = j*8 + k;
            string sign = " ";
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
            if(color == 0){
                int n = i/2;
                if(i%2 == 1){
                    n = (i-1)/2;
                }
                switch (position->getPositionId()[n]){
                case '0':
                    sign = "B";
                    break;
                case '1':
                    sign = "W";
                    break;
                case '2':
                    sign = "K";
                    break;
                case '3': 
                    sign = "Q";
                    break;
                case '4':
                    sign = "0";
                    break;
                default:
                    return 1;
                }
            } 
            cout << " | " << sign;
        }
        cout << " |" << '\n' << "   ---------------------------------"<< '\n';
    }
    cout << "  /  a | b | c | d | e | f | g | h " << '\n';
    return 0;
}


bool turn(){
    positions.push_back(*position);
    showFields(position);
    for (int i = 0; i < positions.size(); i++){
        cout << positions[i].getPositionId() << '\n';
    }
    
    if (turn_color){
        cout << "white to move: ";
    }else{
         cout << "black to move: ";
    }
    string my_input;
    cin >> my_input;
    if((my_input.length()-2)%3 == 0){
        if(my_input.find(':') == 2){
            cout << "attack" << '\n';
            if(attack(my_input) == 1){
                return 1;
            }
        }else if(my_input.find('-') == 2 && my_input.length() == 5){
            cout << "silent move" << '\n';                
            // if(silentMove(my_input) == 1){
            //     return 1;
            // }
        }else{
            cout << "InputFormatError" << '\n';
            return 1;
        }
    }else{
        cout << "InputFormatError (not allowed number of symbols)" << '\n';
        return 1;
    }
    return 0;
}

void game(){
    while (white_checkers_alive > 0 && black_checkers_alive > 0){
        if(turn() == 1){
            *position = positions.back();
        }
    }
    if(white_checkers_alive == 0){
        cout << "BLACK WON" << '\n';
    }else{
        cout << "WHITE WON" << '\n';
    }
}

int main(int argc, char const *argv[]){
    //game();
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML Draughts");

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        
        for(int j = 7; j > -1; j--){
            for(int k = 0; k < 8;k++){
                sf::RectangleShape field_square(sf::Vector2f(100.f, 100.f));
                int i = j*8 + k;
                string sign = " ";
                bool color = 1;
                sf::Color field_color = sf::Color::Cyan;
                field_square.setFillColor(field_color);
                if(j%2 == 0){
                    if(k%2 == 0){
                        color = 0;
                        field_color = sf::Color::Blue;
                    }
                }else{
                    if(k%2 == 1){
                        color = 0;
                        field_color = sf::Color::Blue;
                    }
                }
                field_square.setFillColor(field_color);
                field_square.setPosition((i%8)*100,(6 - (i/8))*100);
                window.draw(field_square);
                if(color == 0){
                int n = i/2;
                if(i%2 == 1){
                    n = (i-1)/2;
                }
                sf::Texture texture;
                sf::Sprite sprite;
                texture.loadFromFile("figures.png");
                
                switch (position->getPositionId()[n]){
                    case '0':
                        sprite.setTextureRect(sf::IntRect(5,5,50,50));
                        sprite.setTexture(texture);
                        break;
                    case '1':
                        sprite.setTextureRect(sf::IntRect(5,60,50,50));
                        sprite.setTexture(texture);
                        break;
                    case '2':
                        sign = "K";
                        break;
                    case '3': 
                        sign = "Q";
                        break;
                    case '4':
                        break;
                    default:
                        return 1;
                    }
                sprite.setPosition((i%8)*100 + 25,(6 - (i/8))*100 + 25);
                window.draw(sprite);
                } 
            }
        }
        window.display();
        
    }
    return 0;
}
