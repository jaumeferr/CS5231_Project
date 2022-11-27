
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define CANARY 0xa1e7b763

// Compiled with gcc -ggdb -m32 -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul3.c -o challenge

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
    char war_cry[10];
} player_t;

//----------------//
// Util function  //
//----------------//
char * set_middlename(char * middlename, player_t * my_player){
    char * concatenation = malloc((strlen(my_player->name) + strlen(middlename)) * sizeof(char));
    char * res;
    printf("[INFO] Name to be concatenated: %s -> size: %d\n", my_player->name, strlen(my_player->name));
    printf("[INFO] Middlename to be added: %s -> size: %d\n", middlename, strlen(middlename));

    res = strncpy(concatenation, my_player->name, sizeof(my_player->name));
    res = strncat(concatenation, ", ", 2);
    res = strncat(concatenation, middlename, strlen(middlename));
    //res = strncat(concatenation, "\n", 1);
    printf("[INFO] Concatenation is now %s with size %d\n", concatenation, strlen(concatenation));
    return concatenation;
}

//-----------------------------------//
//    my_player CONFIG  functions    //
//-----------------------------------//

void show_hof(){ // MODIFY GETLINE, CHECK FIELD MEANING BC ITS NOT CORRECT
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
}

void add_to_hof(player_t * my_player){
    char * buffer = malloc(30 *sizeof(char));
    rewind(stdout);

    printf("Congratulations! You have achieved the maximum score, you're a legend!\n");
    printf("Please choose a new middlename\n");  
    scanf("%s",buffer);
    getchar();

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
    //Write player name
    char * fname = set_middlename(buffer, my_player);
    printf("A new middlename has been set: %s\n", my_player->middlename);
    printf("Your hero new fullname: %s\n", fname);
    fputs(fname, fp);
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
    strncpy(my_player->fullname, buffer, buffer_size);

    if(r>=0){
        //Show hero info
        printf("Hero fullname: %s\n", my_player->fullname);
    }

    fclose(fp);
    if(r){
        free(r);
    }
    return my_player;
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
    printf("[INFO] Player struct initialized\n");

    //printf("[INFO] You have set a new name\n");
    strncpy(my_player->name, name, strlen(my_player->name));
    //With name set fullname
    strncpy(my_player->fullname, name, strlen(my_player->name));
    printf("[INFO] Name chars have been copied\n");
    printf("[INFO] Your player name: %s\n", my_player->fullname);
    return my_player;
}


//-----------------------------------//
//          IN-GAME  functions       //
//-----------------------------------//

void action_01(player_t * my_player, int * map){
    *map = 6;
    char buf[10];
    scanf("%s", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}
void action_02(player_t * my_player, int * map){
    *map = -1;
    char buf[10];
    scanf("%s", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}
void action_03(player_t * my_player, int * map){
    *map = -1;
    char buf[10];
    scanf("%s\n", buf);
    snprintf(my_player->war_cry,10,buf);
    printf("%s\n", my_player->war_cry);
    printf("[INFO] Action %08x has been performed\n", my_player->action);
}

void god_mode() {
        system("/bin/sh");
}

void action_04(player_t * my_player, int * map){
    *map = -1;
    char buf[10];
    fgets(buf, sizeof(buf), stdin);
    snprintf(my_player->war_cry, 10, buf);
    printf("[Player]: %s\n", my_player->war_cry); 
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
        case 6: 
            printf("Victory!!\n");
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
    scanf("%d", &action);
    getchar();
    printf("[INFO] Action selected: %d\n", action);

    if (my_player->action == NULL){
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
        (my_player->action)(my_player, action); //Execute action
    }
}

void reset_action(player_t * my_player){
    my_player->action = NULL;
    printf("[INFO] Action has been reseted\n");
}

//-----------------------------------//
//         GAME MAIN FUNCTIONS       //
//-----------------------------------//
player_t * init_game(player_t * my_player, int * victory){
    int map = 0;
    int goal = 6;

    while(*victory == -1){
        show_dialog(map);
        choose_action(my_player);
        printf("[INFO] The chosen action is: %08x\n",my_player->action);
        perform_action(my_player, &map); //performs action and changes map number

        if(map == 6){
            *victory = 1;
            printf("VICTORY\n");
            printf("BEFORE HOFF PPLAYER CURRENT player: %08x\n", my_player);
            add_to_hof(my_player);
            printf("HOFFF PPLAYER CURRENT player: %08x\n", my_player);
        }
        if(map == -1){
            *victory = 0;
            reset_action(my_player);
            printf("DEFEAT\n");
        }
    }
    return my_player;
}

player_t * play(int mode, player_t * my_player, int * victory, char * name){
    printf("PLAY PLAYER CURRENT player: %08x\n", my_player);
    if((mode > 0) && (mode<=3)){
        switch(mode){
            case 1:
                printf("Play mode: HERO\n");
                my_player = init_game(load_hero(my_player), victory);
                break;
            case 2:
                printf("Play mode: NEWBIE\n");
                my_player = init_game(create_player(my_player, name), victory);
                break;
            case 3:
                *victory = 0;
                break;
        }
    }
    printf("return PLAYER CURRENT player: %08x\n", my_player);
    return my_player;
}

int main(int argc, char* argv[]) {
    long int can = CANARY;
    int victory = -1;
    int mode = 2;
    player_t * my_player = NULL;
    printf("My name is: %s", argv[1]);
    //my_player=
    my_player = play(mode, my_player, &victory, argv[1]);
    
    while(victory == 1){
        printf("Do you want to restart the game?\n");
        printf("1- As a HERO\n");
        printf("2- As a new player\n");
        printf("3- [EXIT]\n");
        scanf("%d", &mode);
        getchar();
        //printf("The last action was: %08x \n", my_player->action);
        my_player = play(mode, my_player, &victory, argv[1]);
        
    } 
}
