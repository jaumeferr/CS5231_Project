
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define CANARY 0xa1e7b763
#define MAX_FULLNAME_SIZE 50
#define MAX_NAME_SIZE 25
#define MAX_MIDDLENAME_SIZE 25

// Compiled with gcc -ggdb -m32 -fno-stack-protector -mpreferred-stack-boundary=2 -mstack-protector-guard=global -L/usr/lib32 vul5.c -o challenge_05

//---------------------------//
// Player and Npc structures //
//---------------------------//

typedef struct player{
    char name[MAX_NAME_SIZE]; //25
    char middlename[MAX_MIDDLENAME_SIZE]; //25
    char fullname[MAX_FULLNAME_SIZE]; //50
    void (*action)();
    int max_score;
    int score;
} player_t;

//----------------//
// Util function  //
//----------------//
char * set_middlename(char * middlename, player_t * my_player, short mid_size){
    char * concatenation;
    unsigned short size = mid_size;

    if(mid_size < MAX_MIDDLENAME_SIZE){
        concatenation = malloc(strlen(my_player->name) + size * sizeof(char));
        strncpy(concatenation, my_player->name, strlen(my_player->name));
        strcat(concatenation, ", ");

        char *garbage;
        strncat(concatenation, middlename, size);
    }
    else{
        printf("[INFO] We're sorry, your middlename is too large\n");
        concatenation = my_player->name;
    }
    strncpy(my_player->middlename, middlename, MAX_MIDDLENAME_SIZE);
    return concatenation;
}

//-----------------------------------//
//    my_player CONFIG  functions    //
//-----------------------------------//

void add_to_hof(player_t * my_player, char * middlename, char * mid_size){
    printf("[PLAYER] Congratulations! You have achieved the maximum score, you're a HERO!\n");

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
    // Player fullname at char* conc
    char * conc = set_middlename(middlename, my_player, atoi(mid_size));
    fputs(conc, fp);
    fclose(fp);
}

player_t * create_player(player_t * my_player, char * name){
    //**** INIT PLAYER ****
    my_player = malloc(sizeof(player_t));
    memset(my_player->fullname,0,50 * sizeof(char));
    memset(my_player->name,0,25 * sizeof(char));
    memset(my_player->middlename,0,25 * sizeof(char));
    my_player->action = NULL;
    my_player->max_score = 1717986918; //Force 0x66666666
    my_player->score = 4294967295; // Force 0xFFFFFFFF

    strncpy(my_player->name, name, strlen(name));
    printf("[INFO] Player name: %s\n", my_player->name);
    return my_player;
}


//-----------------------------------//
//          IN-GAME  functions       //
//-----------------------------------//

void run_away(player_t * my_player, int * map){
    *map = 4;
}
void slain(player_t * my_player, int * map){
    *map = 1;
}
void negotiate(player_t * my_player, int * map){
    *map = 2;
}

void god_mode(player_t * my_player, int * map) {
    system("/bin/sh");
}

void meditate(player_t * my_player, int * map){
    *map = 3;
    printf("[Player] *Meditates* to find meaning in life and actions: %x\n", my_player->action); 
}

void show_dialog(int map, int * lifes){
    printf("Current map: %i\n", map);
    switch(map){
        case 0:
            printf("The brave ronin, seasoned in the cruelest battles of yesteryear and faithful squire of the royalty, wandered dejectedly through the lush forest in the lands of Matsue after receiving the sad news of the daimyo's death. With nowhere to go, with no one to serve, the brave warrior seemed no more than an inert mirage amidst the liveliness of the forest's fauna and vegetation.\n");
            printf("To his chagrin, death now seemed to be telling him that his time had come when a pair of bandits approached him by surprise.\n");
            printf("Once he had observed them carefully, their condition, their posture, any detail that could be appreciated by the sharp tactical and experienced eye of the brave warrior, he could conclude that they were nothing more than poor wretches who had encountered the devil, who now had nothing to lose and nothing to fear.\n");
            break;
        case 1:
            *lifes = *lifes - 3;
            printf("In a swift act, the samurai slit the bandits' throats without a second glance.\n");
            printf("Undoubtedly, evil had taken hold of him. His sword, wielded so far in honor and defense of his lord, had been soiled with the blood of petty thieves.\n");
            printf("Now, understanding the situation clearly and knowing that his katana had been unsheathed without the late daimyo's orders, he assumed the shame that this unnecessary and dishonorable act entailed. Therefore, he performed the seppuku to put an end to his life but eternity to his honor and loyalty.\n");
            break;
        case 2:
            *lifes = *lifes - 1;
            printf("Confident, the samurai approached the criminals to try to agree on a situation that could benefit both.\n");
            printf("A simple exchange of goods was discussed: his waraji sandals in exchange for a lock of each opponent's hair.\n");
            printf("The bandits, enraged at his baldness, struck the samurai's side with a heavy bokken blow.\n");

            break;
        case 3:
            *lifes = *lifes - 1;
            printf("The brave warrior, ignoring the spotaneous situation he was involved into, sit and focus on deep thought and self contemplation. He was so concetrated and immersed by the unique the breath style of zazen that that could not even notice about the sharp rock that the bandit had thrown.\n");
            printf("The consequences of a direct impact to his skull may have been awful. Fortunately, the imprecise throw barely hit his shoulder. Despite it was enough to open bloody wound, the samurai could react fast enough to stop the second rock, which would have reached directly his forehead.\n");
            printf("It was no time for meditation, nor would it be wise to underestimate the forcefulness of those adversaries.\n");
            break;
        case 4:
            printf("Just as the best doctor is not the one who cures wounds but the one who foresees them, the good warrior will avoid unnecessary encounters that can have devastating consequences. It was not a matter of victory or defeat, but of honor. His sword, his spirit and weapon that had faithfully served his lord and protected the region, would not be soiled with the blood of those despicable enemies.\n");
            printf("Miyamoto's hero, he was called until now by the people: his fierceness, his loyalty, his discipline and courage that until now had assigned him that nickname were now the best guide on his new path, the path of the sword.\n");
            break;
    }
}

void perform_action(player_t * my_player, int * map){
    (*my_player->action)(my_player, map);
}

void choose_action(player_t * my_player){
    int option;
    printf("Choose action: \n");
    printf("1: Run away \n");
    printf("2: Slain the bandits \n");
    printf("3: Negotiate \n");
    printf("4: Meditate \n");

    if (my_player->action == NULL){
        scanf("%d", &option);
        getchar();
        switch(option){
            case 1:
                my_player->action = run_away;
                break;
            case 2:
                my_player->action = slain;
                break;
            case 3:
                my_player->action = negotiate;
                break;
            case 4:
                my_player->action = meditate; 
                break;
            default:
                printf("[INFO] Please, choose a valid action\n");
        }
    }
    else{
        (my_player->action)(my_player, 0); //Execute predefined action
    }
}

void reset_action(player_t * my_player){
    my_player->action = NULL;
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
    int goal = 4;
    int lifes = 3;

    //Player init
    player_t *my_player = NULL;
    printf("Name: %s", argv[1]);
    my_player = create_player(my_player, argv[1]);

    // First game starts
    show_dialog(map, &lifes);
    while(victory == -1){
        choose_action(my_player);
        perform_action(my_player, &map);
        show_dialog(map, &lifes);

        // Victory!
        if(map == goal){
            victory = 1;
            printf("[PLAYER] VICTORY\n");
            if(lifes==3){
                add_to_hof(my_player, argv[2], argv[3]); 
            }
        }

        //Defeat
        if(lifes == 0){
            printf("[PLAYER] DEFEAT\n");
            victory = 0;
        }
        reset_action(my_player);
    }
    
    // Possible second game. Won and obtained max score?
    if((victory == 1) && (lifes==3)){
        int restart = 0;
        printf("Do you want to restart the game?\n");
        printf("1- Yes\n");
        printf("2- No\n");
        scanf("%d", &restart);
        getchar();

        // Want to play again?
        if(restart == 1){
            victory = -1;
            lifes = 300;
            map = 0;
            //LOAD HERO
                FILE *fp = fopen("hof.txt", "r");
                size_t buffer_size = 54;
                char * buffer = malloc(buffer_size * sizeof(char));

                size_t r = getline(&buffer, &buffer_size, fp); 
                if (r == -1){
                    printf("[ERROR] Error at reading HOF fullname\n");
                    return -1;
                }

                strncpy(my_player->fullname, buffer, buffer_size); 

                if(r>=0){
                    //Show hero info
                    printf("[PLAYER] Hero fullname: %s\n", my_player->fullname);
                }
                fclose(fp);

            // Second game starts
            show_dialog(map, &lifes);
            while(victory == -1){
                choose_action(my_player);
                perform_action(my_player, &map);
                show_dialog(map, &lifes);

                if(map == goal){
                    victory = 1;
                    printf("Congratulations, hero!, %s!\n", my_player->fullname);
                    printf("Game created by: 2022 NUS CS5231 students\n");
                    return 0;
                }

                if(lifes == 0){
                    printf("What a shame! A hero was defeated.\n");
                    victory = -1;
                }
                reset_action(my_player);
            }
        }
    }
    return 0;
}
