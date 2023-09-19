#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <random>

struct Object

{
    std::string name;
    int strength{ 0 };
    int health{ 0 };
};

std::vector<Object> loadMonsters(std::string fileName);
void displayBattle(const Object& player, const std::vector<Object>& monsters);
bool monsterAttack(const std::vector<Object>& monsters, Object& player);
void playerAttack(std::vector<Object>& monsters, const Object& player);
int attack(const Object& attacker);
void defend(Object& defender, int damage);
void heal(Object& player);

std::random_device seed;
std::default_random_engine engine(seed());

int main()
{


    std::normal_distribution<double> randomHealth(30.0, 5.0);
    std::normal_distribution<double> randomStrength(5.0, 1.0);

    Object player
    {
        "Ravager",
        std::max(1, (int)randomStrength(engine)),
        std::max(1, (int)randomHealth(engine))
    };



    std::vector<Object> monsters{ loadMonsters("monsters.txt") };

    std::cout << "Three monsters step up!" << std::endl;
    bool allDead{ false };

    while (player.health > 0 && !allDead)
    {

        displayBattle(player, monsters);

        std::cout << "What will you do here? (a)ttack (h)eal ";
        char command{ ' ' };
        std::cin >> command;

        switch (command)
        {

        case 'a':
        {
            playerAttack(monsters, player);
            break;

        }

        case 'h':

            heal(player);
            break;

        default:

            std::cout << "please enter an a or h" << std::endl;
            break;

        }

        allDead = monsterAttack(monsters, player);

        system("PAUSE");
        system("CLS");

    }

    if (player.health <= 0)

    {

        std::cout << "You've died! R.I.P" << std::endl;

    }

    if (player.health <= 0 && allDead)

    {

        std::cout << "but" << std::endl;

    }

    if (allDead)

    {

        std::cout << "You've killed the monsters" << std::endl;

    }

    system("PAUSE");

}



std::vector<Object> loadMonsters(std::string fileName)

{

    std::vector<Object> monsters;


    std::ifstream fin;
    fin.open(fileName);

    if (fin.is_open())

    {

        int numMonsters{};
        fin >> numMonsters;

        std::string name;
        int strength{};
        int health{};

        for (int i{ 0 }; i < numMonsters; i++)

        {

            fin >> name;
            fin >> strength;
            fin >> health;

            monsters.push_back({ name, strength, health });

        }

    }

    fin.close();

    return monsters;

}



void displayBattle(const Object& player, const std::vector<Object>& monsters) {



    std::cout << player.name << ": " << player.health << std::endl

        << "  Monsters: " << std::endl;

    for (int i{ 0 }; i < monsters.size(); i++)

    {

        std::cout << "   " << i + 1 << ". " << monsters[i].name << ": ";

        if (monsters.at(i).health <= 0)

            std::cout << "<DEAD> " << std::endl;

        else

            std::cout << monsters[i].health << std::endl;

    }

}



bool monsterAttack(const std::vector<Object>& monsters, Object& player)

{

    std::bernoulli_distribution willAttack(.75);
    bool allDead{ true };

    for (const auto& monster : monsters)
    {
        if (monster.health > 0)
        {

            allDead = false;

            if (willAttack(engine))

            {

                std::normal_distribution<double> randomMonsterDamage(monster.strength, 2.0);
                int monsterDamage{ std::max(1, (int)randomMonsterDamage(engine)) };

                player.health -= monsterDamage;
                std::cout << monster.name << " strikes!" << std::endl;
                player.health -= monster.strength;

            }
            else
            {
                std::cout << monster.name << " does nothing..." << std::endl;
            }

        }

    }

    return allDead;

}




int attack(const Object& attacker)

{

    std::normal_distribution<double> randomPlayerDamage(attacker.strength, 2.0);

    int playerDamage{ std::max(1, (int)randomPlayerDamage(engine)) };

    std::cout << attacker.name << " attacked you dealing " << playerDamage << " damage!" << std::endl;

    return playerDamage;

}

void playerAttack(std::vector<Object>& monsters, const Object& player)

{

    std::cout << "Which monster? ";

    int monsterNum{ 0 };
    std::cin >> monsterNum;

    if (monsterNum > 0 && monsterNum <= monsters.size())
    {
        if (monsters.at(monsterNum - 1).health > 0)
        {
            monsters[monsterNum - 1].health -= attack(player);
        }

    }

}



void defend(Object& defender, int damage)
{

    defender.health -= damage;
    std::cout << damage << " damage to " << defender.name << "!!!" << std::endl;

}



void heal(Object& player)
{

    std::normal_distribution<double> randomHeal(player.strength * 2, 3.0);

    player.health += std::max(1, (int)randomHeal(engine));


}