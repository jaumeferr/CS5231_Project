
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define CANARY 0xa1e7b763
#define MAX_FULLNAME_SIZE 50
#define MAX_NAME_SIZE 25
#define MAX_MIDDLENAME_SIZE 25

// Compiled with gcc -ggdb -m32 -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul4.c -o challenge_04

//---------------------------//
// Player and Npc structures //
//---------------------------//

typedef struct player{
    void (*action)(); //Modify action function address by overflowing fullname field
    char fullname[MAX_FULLNAME_SIZE]; //To overflow -> 25(name)+ 2(", ") + 23(middle) + 4(func addr)
    char name[MAX_NAME_SIZE];
    char middlename[MAX_MIDDLENAME_SIZE];
    int max_score;
    int score;
    //char war_cry[10];
} player_t;

//----------------//
// Util function  //
//----------------//
char * set_middlename(char * middlename, player_t * my_player, unsigned long mid_size){
    //Integer overflow with mid_size is possible
    char * concatenation;
    if(mid_size < MAX_MIDDLENAME_SIZE){
        concatenation = malloc(strlen(my_player->name) + mid_size * sizeof(char));
        printf("[INFO] Name to be concatenated: %s -> size: %d\n", my_player->name, strlen(my_player->name));
        printf("[INFO] Middlename to be added: %s -> size: %d\n", middlename, strlen(middlename));

        strncpy(concatenation, my_player->name, strlen(my_player->name));
        strcat(concatenation, ", ");
        //DIFICULTY 2
        printf("[INFO] mid_size value is: %i", mid_size);
        strncat(concatenation, middlename, mid_size); 
        
        /*DIFICULTY 1
        printf("[INFO] mid_size value is: %i", mid_size);
        printf("[INFO] middlename length is actually: %i", strlen(middlename));
        strncat(concatenation, middlename, strlen(middlename)); 
        // res = strncat(concatenation, "\n", 1);*/
        printf("[INFO] Concatenation is now %s with size %d\n", concatenation, strlen(concatenation));
    }
    else{
        printf("[INFO] Sorry, your middlename is too large\n");
        concatenation = my_player->name;
    }
    return concatenation;
}

//-----------------------------------//
//    my_player CONFIG  functions    //
//-----------------------------------//

void add_to_hof(player_t * my_player, char * middlename, unsigned long mid_size){
    printf("Congratulations! You have achieved the maximum score, you're a HERO!\n");

    //Remove outdated hof file
    if (remove("hof.txt") == 0) {
        printf("[INFO] The hof file is deleted successfully.\n");
    } else {
        printf("[ERROR] The file is not deleted.\n");
    }

    FILE *fp = fopen("hof.txt", "w");
    if(!fp){
        perror("[ERROR] File opening failed\n");
    }
    //New player fullname at char* conc
    char * conc = set_middlename(middlename, my_player, mid_size); //Conc length can be > than fullname length if integer overflow is performed
    printf("Your hero new fullname: %s\n", conc);
    fputs(conc, fp);
    fclose(fp);
}

player_t * load_hero(player_t * my_player){
    //Show hall of fame data
    FILE *fp = fopen("hof.txt", "r");
    size_t buffer_size = 54;
    char * buffer = malloc(buffer_size * sizeof(char));

    size_t r = getline(&buffer, &buffer_size, fp); // with this operation, player->fullname can be overflowed if fullname length > 50 (max 54 chars)
    if (r == -1){
        printf("[INFO] Error while reading HOF fullname\n");
    } else{
        printf("[INFO] Hero name OK\n");
    }
    strncpy(my_player->fullname, buffer, buffer_size); //50(fullname) + 4(overflow to fill my_player->action field)

    if(r>=0){
        //Show hero info
        printf("Hero fullname: %s\n", my_player->fullname);
    }
    fclose(fp);
}

player_t * create_player(player_t * my_player, char * name){
    //**** INIT PLAYER ****
    my_player = malloc(sizeof(player_t));
    memset(my_player->fullname,0,50 * sizeof(char));
    memset(my_player->name,0,25 * sizeof(char));
    memset(my_player->middlename,0,25 * sizeof(char));
    my_player->action = NULL;
    my_player->max_score = 0;
    my_player->score = 0;
    printf("[INFO] Player struct initialized: %08x\n", my_player);

    //printf("[INFO] You have set a new name\n");
    strncpy(my_player->name, name, strlen(name));
    printf("[INFO] Player name: %s\n", my_player->name);
    return my_player;
}


//-----------------------------------//
//          IN-GAME  functions       //
//-----------------------------------//

void action_01(player_t * my_player, int * map){
    *map = 6;
    /*char buf[10];
    scanf("%s", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);*/
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}
void action_02(player_t * my_player, int * map){
    *map = 2;
    /*
    char buf[10];
    scanf("%s", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);*/
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}
void action_03(player_t * my_player, int * map){
    *map = 3;
    /*
    char buf[10];
    scanf("%s\n", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);*/
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}

void god_mode() {
        system("/bin/sh");
}

void action_04(player_t * my_player, int * map){
    *map = 4;
    /*char buf[10];
    fgets(buf, sizeof(buf), stdin);
    snprintf(my_player->war_cry, 10, buf);*/
    printf("[Player] Report situation: %x\n", my_player->action); 
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}

void show_dialog(int map){
    printf("Current map: %i\n", map);
    switch(map){
        case 0:
            printf("MAP 0 dialog\n");
            break;
        case 1:
            printf("MAP 1 dialog\n");
            break;
        case 2:
            printf("MAP 2 dialog\n");
            break;
        case 3:
            printf("MAP 3 dialog\n");
            break;
        case 4:
            printf("MAP 4 dialog\n"); 
            break;
        case 5:
            printf("MAP 5 dialog\n");
            break;
    }
}

void perform_action(player_t * my_player, int * map){
    printf("[INFO] The action is about to be performed: %08x\n", my_player->action);
    printf("[INFO] The action map is: %d\n", *map);
    (*my_player->action)(my_player, map);
}

void choose_action(player_t * my_player){
    int action;
    printf("Choose action: \n");
    printf("1: ACTION_01 \n");
    printf("2: ACTION_02 \n");
    printf("3: ACTION_03 \n");
    printf("4: ACTION_04 \n");

    if (my_player->action == NULL){
        scanf("%d", &action);
        getchar();
        switch(action){
            case 1:
                my_player->action = action_01;
                break;
            case 2:
                my_player->action = action_02;
                break;
            case 3:
                my_player->action = action_03;
                break;
            case 4:
                my_player->action = action_04; 
                break;
            default:
                printf("[INFO] DEFAULT ACTION: %d\n", action);
                printf("Please, choose a valid action\n");
        }
    }
    else{
        printf("[INFO] Action selected: %d\n", action);
        (my_player->action)(my_player, action); //Execute action
    }
}

void reset_action(player_t * my_player){
    my_player->action = NULL;
    printf("[INFO] Action has been reseted\n");
}

//-----------------------------------//
//            GAME MAIN              //
//-----------------------------------//
int main(int argc, char* argv[]) {
    //Game config init
    long int can = CANARY;
    int victory = -1;
    int mode = 2;
    int map = 0;
    int goal = 6;

    //Player init
    player_t *my_player = NULL;
    printf("Name: %s", argv[1]);
    my_player = create_player(my_player, argv[1]);

    while(victory == -1){
        show_dialog(map);
        choose_action(my_player);
        printf("[INFO] The chosen action is: %08x\n",my_player->action);
        perform_action(my_player, &map); //performs action and changes map number

        if(map == goal){
            victory = 1;
            printf("VICTORY\n");
            printf("BEFORE HOFF PPLAYER CURRENT player: %08x\n", my_player);
            char *garbage;
            unsigned long msize = strtoul(argv[3], &garbage, 10);
            add_to_hof(my_player, argv[2], msize); //Add middlename to player name and create fullname
            printf("HOFFF PPLAYER CURRENT player: %08x\n", my_player);
            reset_action(my_player);
        }

        if(map == -1){
            printf("DEFEAT\n");
            victory = 0;
            reset_action(my_player);
        }
    }

    printf("[INFO] Current player at address: %08x\n", my_player);

    if(victory == 1){
        int restart = 0;
        printf("Do you want to restart the game?\n");
        printf("1- Yes\n");
        printf("2- No\n");
        scanf("%d", &restart);
        getchar();

        if(restart == 1){
            victory = -1;
            map = 0;
            //LOAD HERO
                FILE *fp = fopen("hof.txt", "r");
                size_t buffer_size = 54;
                char * buffer = malloc(buffer_size * sizeof(char));

                size_t r = getline(&buffer, &buffer_size, fp); // with this operation, player->fullname can be overflowed if fullname length > 50 (max 54 chars)
                if (r == -1){
                    printf("[INFO] Error while reading HOF fullname\n");
                } else{
                    printf("[INFO] Hero name OK\n");
                }
                strncpy(my_player->fullname, buffer, buffer_size); //50(fullname) + 4(overflow to fill my_player->action field)

                if(r>=0){
                    //Show hero info
                    printf("[INFO] Hero fullname: %s\n", my_player->fullname);
                }
                fclose(fp);

            while(victory == -1){
                show_dialog(map);
                choose_action(my_player); //Default action is called is the action has been overflowed and the field is not NULL
                printf("[INFO] The chosen action is: %08x\n",my_player->action);
                perform_action(my_player, &map); //performs action and changes map number

                if(map == goal){
                    victory = 1;
                    printf("CONGRATULATIONS!, %s!\n", my_player->fullname);
                    printf("Game created by: 2022 NUS CS5231 students\n");
                    return 0;
                }

                if(map == -1){
                    printf("What a shame! A hero was defeated.\n");
                    victory = -1;
                    reset_action(my_player);
                    return 0;
                }
            }
        }
    }
    printf("[INFO] Exit");
    return 0;
}


/*void show_hof(){ // MODIFY GETLINE, CHECK FIELD MEANING BC ITS NOT CORRECT
    //Show hall of fame data
    FILE *fp = fopen("hof.txt", "r");
    char *line;
    size_t line_size;
    int r = 0;
    
    //Print hero fullname
    if (r = getline(line, &line_size, fp) != -1){
        printf("%s", line);
    } else{
        printf("[ERROR] Error showing hero name");
    }
    //Print hero max score
    if (r = getline(&line, &line_size, fp) != -1){
        printf("%s", line);
    } else{
        printf("[ERROR] Error showing hero score");
    }

    fclose(fp);
    if(r){
        free(r);
    }
}*/
