#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <map>

#include "Monster.h"
#include "Player.h"
#include "Item.h"

std::vector<Monster> createMonsters(const Player& player);
void monsterAttack(Player& player, std::vector<Object*>& objects);
void bringOutYourDead(std::vector<Object*>& objects);

void playerAttack(Player& player, std::vector<Object*>& objects);
void levelUp(Player& player);
int calculateAC(const Object& object);
void heal(Player& player);

void displayBattle(const Player& player, const std::vector<Object*>& objects);

void printName(const Object& object);
void printItem(const Item& item);
int attack(Player& player);
void defend(Object& object, int damage);

std::random_device seed;
std::default_random_engine engine(seed());

int main() {
    Player player;
    std::vector<Monster> monsters;
    std::vector<Object*> objects;

    while (!player.isDead()) {
        levelUp(player);
        monsters = createMonsters(player);

        std::cout << monsters.size() << " monster(s) approach!!" << std::endl;
        system("pause");
        system("cls");

        objects.push_back(&player);
        objects.insert(objects.end(), monsters.begin(), monsters.end());

        while (!player.isDead() && !monsters.empty()) {
            displayBattle(player, objects);

            std::cout << "What do you do? (a)ttack (h)eal ";
            char command{ 'x' };
            std::cin >> command;

            switch (command) {
                case 'a':
                    playerAttack(player, objects);
                    break;
                case 'h':
                    heal(player);
                    break;
                default:
                    std::cout << "Please enter 'a' or 'h'" << std::endl;
                    break;
            }

            bringOutYourDead(objects);

            monsterAttack(player, objects);

            system("PAUSE");
            system("CLS");
        }
    }

    if (player.isDead()) {
        std::cout << "You Have Died" << std::endl;
    }
    if (player.isDead() <= 0 && monsters.empty()) {
        std::cout << "BUT" << std::endl;
    }
    if (monsters.empty()) {
        std::cout << "You have killed the monsters!!!" << std::endl;
    }

    // Clean up memory for objects
    for (Object* object : objects) {
        delete object;
    }
    objects.clear();

    system("PAUSE");
}
