#ifndef MONSTER_H
#define MONSTER_H
#include "Object.h"

class Player;
class Monster :
	public Object
{
public:
	Monster() {};
    Type getType() const override { return Type::slime; }
	Monster(const Player& player);

	void update(Player& player, std::vector<Monster>& monsters) override;
	
	int attack() const override;
	void defend(int damage) override;
	void print(std::ostream& o) const override;
private:
	int AC{ 0 };

};

#endif // !MONSTER_H