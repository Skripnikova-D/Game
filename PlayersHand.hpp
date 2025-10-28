#pragma once
#include "Spell.hpp"
#include <vector>
#include <memory>

class PlayersHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;

public:
    PlayersHand();

    bool addSpell(std::unique_ptr<Spell> spell);
    Spell* getSpell(int index);
    void removeSpell(int index);
    int getSpellCount() const;
    bool isFull() const;
    int getMaxSize() const;

    void AddRandomSpell();//шанс на получение заклинания 70%
};
