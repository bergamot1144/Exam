
#include <iostream>
#include <vector>


using namespace std;

const int DECK_SIZE = 10;

enum CellState { //состояние ячейки
    EMPTY,
    SHIP,
    HIT,
    MISS
};

class Ship {
public:
    int size;
    vector<pair<int, int>> coordinates;
    Ship(int s) : size(s) {}

    void addCoordinates(int x, int y)   //добавление координат корабля
    {
        coordinates.push_back(make_pair(x, y));
    }
};

class Deck {
private:
    vector<vector<CellState>> setka;
public:
    Deck() : setka(DECK_SIZE, vector<CellState>(DECK_SIZE, EMPTY)) {}     // уточнить строку
    //отображение игр поля
    void display(bool hideShips = false) {
        cout << " ";
        for (int i = 0; i < DECK_SIZE; ++i)
            cout << i << " ";
        cout << endl;


        for (int i = 0; i < DECK_SIZE; ++i) {
            cout << i << " ";
            for (int j = 0; j < DECK_SIZE; ++j) {
                if (setka[i][j] == EMPTY)
                    cout << ". ";
                else if (setka[i][j] == SHIP) 
                    cout << (hideShips ? ". " : "S ");
                
                else if (setka[i][j] == HIT) 
                    cout << "X ";
                
                else if (setka[i][j] == MISS) 
                    cout << "O ";
                
            }
            cout << endl;
        }
    }

    //возможность размещения корабля
    bool canPlaceShip(int x, int y, int size, bool horisontal) 
    {
        if (horisontal) 
        {
            if (y + size > DECK_SIZE) return false;
            for (int i = 0; i < size; i++) 
            {
                if (setka[x][y + i] != EMPTY) return false;
            }
        }
        else {
            if (x + size > DECK_SIZE) return false;
            for (int i = 0; i < size; i++) {
                if (setka[x + i][y] != EMPTY) return false;

            }
        }
        return true;
    }

    //размещение корабля на игр поле

    void placeShip(Ship& ship, int x, int y, bool horisontal)
    {
        for (int i=0;i<ship.size; ++i) 
        {
            if (horisontal) 
            {
                setka[x][y + i] = SHIP;
                ship.addCoordinates(x, y + i);
            }
            else
            {
                setka[x + i][y] = SHIP;
                ship.addCoordinates(x+i, y);
            }
        }
    }

    //выстрел

    bool shoot(int x, int y)
    {
        if (setka[x][y] == SHIP)
        {
            setka[x][y] = HIT;
            return true;
        }
        else if (setka[x][y] == EMPTY)
        {
            setka[x][y] == MISS;
            return false;
        }
        return false;
    }

    //проверка на уничтожение всего флота

    bool Loose()
    {
        for (int i = 0; i < DECK_SIZE; ++i)
        {
            for (int j = 0; j < DECK_SIZE; ++j)
            {
                if (setka[i][j] == SHIP) return false;
            }
        }
        return true;
    }   
};





class Player {
public:
    string name;
    Deck deck;
    vector<Ship> ships;

    Player(string n) : name(n) {}
    // расстанока кораблей
    void placeShips()
    {
        ships.push_back(Ship(4)); 
        ships.push_back(Ship(3));
        ships.push_back(Ship(3)); 
        ships.push_back(Ship(2));
        ships.push_back(Ship(2));
        ships.push_back(Ship(2)); 
        ships.push_back(Ship(1));
        ships.push_back(Ship(1));
        ships.push_back(Ship(1));
        ships.push_back(Ship(1));

        for (Ship& ship : ships) {
            bool placed = false;
            while (!placed)
            {
                int x = rand() % DECK_SIZE;
                int y = rand() % DECK_SIZE;
                bool horisontal = rand() % 2;
                if (deck.canPlaceShip(x, y, ship.size, horisontal))
                {
                    deck.placeShip(ship, x, y, horisontal);
                    placed = true;
                }
            }

        }
    }
    //выстрел по полю другого игрока
    bool shoot(Player& enemy, int x, int y)
    {
        return enemy.deck.shoot(x, y);
    }

};


class Game {
private:
    Player player1;
    Player player2;

    //отображение текущей игры
    void displayDecks()
    {
        cout << player1.name << "'s Deck: " << endl;
        player1.deck.display();
        cout << endl;
        
        cout << player2.name << "'s Deck: " << endl;
        player2.deck.display(true); // Скрываем корабли игрока 2 от игрока 1
        cout << endl;
    }

public:
    Game(string name1, string name2) : player1(name1), player2(name2) {}

    void start()
    {
        srand(time(0));
        player1.placeShips();
        player2.placeShips();

        bool player1Turn = true;
        while (!player1.deck.Loose() && !player2.deck.Loose())
        {
            displayDecks();

            Player& currentPlayer = player1Turn ? player1 : player2; 
            Player& enemy = player1Turn ? player2 : player1;

            int x, y;
            cout << currentPlayer.name << "'s turn. Enter coordinates to shoot (x y): ";
            cin >> x >> y;

            if (currentPlayer.shoot(enemy, x, y))
            {
                cout << "Hit!!!" << endl;
            }
            else
            {
                cout << "Miss:((((" << endl;
            }
            player1Turn = !player1Turn;
        }

        if (player1.deck.Loose())
        {
            cout << player2.name << " wins!!!" << endl;
        }
        else
        {
            cout << player1.name << " wins!!!" << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Game game("One Hit Joe", "Jack Sparrow");
    game.start();
    return 0;
}


/// 1. добавить обводку уничтоженого корабля
/// 2. Сделать переход хода в случае промаха




