#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

#define TEAM_SIZE 2
#define MAX_CHARACTERS 6

typedef struct StateNode
{
    int effectType;
    int turnsLeft;
    struct StateNode *next;
} StateNode;

typedef struct
{
    char name[30];
    int damage;
    int effect;
} Attack;

typedef struct
{
    char name[30];
    char type[20];
    int health;
    int defense;
    Attack attacks[4];
    int lastAttackSpecial;
    int isDefending;
    StateNode *states;
} Character;

enum
{
    NOEFFECT = 0,
    SLEEP = 5,
    GUARD = 6,
    POISON = 7
};

Character characterPool[MAX_CHARACTERS] = {
    {"Eryon", "Psychic", 140, 10, {{"gaslight", 10, NOEFFECT}, {"nostalgia", 10, NOEFFECT}, {"narcolepsy", 50, SLEEP}, {"depression", 33, SLEEP}}, 0, 0, NULL},
    {"Lira", "Psychic", 130, 8, {{"gaslight", 10, NOEFFECT}, {"nostalgia", 10, NOEFFECT}, {"narcolepsy", 50, SLEEP}, {"depression", 33, SLEEP}}, 0, 0, NULL},
    {"Gorrak", "Brute Force", 160, 12, {{"pressure", 15, NOEFFECT}, {"crush", 20, NOEFFECT}, {"peak pressure", 70, GUARD}, {"super kick", 70, GUARD}}, 0, 0, NULL},
    {"Tharja", "Brute Force", 170, 10, {{"smash", 15, NOEFFECT}, {"trow", 15, NOEFFECT}, {"link attack", 70, GUARD}, {"broken link", 70, GUARD}}, 0, 0, NULL},
    {"Venomous", "Poison", 120, 5, {{"Bite", 10, NOEFFECT}, {"Snake Bite", 10, NOEFFECT}, {"venemous bite", 50, POISON}, {"Super bite", 50, POISON}}, 0, 0, NULL},
    {"Toxin", "Poison", 125, 7, {{"Punch", 12, NOEFFECT}, {"stamp", 8, NOEFFECT}, {"Poison cloud", 50, POISON}, {"poison smoke", 50, POISON}}, 0, 0, NULL}};

void applyState(Character *target, int effectType, int duration)
{
    StateNode *n = (StateNode *)malloc(sizeof(StateNode));
    n->effectType = effectType;
    n->turnsLeft = duration;
    n->next = target->states;
    target->states = n;
}

void processStates(Character *c, const char *label)
{
    StateNode *prev = NULL;
    StateNode *cur = c->states;
    while (cur)
    {
        if (cur->effectType == POISON)
        {
            c->health -= 5;
            if (c->health < 0)
                c->health = 0;
            printf("\n%s: %s suffers 5 damage from poison.\n", label, c->name);
        }
        cur->turnsLeft--;
        if (cur->turnsLeft <= 0)
        {
            StateNode *f = cur;
            if (!prev)
            {
                c->states = cur->next;
                cur = c->states;
            }
            else
            {
                prev->next = cur->next;
                cur = prev->next;
            }
            free(f);
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

int isAsleep(const Character *c)
{
    StateNode *tmp = c->states;
    while (tmp)
    {
        if (tmp->effectType == SLEEP)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

int guardIsBroken(const Character *c)
{
    StateNode *tmp = c->states;
    while (tmp)
    {
        if (tmp->effectType == GUARD)
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

void printCharacterStatus(const Character *c, const char *label)
{
    printf("%s: %s (%s) - HP:%d, DEF:%d", label, c->name, c->type, c->health, c->defense);
    if (c->isDefending)
        printf(" [DEFENDING]");
    StateNode *s = c->states;
    if (s)
    {
        printf(" [Effects:");
        while (s)
        {
            if (s->effectType == SLEEP)
                printf(" Sleep(%d)", s->turnsLeft);
            else if (s->effectType == GUARD)
                printf(" GuardBreak(%d)", s->turnsLeft);
            else if (s->effectType == POISON)
                printf(" Poison(%d)", s->turnsLeft);
            s = s->next;
        }
        printf("]");
    }
    printf("\n");
}

void showAllCharacters(Character *p, Character *a)
{
    printf("\n--- Your Team ---\n\n");
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        if (p[i].health > 0)
            printCharacterStatus(&p[i], "Player1");
        else
            printf("Player1: %s (%s) - DEAD\n", p[i].name, p[i].type);
    }
    printf("\n--- Enemy Team ---\n\n");
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        if (a[i].health > 0)
            printCharacterStatus(&a[i], "Player2");
        else
            printf("Player2: %s (%s) - DEAD\n", a[i].name, a[i].type);
    }
    printf("\n");
}

void describeEffect(int effect)
{
    if (effect == 0)
        printf(" (No effect)");
    else if (effect == 5)
        printf(" (Sleep: skip next turn, does 10 ignoring defense)");
    else if (effect == 6)
        printf(" (Guard Break: can't defend next turn)");
    else if (effect == 7)
        printf(" (Poison: 5 immediate dmg + 5 dmg/turn for 3 turns)");
}

double performAttack(Character *attacker, Character *defender, int index, const char *atkLabel, const char *defLabel)
{
    if (index >= 2 && attacker->lastAttackSpecial)
    {
        printf("\n%s: %s tried using another special attack in a row!\n\n", atkLabel, attacker->name);
        return 0;
    }
    Attack atk = attacker->attacks[index];
    int dmg = atk.damage;
    int eff = atk.effect;
    int finalDamage = 0;
    if (eff == POISON)
    {
        finalDamage = 5;
        applyState(defender, POISON, 3);
        printf("\n%s: %s poisons %s: %s!\n\n", atkLabel, attacker->name, defLabel, defender->name);
    }
    else if (eff == SLEEP)
    {
        finalDamage = 10;
        applyState(defender, SLEEP, 1);
        printf("\n%s: %s makes %s: %s fall asleep!\n\n", atkLabel, attacker->name, defLabel, defender->name);
    }
    else
    {
        finalDamage = dmg - defender->defense;
        if (finalDamage < 0)
            finalDamage = 0;
    }
    if (defender->isDefending)
    {
        finalDamage /= 2;
        defender->isDefending = 0;
    }
    defender->health -= finalDamage;
    if (defender->health < 0)
        defender->health = 0;
    if (index >= 2)
        attacker->lastAttackSpecial = 1;
    else
        attacker->lastAttackSpecial = 0;
    printf("\n%s: %s used %s and dealt %.2f damage!\n\n", atkLabel, attacker->name, atk.name, (double)finalDamage);
    return finalDamage;
}

int allDead(Character *t)
{
    int alive = 0;
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        if (t[i].health > 0)
            alive++;
    }
    return (alive == 0);
}

void selectCharacters(Character *team)
{
    int used[TEAM_SIZE];
    for (int i = 0; i < TEAM_SIZE; i++)
        used[i] = -1;
    printf("\nSelect your 2 characters:\n\n");
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        printf("%d. %s (%s) %s\n", i + 1, characterPool[i].name, characterPool[i].type, characterPool[i].type);
    }
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        int c, v = 0;
        while (!v)
        {
            printf("\nChoose character %d: ", i + 1);
            scanf("%d", &c);
            int idx = c - 1;
            if (idx < 0 || idx >= MAX_CHARACTERS)
            {
                printf("\nInvalid.\n");
                continue;
            }
            int d = 0;
            for (int j = 0; j < i; j++)
            {
                if (used[j] == idx)
                {
                    d = 1;
                    break;
                }
            }
            if (d)
                printf("\nYou already chose that.\n");
            else
            {
                used[i] = idx;
                team[i] = characterPool[idx];
                v = 1;
            }
        }
    }
}

void autoSelectCharacters(Character *team)
{
    printf("\nAI selecting characters...\n\n");
    int used[TEAM_SIZE];
    for (int i = 0; i < TEAM_SIZE; i++)
        used[i] = -1;
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        int idx, v = 0;
        while (!v)
        {
            idx = rand() % MAX_CHARACTERS;
            int d = 0;
            for (int j = 0; j < i; j++)
            {
                if (used[j] == idx)
                {
                    d = 1;
                    break;
                }
            }
            if (!d)
            {
                used[i] = idx;
                v = 1;
            }
        }
        team[i] = characterPool[idx];
        printf("AI chose %s!\n", team[i].name);
    }
}

int coinToss()
{
    int c;
    printf("\nCoin toss! Choose \n0=Heads, 1=Tails: ");

    scanf("%d", &c);
    int r = rand() % 2;
    if (r == c)
    {
        printf("\nYou won the toss! You go first.\n\n");
        return 1;
    }
    else
    {
        printf("\nYou lost the toss! The AI goes first.\n\n");
        return 0;
    }
}

int chooseStartingCharacter(Character *team)
{
    int c;
    printf("\nChoose your starting character:\n\n");
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        printf("%d. %s (HP:%d)\n", i + 1, team[i].name, team[i].health);
    }
    scanf("%d", &c);
    return c - 1;
}

int switchCharacter(Character *t, int curr)
{
    int c;
    printf("\nChoose a character to switch to:\n\n");
    for (int i = 0; i < TEAM_SIZE; i++)
    {
        if (t[i].health > 0)
            printf("%d. %s (HP:%d)\n", i + 1, t[i].name, t[i].health);
    }
    scanf("%d", &c);
    return c - 1;
}

void showMenu(const char *label)
{
    printf("Choose:\n1=Attack\n2=Defend\n3=Switch\n4=Show Info\n\n");
    printf("%s> ", label);
}

void combat(Character *pTeam, Character *aTeam)
{
    int coin = coinToss();
    int pIndex = chooseStartingCharacter(pTeam);
    int aIndex = 0;
    int turn = 1;
    getchar();
    while (1)
    {
        system(CLEAR_COMMAND);
        printf("\n===== Turn %d =====\n\n", turn);
        for (int i = 0; i < TEAM_SIZE; i++)
        {
            if (pTeam[i].health > 0)
                processStates(&pTeam[i], "Player1");
        }
        for (int i = 0; i < TEAM_SIZE; i++)
        {
            if (aTeam[i].health > 0)
                processStates(&aTeam[i], "Player2");
        }
        if (allDead(pTeam))
        {
            printf("\nAll your chars died! AI wins!\n\n");
            printf("\nhow did you manage to lose against an ai?\n");
            break;
        }
        if (allDead(aTeam))
        {
            printf("\nAll AI are dead! You win!\n\n");
            printf("\nI mean... its an ai after all\n\n");
            break;
        }
        if (pTeam[pIndex].health <= 0)
        {
            int f = -1;
            for (int i = 0; i < TEAM_SIZE; i++)
            {
                if (pTeam[i].health > 0)
                {
                    f = i;
                    break;
                }
            }
            if (f < 0)
            {
                printf("\nAll your chars died! AI wins!\n\n");
                break;
            }
            pIndex = f;
        }
        if (aTeam[aIndex].health <= 0)
        {
            int f = -1;
            for (int i = 0; i < TEAM_SIZE; i++)
            {
                if (aTeam[i].health > 0)
                {
                    f = i;
                    break;
                }
            }
            if (f < 0)
            {
                printf("\nAll AI are dead! You win!\n\n");
                break;
            }
            aIndex = f;
        }
        if (coin == 1)
        {
            printf("=== PLAYER TURN ===\n\n");
            if (isAsleep(&pTeam[pIndex]))
            {
                printf("\nPlayer1: %s is asleep!\n\n", pTeam[pIndex].name);
            }
            else
            {
            player_menu_1:
                printCharacterStatus(&pTeam[pIndex], "Player1");
                printCharacterStatus(&aTeam[aIndex], "Player2");
                showMenu("Player1");
                int action;
                scanf("%d", &action);
                if (action == 1)
                {
                    printf("\nAvailable Attacks:\n\n");
                    for (int i = 0; i < 4; i++)
                    {
                        Attack a = pTeam[pIndex].attacks[i];
                        printf("%d. %s (DMG:%d)", i + 1, a.name, a.damage);
                        describeEffect(a.effect);
                        printf("\n");
                    }
                    printf("\nPlayer1> Choose attack: ");
                    int c;
                    scanf("%d", &c);
                    performAttack(&pTeam[pIndex], &aTeam[aIndex], c - 1, "Player1", "Player2");
                }
                else if (action == 2)
                {
                    if (guardIsBroken(&pTeam[pIndex]))
                    {
                        printf("\nPlayer1: %s tried to defend but guard is broken!\n\n", pTeam[pIndex].name);
                    }
                    else
                    {
                        printf("\nPlayer1: %s is defending! (Damage halved)\n\n", pTeam[pIndex].name);
                        pTeam[pIndex].isDefending = 1;
                    }
                }
                else if (action == 3)
                {
                    int old = pIndex;
                    pIndex = switchCharacter(pTeam, pIndex);
                    printf("\nPlayer1: switched from %s to %s!\n\n", pTeam[old].name, pTeam[pIndex].name);
                }
                else if (action == 4)
                {
                    showAllCharacters(pTeam, aTeam);
                    goto player_menu_1;
                }
                else
                {
                    printf("\nInvalid.\n\n");
                    goto player_menu_1;
                }
            }
            if (allDead(aTeam))
            {
                printf("\nAll AI are dead! You win!\n\n");
                break;
            }
            printf("=== AI TURN ===\n\n");
            if (isAsleep(&aTeam[aIndex]))
            {
                printf("\nPlayer2: %s is asleep!\n\n", aTeam[aIndex].name);
            }
            else
            {
                int aiAction = rand() % 3;
                if (aiAction == 0)
                {
                    int c = rand() % 4;
                    if (c >= 2 && aTeam[aIndex].lastAttackSpecial)
                        c = 0;
                    performAttack(&aTeam[aIndex], &pTeam[pIndex], c, "Player2", "Player1");
                }
                else if (aiAction == 1)
                {
                    if (guardIsBroken(&aTeam[aIndex]))
                    {
                        printf("\nPlayer2: %s's guard is broken!\n\n", aTeam[aIndex].name);
                    }
                    else
                    {
                        printf("\nPlayer2: %s defends! (Damage halved)\n\n", aTeam[aIndex].name);
                        aTeam[aIndex].isDefending = 1;
                    }
                }
                else
                {
                    int old = aIndex;
                    for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (aTeam[i].health > 0 && i != aIndex)
                        {
                            aIndex = i;
                            printf("\nPlayer2: switched from %s to %s!\n\n", aTeam[old].name, aTeam[aIndex].name);
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            printf("=== AI TURN ===\n\n");
            if (isAsleep(&aTeam[aIndex]))
            {
                printf("\nPlayer2: %s is asleep!\n\n", aTeam[aIndex].name);
            }
            else
            {
                int aiAction = rand() % 3;
                if (aiAction == 0)
                {
                    int c = rand() % 4;
                    if (c >= 2 && aTeam[aIndex].lastAttackSpecial)
                        c = 0;
                    performAttack(&aTeam[aIndex], &pTeam[pIndex], c, "Player2", "Player1");
                }
                else if (aiAction == 1)
                {
                    if (guardIsBroken(&aTeam[aIndex]))
                    {
                        printf("\nPlayer2: %s's guard is broken!\n\n", aTeam[aIndex].name);
                    }
                    else
                    {
                        printf("\nPlayer2: %s defends! (Damage halved)\n\n", aTeam[aIndex].name);
                        aTeam[aIndex].isDefending = 1;
                    }
                }
                else
                {
                    int old = aIndex;
                    for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (aTeam[i].health > 0 && i != aIndex)
                        {
                            aIndex = i;
                            printf("\nPlayer2: switched from %s to %s!\n\n", aTeam[old].name, aTeam[aIndex].name);
                            break;
                        }
                    }
                }
            }
            printf("=== PLAYER TURN ===\n\n");
            if (isAsleep(&pTeam[pIndex]))
            {
                printf("\nPlayer1: %s is asleep!\n\n", pTeam[pIndex].name);
            }
            else
            {
            player_menu_2:
                printCharacterStatus(&pTeam[pIndex], "Player1");
                printCharacterStatus(&aTeam[aIndex], "Player2");
                showMenu("Player1");
                int action;
                scanf("%d", &action);
                if (action == 1)
                {
                    printf("\nAvailable Attacks:\n\n");
                    for (int i = 0; i < 4; i++)
                    {
                        Attack a = pTeam[pIndex].attacks[i];
                        printf("%d. %s (DMG:%d)", i + 1, a.name, a.damage);
                        describeEffect(a.effect);
                        printf("\n");
                    }
                    printf("\nPlayer1> Choose attack: ");
                    int c;
                    scanf("%d", &c);
                    performAttack(&pTeam[pIndex], &aTeam[aIndex], c - 1, "Player1", "Player2");
                }
                else if (action == 2)
                {
                    if (guardIsBroken(&pTeam[pIndex]))
                    {
                        printf("\nPlayer1: %s tried to defend but guard is broken!\n\n", pTeam[pIndex].name);
                    }
                    else
                    {
                        printf("\nPlayer1: %s is defending! (Damage halved)\n\n", pTeam[pIndex].name);
                        pTeam[pIndex].isDefending = 1;
                    }
                }
                else if (action == 3)
                {
                    int old = pIndex;
                    pIndex = switchCharacter(pTeam, pIndex);
                    printf("\nPlayer1: switched from %s to %s!\n\n", pTeam[old].name, pTeam[pIndex].name);
                }
                else if (action == 4)
                {
                    showAllCharacters(pTeam, aTeam);
                    goto player_menu_2;
                }
                else
                {
                    printf("\nInvalid.\n\n");
                    goto player_menu_2;
                }
            }
        }
        printf("\nPress Enter to continue to next turn...\n\n");
        getchar();
        getchar();
        turn++;
        if (allDead(pTeam))
        {
            printf("\nAll your chars died! AI wins!\n\n");
            break;
        }
        if (allDead(aTeam))
        {
            printf("\nAll AI are dead! You win!\n\n");
            break;
        }
    }
    printf("\nBattle has ended!\n\n");
}

int main()
{
    srand(time(NULL));
    Character playerTeam[TEAM_SIZE];
    Character aiTeam[TEAM_SIZE];
    selectCharacters(playerTeam);
    autoSelectCharacters(aiTeam);
    combat(playerTeam, aiTeam);
    return 0;
}
