/**********************************************************************************************************************************************
*   FILENAME: chall2.c
*   AUTHOR: Quentin AUDINET
*   DESCRIPTION:
*       A second challenge for the CTF for the final project in the CS5231 course from NUS
*   COMPILATION:
*       gcc -m32 -ggdb -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 chall2.c -o chall2        
*   SETUP:
*       Please add a flag.txt file in the same folder with the flag value you want
*       You can perform the following commands so that you'll be in a more realistic environment
*        - sudo chown root:root flag.txt chall2 player_data.txt
*        - sudo chmod u+s chall2
*        - touch player_data.txt && sudo chmod 300 player_data.txt
**********************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define N_GAMES 2

// Player struct
typedef struct {
    int money;
    int games_played;
    char* history_file;
} player_t;

// Game data struct
typedef struct {
    char flag[20];
    player_t player;
    int games[2];
    int current;
    int difficulty;
} game_heap_data_t;

typedef struct {
    int game;
    int score;
} game_t;
//  First game proposed
int dice(int difficulty) {
	printf("Choose a number between 1 and %d\n\n>> ", difficulty * 6);
	int attempt;
	scanf("%d", &attempt);
	getchar();
	int result = rand() % (6 * difficulty) + 1;
	printf("\nThe dice rolls and...\nIt's a %d !\n", result);
	if(attempt == result) {
		printf("You won! But we have no more shell availaible...\n");
        return 1;
	} else {
		printf("Sorry, bad number... Try again\n");
        return 0; 
	}
}

//  Second game proposed
int more_or_less(int difficulty) {
    int max;
    if (difficulty > 8) {
        max = 0xffffffff;
    } else if (difficulty < 1) {
        max = 0xf;
    } else {
        max = 0xf << ((difficulty - 1) * 8);
    }
	printf("Find the number between %u and %u\n\n>> ", 0, max);
	uint secret = rand() % max;
	uint attempt;
	int nb_round = 1;
	do {
		scanf("%d", &attempt);
		getchar();


		if(attempt == secret) {
			printf("Congratz ! You won in %d attempts\n\n", nb_round);
            return nb_round;
        }
		else if (attempt < secret)
			printf("More\n>> ");
		else
			printf("Less\n>> ");
		nb_round++;
	}while (attempt != secret);
    return 0;
}

//  Choose a game
game_t play(int difficulty) {
    int correct = 0;
    int choice = -1;
    game_t game_data;
    do {
    puts("Choose a game:\n\t1 - Roll the dice\n\t2 - More or less\n\t3 - Quit\n\n>> ");
    scanf("%d", &choice);
	getchar();

    } while(choice < 1 || choice > 3);

    game_data.game = choice - 1;

    switch (choice) {
    case 1:
        game_data.score = dice(difficulty);
        break;
    case 2:
        game_data.score = more_or_less(difficulty);
        break;
    default:
        game_data.score = 0;
    }
    return game_data;
}

int main(int argc, char* argv[]) {

    game_t game_info = {0,0};

    // All data are stored on the heap to avoid corruption in case of overflow

    game_heap_data_t* game_data = malloc(sizeof(game_heap_data_t));

    //  Get the secret
    char* flag =  game_data->flag;
    fscanf(fopen("flag.txt", "r"), "%s", flag);

    printf("Welcome player ! A secret is hidden at %p but there's no way you can find it ahah !\n\nWhat is your name ?: ", flag);
    
    //  Get a pointer to the player
    player_t* player = &(game_data->player);

    //  Initialize it
    player->money = 50;
    player->games_played = 0;
    player->history_file = "player_data.txt";

    //  Will contain history of played games
    FILE* history = NULL;

    //  Get a pointer to usefull data 
    int* games = (int*)&(game_data->games);
    int* current = &(game_data->current);
    int* difficulty = &(game_data->difficulty);

    //  Player name
    char player_name[24];

    fgets(player_name, sizeof(int) * 12, stdin);
    fflush(stdin);

    history = fopen(player->history_file, "w");

    //  Select the difficulty
    puts("\n\nChoose a difficulty: ");
    scanf("%d", difficulty);

    while(player->money > 0) {
        //  Let's play a game
        game_info = play(*difficulty);
        //  The user wants to leave
        if(game_info.game == 2)
            break;
        player->money --;
        game_data->games[game_info.game] = game_info.score;
        game_data->current = game_info.game;
        fprintf(history, "%d;", games[*current]);
        player->games_played++;
    }

    free(game_data);
    fclose(history);

    return 0;
}