#include <random>
#include <iostream>
#include "Player.h"
#include "Monster.h"

Player::Player() :Object(Object::Type::player, 0, 1, 0)
{
	//levelUp();
}

void Player::levelUp()
{
	level++;
	std::normal_distribution<double> randomHealth(20.0 + level * 5, 5.0);
	health += std::max(1, (int)randomHealth(engine));

	std::normal_distribution<double> randomStrength(3.0 + level, 1.0);
	strength += std::max(1, (int)randomStrength(engine));
	SP += std::max(1, (int)randomStrength(engine));

	//grab new item.
	std::uniform_int_distribution<int> randomItem(0, (int)Item::Type::numTypes - 1);
	std::normal_distribution<double> randomBonus((double)level, (double)level / 2);
	Item found{ (Item::Type)randomItem(engine), std::max(1, (int)randomBonus(engine)) };

	std::cout << "You found a " << found << "!!!!" << std::endl;
	if (
		auto haveOne{ inventory.find(found.getClassification()) };
		haveOne == inventory.end()
		|| inventory[found.getClassification()] < found
		)
	{
		std::cout << "You keep the shiny new toy!" << std::endl;
		inventory[found.getClassification()] = found;
	}
	else
	{
		std::cout << "You toss aside the ugly old thing!" << std::endl;
	}
}

void Player::update(Player& player, std::vector<Monster>& monsters)
{
	
	std::cout << "What do you do? (a)ttack (h)eal ";
	char command{ 'x' };
	std::cin >> command;
	switch (command)
	{
	case 'a':
	{
		int monsterNum{ 1 };
		if (monsters.size() > 1)
		{
			std::cout << "Which Monster: ";
			std::cin >> monsterNum;
		}
		if (monsterNum > 0 && monsterNum <= monsters.size())
		{

			monsters[monsterNum - 1].defend(player.attack());
		}
		break;
	}
	case 'h':
		player.heal();
		break;
	default:
		std::cout << "please enter a or h" << std::endl;
		break;
	}
}

int Player::getLevel() const
{
	return level;
}

int Player::getSP() const
{
	return SP;
}

int Player::attack() const
{
	if (auto sword{ inventory.find(Item::Type::sword) }; sword != inventory.end())
	{
		return damageDone(sword->second.getBonusValue());
	}
	else
	{
		return damageDone(0);
	}
}

std::map<Item::Type, Item> Player::getInventory() const
{
	return inventory;
}

void Player::defend(int damage)
{
	int AC{ 0 };
	if (auto armor{ inventory.find(Item::Type::armor) }; armor != inventory.end())
	{
		AC += armor->second;
	}
	if (auto shield{ inventory.find(Item::Type::shield) }; shield != inventory.end())
	{
		AC += shield->second;
	}
	damageTaken(damage, AC);
}

void Player::heal()
{
	Object::nameOnly = true;
	if (SP >= 2)
	{
		std::normal_distribution<double> randomHeal(strength*2.0, 20.0*(1.0/level));
		int  amountHealed{ std::max(1, (int)randomHeal(engine)) };

		SP -= 2;
		std::cout << *this << " is healed by " << amountHealed << "hp!" << std::endl;
		health += amountHealed;
	}
	else
	{
		std::cout << "Not enough SP!!!" << std::endl;
	}
}

void Player::print(std::ostream& o) const
{
	if (nameOnly) Object::print(o);
	else
	{
		o << "L:" << getLevel() << " ";
		Object::print(o);
		o << " h:" << getHealth() << ", SP:" << getSP() << std::endl;
		o << inventory;
	}
}

std::ostream& operator<<(std::ostream& o, const std::map<Item::Type, Item>& src)
{
	std::for_each(src.begin(), src.end(), [&](std::pair<Item::Type, Item> item)
		{
			o << "  " << item.second << std::endl;
		});
	return o;
}
