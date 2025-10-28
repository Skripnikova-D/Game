#include "PlayersHand.hpp"
#include "SpellDirectDamage.hpp"
#include "SpellSplash.hpp"
#include "SpellTrap.hpp"
#include "SpellSummon.hpp"
#include "SpellEnhancement.hpp"
#include <iostream>
#include <random>

PlayersHand::PlayersHand(){
    maxSize=5;
}

bool PlayersHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        std::cout << "Spell added to hand. Total: " << spells.size() << std::endl;
        return true;
    }
    std::cout << "Hand is full! Cannot add spell." << std::endl;
    return false;
}

Spell* PlayersHand::getSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

void PlayersHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
    }
}

int PlayersHand::getSpellCount() const {
    return spells.size();
}

bool PlayersHand::isFull() const {
    return spells.size() >= maxSize;
}

int PlayersHand::getMaxSize() const {
    return maxSize;
}

// 70% шанс получить случайное заклинание
void PlayersHand::AddRandomSpell() {
    if (isFull()) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> chance(0.0, 1.0);
    std::uniform_int_distribution<> spellType(0, 4);

    if (chance(gen) <= 0.7) {
        int type = spellType(gen);
        std::unique_ptr<Spell> newSpell;

        switch (type) {
            case 0:
                newSpell = std::make_unique<SpellDirectDamage>();
                std::cout << "Gained: Direct Damage Spell" << std::endl;
                break;
            case 1:
                newSpell = std::make_unique<SpellSplash>();
                std::cout << "Gained: Splash Spell" << std::endl;
                break;
            case 2:
                newSpell = std::make_unique<SpellTrap>();
                std::cout << "Gained: Trap Spell" << std::endl;
                break;
            case 3:
                newSpell = std::make_unique<SpellSummon>();
                std::cout << "Gained: Summon Spell" << std::endl;
                break;
            case 4:
                newSpell = std::make_unique<SpellEnhancement>();
                std::cout << "Gained: Enhancement Spell" << std::endl;
                break;
        }
        addSpell(std::move(newSpell));
    } else {
        std::cout << "No new spell this time." << std::endl;
    }
}