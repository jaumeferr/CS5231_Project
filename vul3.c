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

struct player{
	void (*action)(); //Modify action function address by overflowing fullname field
	char fullname[50]; //To overflow -> 25 + 25 + 4
    char name[25];
    char middlename[25];
    int max_score;
    int score;
}

//----------------//
// Admin function //
//----------------//

void god_mode() {
	system("/bin/sh");
}

//----------------//
// Util function  //
//----------------//
void set_middlename(char* middlename, struct player){
    char *concatenation[strlen(player->name) + strlen(middlename)];
    strncpy(concatenation, player->name, sizeof(player->name));
    strncat(concatenation, ", ", 2);
    strncat(concatenation, middlename, strlen(player->middlename));
    strncat(concatenation, "\n", 1);
    strncpy(player->fullname, concatenation, (strlen(player->fullname)));
}

//-------------------------//
//    In game functions    //
//-------------------------//

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

void add_to_hof(struct player me){
    char m_name[30]; //Player's middlename size is actually 25
    size_t m_size = 30;

    printf("Congratulations! You have achieved the maximum score, you're a legend! \nPlease choose a new middlename so you will be remember as it:");
    
    //Check validity of new middlename
    bool valid = false
    while(!valid){
        do{
		    scanf("%d", &m_name);
		    getchar();
            if(sizeof(m_name)<=m_size){
                set_middlename(m_name, me)
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
    fputs(me->fullname, fp);
    //Write player score
    char *s_score[1];
    snprintf(s_score, "%d", me->score);
    fputs(s_score, fp);
    fclose(fp);
}

void play(int mode){
    struct player me;
    if((mode > 0) && (mode<=3)){
        switch(mode){
            case 1:
                init_game(load_hero());
                break;
            case 2:
                init_game(create_player());
                break;
            case 3:
                exit();
                break;
        }
    }
}

struct player load_hero(){
    struct player hero;

    //Show hall of fame data
    FILE *fp = fopen('hof.txt', r);
    int h_fullname_size = NULL;
    int h_maxscore_size = NULL;

    //Get fullname
    r = getline(&hero->fullname, *p_fullname_size, fp); // with this operation, player->fullname can be overflowed if fullname length > 50
    if (r == -1){
        printf("Error while reading HOF max_score\n");
    } else{
        printf("Hero fullname has been readed successfull\n");
    }

    //Get max score
    r = getline(&hero->max_score, *h_maxscore_size, fp);
    printf(me)
    if (r == -1){
        printf("Error while reading HOF max_score\n");
    } else{
        printf("Hero max_score has been readed successfully\n");
    }

    if(h_fullname_size && h_maxscore_size){
        //Show hero info
        printf("Hero fullname: %s\n", hero->fullname);
        printf("Hero score: %d\n", hero->score);
    }

    fclose(fp);
    if(r){
        free(r)
    }
    return hero;
}

struct player create_player(){
    //**** TO FILL ****
    struct player me = {
        void (*action)() = NULL;
        max_score = 0;
        score = 0;
    }
    //Set player name
    char *t_name;
    printf("Set your player name: ");
    scanf("%s", &t_name);
    if(strlen(t_name) > sizeof(me->name)){
        strncpy(me->name, t_name, sizeof(me->name));
    } else{
        strncpy(me->name, t_name, sizeof(t_name));
    }
    //With name set fullname
    strncpy(me->fullname, me->name, sizeof(me->name));

    return me;
}

void init_game(struct player me){
    //**** TO FILL ****
}

void next_action(struct player me){
    //3 - random action

    if (player->action == NULL){
        //choose action
            //1 - something
            //2 - something
    }
    else{
        //do defined action
    }
}

void reset_action(struct player me){
    player->action = NULL;
}

int main(int argc, char* argv[]) {
    size_t size_of_name = 25;
    long int can = CANARY;
    bool victory = false;
    int mode = NULL;

    victory = play(2);

    if(victory){
        fprintf("Do you want to restart the game?\n");
        fprintf("1- As a HERO\n");
        fprintf("2- As a new player\n");
        fprintf("3- [EXIT]\n");
        scanf("%d", &mode);
        replay(mode);
    }
}
