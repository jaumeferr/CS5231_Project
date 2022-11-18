#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CANARY 0xa1e7b763

// Compiled with gcc -m32 -ggdb -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul.c -o challenge
/*  do {
		scanf("%d", &attempt);
		getchar();*/

//---------------------------//
// Player and Npc structures //
//---------------------------//

typedef struct player{
	void (*action)(); //Modify action function address by overflowing fullname field
	char fullname[50]; //To overflow -> 25(name)+ 2(", ") + 23(middle) + 4(func addr)
    char name[25];
    char middlename[25];
    int max_score;
    int score;
} player_t;

//----------------//
// Admin function //
//----------------//

void god_mode() {
	system("/bin/sh");
}

//----------------//
// Util function  //
//----------------//
void set_middlename(char * middlename, player_t * character){
    char * concatenation[strlen(character->name) + strlen(middlename)];
    print("middlename size is %d\n", strlen(middlename));
    strncpy(concatenation, character->name, sizeof(character->name));
    strncat(concatenation, ", ", 2);
    strncat(concatenation, middlename, strlen(middlename));
    strncat(concatenation, "\n", 1);
    strncpy(character->fullname, concatenation, strlen(contatenation));
    print("char->fullname size is now %d\n", strlen(character->fullname));
}

//-----------------------------------//
//    CHARACTER CONFIG  functions    //
//-----------------------------------//

void show_hof(){
    //Show hall of fame data
    FILE *fp = fopen('hof.txt', r);
    char *line = "";
    int line_size = NULL;
    
    //Print hero fullname
    if (r = getline(&line, &line_size, fp) != -1){
        printf("%s", line);
    } else{
        printf("Error showing hero name");
    }
    //Print hero max score
    if (r = getline(&line, &line_size, fp) != -1){
        printf("%s", line);
    } else{
        printf("Error showing hero score");
    }

    fclose(fp);
    if(r){
        free(r)
    }
}

void add_to_hof(player_t * character){
    char * m_name; //Player's middlename size is actually 25
    size_t m_size = 30;

    printf("Congratulations! You have achieved the maximum score, you're a legend! \nPlease choose a new middlename so you will be remember as it:");
    
    //Check validity of new middlename
    bool valid = false
    while(!valid){
        do{
		    scanf("%d", &m_name);
		    getchar();
            if(sizeof(m_name)<=m_size){
                set_middlename(m_name, character)
                valid = true;
                printf("A new middlename has been set.", nb_round);
            } else{
                printf("We're sorry, the middlename you chose is not valid. Please choose a new middlename:");
            }
        }
    }
    FILE *fp = fopen("hof.txt", "w");
    if(!fp){
        perror("File opening failed");
        return EXIT_FAILURE;
    }
    //Write player name
    fputs(character->fullname, fp);
    //Write player score
    char *s_score[1];
    snprintf(s_score, "%d", character->score);
    fputs(s_score, fp);
    fclose(fp);
}

void load_hero(player_t * character){
    character = malloc(sizeof(player_t))
    //Show hall of fame data
    FILE *fp = fopen('hof.txt', r);
    int h_fullname_size = NULL;
    int h_maxscore_size = NULL;

    //Get fullname
    r = getline(character->fullname, &p_fullname_size, fp); // with this operation, player->fullname can be overflowed if fullname length > 50
    if (r == -1){
        printf("Error while reading HOF max_score\n");
    } else{
        printf("Hero fullname has been readed successfull\n");
    }

    //Get max score
    int max_score;
    r = getline(max_score, &h_maxscore_size, fp);
    character->max_score = max_score;
    if (r == -1){
        printf("Error while reading HOF max_score\n");
    } else{
        printf("Hero max_score has been readed successfully\n");
    }

    if(h_fullname_size && h_maxscore_size){
        //Show hero info
        printf("Hero fullname: %s\n", character->fullname);
        printf("Hero score: %d\n", character->score);
    }

    fclose(fp);
    if(r){
        free(r)
    }
}

void create_player(player_t * character){
    //**** INIT PLAYER ****
    character = malloc(sizeof(player_t));
    character->(*action)() = NULL;
    character->max_score = 0;
    character->score = 0;

    //Set player name
    char *t_name;
    printf("Set your player name: ");
    scanf("%s", &t_name);
    if(strlen(t_name) > sizeof(me->name)){
        strncpy(character->name, t_name, sizeof(character->name));
    } else{
        strncpy(character->name, t_name, sizeof(t_name));
    }
    //With name set fullname
    strncpy(character->fullname, character->name, sizeof(character->name));
}


//-----------------------------------//
//          IN-GAME  functions       //
//-----------------------------------//

void action_01(player_t * character, int * map){
    return 0;
}
void action_02(player_t * character, int * map){
    return 0;
}
void action_03(player_t * character, int * map){
    return 0;
}
void action_04(player_t * character, int * map){
    return 0;
}

void perform_action(player_t character, int action, int * map){
    (*character->action)(character, map);
}

void next_action(player_t * character){
    int action;
    printf("Choose action: \n");
    printf("1: ACTION_01 \n");
    printf("2: ACTION_02 \n");
    printf("3: ACTION_03 \n");
    printf("4: Let the gods choose my destiny.\n");
    scanf("%d", &action);
    if (character->action == NULL){
        switch(action){
            case 1:
                character->action = action_01; //return actions
            case 2:
                *character->action = action_02; //return actions
            case 3:
                character->action = action_03; //return actions
            case 4:
                character->action = action_04; //return actions
            default:
                printf("Please, choose a valid action");
        }
    }
    else{
        (character->action)(character, action); //Execute action
    }
}

void reset_action(player_t * character){
    character->action = NULL;
}

//-----------------------------------//
//         GAME MAIN FUNCTIONS       //
//-----------------------------------//
int init_game(player_t character, int * victory){
    int map = 0;
    int goal = 6;

    while(victory != 1){
        switch(map):
            case 0:
                show_dialog_01();
                next_action(character);
                perform_action(character, action, &map); //performs action and changes map number
            case 1:
                show_dialog_02();
            case 2:
                show_dialog_03();
            case 3:
                show_dialog_04();
            case 4:
                show_dialog_05();
            case 5:
                show_dialog_06();
        
        //If you win the game, the action is not reseted.
        if(map == 6){
            return 1;
        }

        reset_action(character);
        if(map == -1){
            return -1;
        }
    }
}

int play(int mode, player_t * character){
    int victory;
    if((mode > 0) && (mode<=3)){
        switch(mode){
            case 1:
                victory = init_game(load_hero(character), &victory);
                break;
            case 2:
                victory = init_game(create_player(character), &victory);
                break;
            case 3:
                victory = -1;
                break;
        }
    }
    return victory;
}

player_t *character = NULL;
int main(int argc, char* argv[]) {
    size_t size_of_name = 25;
    long int can = CANARY;
    bool victory = false;
    int mode = NULL;

    victory = play(2, character);

    if(victory != -1){
        fprintf("Do you want to restart the game?\n");
        fprintf("1- As a HERO\n");
        fprintf("2- As a new player\n");
        fprintf("3- [EXIT]\n");
        scanf("%d", &mode);
        play(mode, character);
    }
}
