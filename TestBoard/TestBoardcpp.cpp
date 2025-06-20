#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h> // For Sleep() and Beep()

using namespace std;

// Utility: Wait a moment
void wait(int ms) {
    Sleep(ms);
}

// Utility: Sound effect (Windows only)
void playHitSound() {
    Beep(600, 100);
}

void playWinSound() {
    Beep(1000, 150);
    Beep(1200, 150);
    Beep(1500, 250);
}

void playLoseSound() {
    Beep(400, 200);
    Beep(300, 200);
    Beep(200, 300);
}

struct Character {
    string name;
    int hp;
    int attack;
    int defense;
    string personality;
};

string getHealthBar(int currentHP, int maxHP, int barWidth = 20) {
    int filledBars = (currentHP * barWidth) / maxHP;
    int emptyBars = barWidth - filledBars;

    string bar = "[";
    for (int i = 0; i < filledBars; ++i) bar += "#";
    for (int i = 0; i < emptyBars; ++i) bar += "-";
    bar += "]";

    return bar;
}

void showStats(const Character& player, const Character& enemy) {
    cout << "\n--- Current Stats ---\n";

    cout << player.name << " HP: " << player.hp << " / 200 ";
    cout << getHealthBar(player.hp, 200) << "\n";

    cout << enemy.name << " HP: " << enemy.hp << " / 200 ";
    cout << getHealthBar(enemy.hp, 200) << "\n\n";
}

int calculateDamage(int attack, int defense) {
    int damage = attack - (defense / 2);
    return (damage > 0) ? damage : 1; // Minimum 1 damage
}

void fightRound(Character& attacker, Character& defender) {
    int damage = calculateDamage(attacker.attack, defender.defense);
    defender.hp -= damage;
    cout << attacker.name << " attacks for " << damage << " damage!\n";
    playHitSound();
    wait(700);
}

void gameLoop(const string& playerName) {
    string personalities[] = { "Aggressive", "Sarcastic", "Stoic" };
    string chosenPersonality = personalities[rand() % 3];
    Character player = { playerName, 200, 20, 10 };
    Character enemy = {"Enemy", 200, 15, 8, chosenPersonality};
    int quickAttackCount = 0;          // For player
    int enemyQuickAttackCount = 0;     // For enemy

    cout << "\nA wild enemy appears! It seems to be.." << enemy.personality << ".\n";


    cout << "=== FIGHT STARTS! ===\n";
    showStats(player, enemy);

    while (player.hp > 0 && enemy.hp > 0) 
    {
        cout << "\nChoose your move:\n";
        cout << "1. Quick Attack\n";
        cout << "2. Heavy Attack ";
        if (quickAttackCount < 3) {
            cout << "(locked - need " << (3 - quickAttackCount) << " more quick attacks)\n";
        }
        else {
            cout << "(READY!)\n";
        }
        cout << "3. Defend\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: // Quick Attack
            player.attack = 15 + rand() % 6;  // 15–20
            fightRound(player, enemy);
            quickAttackCount++;
            break;

        case 2: // Heavy Attack
            if (quickAttackCount >= 3) {
                player.attack = 25 + rand() % 6;  // 25–30
                fightRound(player, enemy);
                quickAttackCount = 0; // Reset cooldown
            }
            else {
                cout << "⚠️  Heavy Attack is on cooldown. Use 3 quick attacks first.\n";
                continue; // Skip enemy turn
            }
            break;

        case 3: // Defend
            player.defense = 20;
            cout << player.name << " braces to defend.\n";
            break;

        default:
            cout << "Invalid move. You hesitate!\n";
            continue;
        }

        if (choice != 3)
            player.defense = 10;

        if (enemy.hp > 0) 
        {
            if (enemyQuickAttackCount >= 3) {
                enemy.attack = 25 + rand() % 11; // Heavy: 25–35
                cout << enemy.name << " unleashes a heavy attack!\n";
                fightRound(enemy, player);
                enemyQuickAttackCount = 0;
            }
            else 
            {
                enemy.attack = 15 + rand() % 6; // Quick: 15–20

                string phrase;
                if (enemy.personality == "Aggressive") {
                    string lines[] = {
                        enemy.name + " snarls: \"I'll crush you!\"",
                        enemy.name + " shouts: \"Take this!\"",
                        enemy.name + " yells: \"You're weak!\""
                    };
                    phrase = lines[rand() % 3];
                }
                else if (enemy.personality == "Sarcastic") {
                    string lines[] = {
                        enemy.name + " smirks: \"Oh no, did I hurt you?\"",
                        enemy.name + " chuckles: \"Try not to cry.\"",
                        enemy.name + " says: \"This is too easy.\""
                    };
                    phrase = lines[rand() % 3];
                }
                else { // Stoic
                    string lines[] = {
                        enemy.name + " strikes silently.",
                        enemy.name + " maintains a calm gaze.",
                        enemy.name + " attacks without expression."
                    };
                    phrase = lines[rand() % 3];
                }

                cout << phrase << endl;

                fightRound(enemy, player);
                enemyQuickAttackCount++;
            }
        }
        showStats(player, enemy);
    }

    if (player.hp <= 0) {
        cout << "\n💀 " << player.name << " was defeated...\n";
        playLoseSound();
    }
    else {
        cout << "\n🏆 " << player.name << " won the battle!\n";
        playWinSound();
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    char playAgain;
    string playerName;

    cout << "Enter your character's name: ";
    getline(cin, playerName);  // use getline for full names

    do {
        gameLoop(playerName);
        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
        cin.ignore();  // Flush leftover newline
    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing, " << playerName << "!\n";
    return 0;
}
