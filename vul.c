#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USAGE_LENGTH 32
#define LAUNCH_LENGTH 40
#define CANARY 0xb4db16d

// Compiled with gcc -m32 -ggdb -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul.c -o challenge

//-------------------//
// Enum for the game //
//-------------------//

enum ROLES {DEAD=0, FARMER=1, EXPLORER=2};
enum GAMES {DICE=1, MORE_OR_LESS=2};


//---------------------------//
// Player and Npc structures //
//---------------------------//

typedef struct player {
	void (*action)();
	char name[20];
	int role;
} player_t;

typedef struct npc {
	int proposed_game;
	char name[24];
} npc_t;

//----------------//
// Admin function //
//----------------//

void god_mode() {
	system("/bin/sh");
}

//--------------------//
// Characters actions //
//--------------------//

void say_hello(player_t* player) {
	printf("Hello, I am %s\n", player->name);
}

void find_my_way(player_t* player) {
	printf("You're located at %p\n", player->action);
}

//------------------------------//
// Initialize the player struct //
//------------------------------//

void init_player(player_t * player) {
	player->action = say_hello;
	strncpy(player->name, "Give me a name", 20);
	player->role = FARMER;

}

void give_role(player_t * player) {
	switch (player->role) {
	case FARMER:
		player->action = say_hello;
		break;
	case EXPLORER:
		player->action = find_my_way;
		break;
	default:
		player->action = say_hello;
	}
}

void give_name(player_t * player, char* name, size_t max_size) {
	for(int i=0; i < max_size; i++) {
		player->name[i] = name[i];
		if(name[i] == '\0')	// Stop when we reach the null byte
			break;
	}
}

//-----------------------//
// Get a char from stdin //
//-----------------------//

char getChar() {
    char nl;
    char ret = getc(stdin);
    nl = getc(stdin);
    if (nl != '\n')
    {
        printf("\nPlease enter a single digit\n\n");
		exit(0);
    }
    return ret;		fflush(stdin);

}

//-----------------//
// Menu of actions //
//-----------------//

int menu() {
	int choice = 0;
	char answer;

	while(choice < 1 || choice > 8) {

		printf("What do you want to do ?\n");
		printf("1.\tCreate a new farmer\n");
		printf("2.\tDo character action\n");
		printf("3.\tInvoke a npc\n");
		printf("4.\tPlay npc game\n");
		printf("5.\tChange npc game\n");
		printf("6.\tKill myself\n");
		printf("7.\tKill the npc\n");
		printf("8.\tLeave\n\n");

		printf("Your choice: ");

		answer = getChar();

		if(answer == '\0')
			continue;

		if ((int)answer >= 0x31 && (int)answer <= 0x38) {
			choice = ((int)answer) - 0x30;
		} else {
			printf("\nPlease enter a number between 1 and 8\n\n");
		}
	}
	return choice;
}

//-----------//
// npc Games //
//-----------//

void dice() {
	printf("Choose a number between 1 and 6\n\n>> ");
	int attempt;
	scanf("%d", &attempt);
	getchar();
	int result = rand() % 6 + 1;
	printf("\nThe dice rolls and...\nIt's a %d !\n", result);
	if(attempt == result) {
		printf("You won! But we have no more shell availaible...\n");
	} else {
		printf("Sorry, bad number... Try again\n");
	}
	return;
}

void more_or_less() {
	printf("Find the number between %u and %u\n\n>> ", 0, 0xffffffff);
	uint secret = rand();
	uint attempt;
	int nb_round = 1;
	do {
		scanf("%d", &attempt);
		getchar();
		if(attempt == secret)
			printf("Congratz ! You won in %d attempts\n\n", nb_round);
		else if (attempt < secret)
			printf("More\n>> ");
		else
			printf("Less\n>> ");
		nb_round++;
	}while (attempt != secret);
	return;
}

//-----------------//
// Change npc Game //
//-----------------//

void change_npc_game(npc_t* npc) {
	printf("Choose a game number (1-2):\n");
	printf("1.\tdice\n");
	printf("2.\tmore or less\n");
	scanf("%d", &npc->proposed_game);
	getchar();
	printf("You've chosen game n°%d !", npc->proposed_game);
	return;
}

//-------------------------//
// Reference to the player //
//-------------------------//

player_t *player = NULL;
npc_t *npc = NULL;

int main(int argc, char* argv[]) {
	size_t name_size = 20;							// We store the maximum length for the name
	long int canary = CANARY;						// We use a canary to prevent buffer overflow


	// Some strings for debug info
	char launch[LAUNCH_LENGTH] = "Program ";
	char usage[USAGE_LENGTH];

	// Ensure there are at least to arguments
	if(argc < 2) {
		snprintf(usage, USAGE_LENGTH, "usage %s name [-D]\n", argv[0]);
		printf(usage);
		return 1;
	}
	//	Add a debug mode to be more verbose
	else if(argc == 3 && strcmp(argv[2], "-D") == 0) {
		snprintf(launch+8, LAUNCH_LENGTH, "%s started successfully!", argv[0]);
		printf("[*] %s\n", launch);
	}

	if(canary != CANARY) {
		printf("The canary is died !!! Abort ! Abort !\n");
		exit(1);
	}

	int choice = 0;
	while(choice != 8) {
		choice = menu();

		switch (choice) {

			// Create a new farmer
			case 1:
				player = malloc(sizeof(player_t));		// Allocate memory
				init_player(player);
				give_name(player, argv[1], name_size);
				give_role(player);
				printf("A new farmer just arrived !\n");
				break;

			// Perform action 
			case 2:
				if (player != NULL && player->role != DEAD) {
					(*player->action)(player);
				} else {
					printf("\nYou don't have any player...\n");
				}
				break;

			// Create a new npc
			case 3:
				if(npc == NULL) {
					npc = malloc(sizeof(npc_t));
					npc->proposed_game = DICE;
					snprintf(npc->name, 24, "npc-%d", rand());

					printf("\n\n%s just arrived to play the dice with you !\n", npc->name);
				} else {
					printf("You already have a npc with whom play\n");
				}
				break;

			// Play npc game
			case 4:
				if(npc == NULL) {
					printf("There is no npc with you...\n");
				} else {
					switch (npc->proposed_game) {
						case DICE:
							dice();
							break;
						case MORE_OR_LESS:
							more_or_less();
							break;
						default:
							printf("Game not found...");
							break;
					}
				}
				break;

			// Change npc game
			case 5:
				if (npc == NULL) {
					printf("There is no npc with you...\n");
				} else {
					change_npc_game(npc);
				}
				break;

			// Kill your character
			case 6:
				printf("This life is too hard for... Bye bye %s\n", player->name);
				memset(player, 0, sizeof(player_t));
				free(player);
				player->role = DEAD;
				break;

			// Kill the npc
			case 7:
				printf("This npc is so boring... Let's get rid of him, and... No more %s !", npc->name);
				memset(npc, 0, sizeof(npc_t));
				free(npc);
				npc = NULL;
				break;

			default:
			break;
		}
	}

	return 0;
}
