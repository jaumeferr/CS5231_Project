#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CANARY 0xa1e7b763

// Compiled with gcc -m32 -ggdb -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul.c -o challenge

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
    char concatenation[strlen(character->name) + strlen(middlename)];
    char * res;
    printf("middlename size is %ld\n", strlen(middlename));
    res = strncpy(concatenation, character->name, sizeof(character->name));
    res = strncat(concatenation, ", ", 2);
    res = strncat(concatenation, middlename, strlen(middlename));
    res = strncat(concatenation, "\n", 1);
    res = strncpy(character->fullname, concatenation, strlen(concatenation));
    printf("char->fullname size is now %ld\n", strlen(character->fullname));
}

//-----------------------------------//
//    CHARACTER CONFIG  functions    //
//-----------------------------------//

void show_hof(){ // MODIFY GETLINE, CHECK FIELD MEANING BC ITS NOT CORRECT
    //Show hall of fame data
    FILE *fp = fopen("hof.txt", "r");
    char line = "";
    size_t line_size;
    int r = 0;
    
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
        free(r);
    }
}

void add_to_hof(player_t * character){
    char m_name[30]; //Player's middlename size is actually 25
    size_t m_size = 30;

    printf("Congratulations! You have achieved the maximum score, you're a legend! \nPlease choose a new middlename so you will be remember as it:");
    
    //Check validity of new middlename
    int valid = -1;
    while(valid==-1){
        scanf("%s", m_name);
        getchar();
        if(sizeof(m_name)<=m_size){
            set_middlename(m_name, character);
            valid = 1;
            printf("A new middlename has been set.");
        } else{
            printf("We're sorry, the middlename you chose is not valid. Please choose a new middlename:");
        }
    }

    FILE *fp = fopen("hof.txt", "w");
    if(!fp){
        perror("File opening failed");
    }
    //Write player name
    fputs(character->fullname, fp);
    //Write player score
    char s_score[1];
    snprintf(s_score, "%d", character->score);
    fputs(s_score, fp);
    fclose(fp);
}

player_t * load_hero(player_t * character){
    character = malloc(sizeof(player_t));
    //Show hall of fame data
    FILE *fp = fopen("hof.txt", "r");
    int h_fullname_size;
    int h_maxscore_size;

    //Get fullname
    int r = getline(character->fullname, h_fullname_size, fp); // with this operation, player->fullname can be overflowed if fullname length > 50
    if (r == -1){
        printf("Error while reading HOF max_score\n");
    } else{
        printf("Hero fullname has been readed successfull\n");
    }

    //Get max score
    int max_score;
    r = getline(max_score, h_maxscore_size, fp);
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
        free(r);
    }
    return character;
}

player_t * create_player(player_t * character){
    //**** INIT PLAYER ****
    character = malloc(sizeof(player_t));
    character->action = NULL;
    character->max_score = 0;
    character->score = 0;

    //Set player name
    char *t_name;
    long int can = CANARY;
    printf("Set your player name: ");
    scanf("%s", t_name);
    if(strlen(t_name) > sizeof(character->name)){
        strncpy(character->name, t_name, sizeof(character->name));
    } else{
        strncpy(character->name, t_name, sizeof(t_name));
    }
    //With name set fullname
    strncpy(character->fullname, character->name, sizeof(character->name));
    return character;
}


//-----------------------------------//
//          IN-GAME  functions       //
//-----------------------------------//

void action_01(player_t * character, int * map){

}
void action_02(player_t * character, int * map){

}
void action_03(player_t * character, int * map){

}
void action_04(player_t * character, int * map){

}

void show_dialog(int map){

}

void perform_action(player_t * character, int * map){
    (*character->action)(character, map);
}

void choose_action(player_t * character){
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
                character->action = action_01;
            case 2:
                character->action = action_02;
            case 3:
                character->action = action_03;
            case 4:
                character->action = action_04; 
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
void init_game(player_t * character, int * victory){
    int map = 0;
    int goal = 6;

    while(*victory == -1){
        show_dialog(map);
        choose_action(character);
        perform_action(character, &map); //performs action and changes map number

        if(map == 6){
            *victory = 1;
        }
        reset_action(character);
        if(map == -1){
            *victory = 0;
        }
    }
}

int play(int mode, player_t * character){
    int victory=-1;
    if((mode > 0) && (mode<=3)){
        switch(mode){
            case 1:
                init_game(load_hero(character), &victory);
                break;
            case 2:
                init_game(create_player(character), &victory);
                break;
            case 3:
                victory = 0;
                break;
        }
    }
    return victory;
}

player_t *character = NULL;
int main(int argc, char* argv[]) {
    long int can = CANARY;
    int victory = -1;
    int mode = 2;

    victory = play(mode, character);

    if(victory == 1){
        printf("Do you want to restart the game?\n");
        printf("1- As a HERO\n");
        printf("2- As a new player\n");
        printf("3- [EXIT]\n");
        scanf("%d", &mode);
        play(mode, character);
    }
}
