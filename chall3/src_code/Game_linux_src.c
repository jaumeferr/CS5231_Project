#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include<limits.h>
#include<signal.h>

///compilation command
/// gcc -m32 -fno-stack-protector -no-pie -g Game_linux_src.c -o game_linux

void GameOver();
void GameOn();
void Worthy();
void First_Trial();
void Second_Trial();
void Third_Trial();


void main()
{
	//signal(SIGINT,GameOver);
	char cont;
	system("clear");
	printf("Katara: \
			\nWater. Earth. Fire. Air. Long ago, the four nations lived together in harmony.\
			\nThen everything changed when the Fire Nation attacked. Only the Avatar, master of all four elements, \
			\ncould stop them, but when the world needed him most, he vanished. A hundred years passed and \
			\nmy brother Sokka and I discovered the new Avatar, an airbender named Aang. \
			\nAlthough his airbending skills were great, he had a lot to learn before he was ready to save anyone. \
			\nBut from the first day I met him, I believed Aang would save the world. And you know what? I was right. \
			\nWith the help of his friends, Aang defeated Fire Lord Ozai and ended the Hundred Year War. \
			\nZuko, Ozai's son and our ally, became the new Fire Lord. Together with Earth King Kuei, \
			\nAang and Zuko promised to return the nations to harmony.\
			\n");
	
	sleep(5); 
	
	printf("\nPress 'Enter' to contine...");
	
	cont = fgetc(stdin);
	if(cont == '\n')
	{
		GameOn();
	}
}


void GameOn()
{
	//signal(SIGINT,GameOver);
	system("clear");
	printf("At Uncle Iroh's Tea shop, The Jasmine Dragon...");
	
	char chr_name[20];
	char resp, cont;

	printf("\n\nType in your name...: ");
	scanf("%s", chr_name);
	
	printf("\nUncle Iroh: \
			\nHello my dear %s !!! As you know, The Order of the White Lotus is a very secretive society! \
			\nTo become a member of it, you need to prove your worth! \
			\nDo you think you are worthy and can prove it? (y/n)\n", chr_name);
			
	scanf(" %c", &resp);

	if(resp == 'y' || resp == 'Y')
	{
		system("clear");
		printf("Uncle Iroh: \
				\nWell then my child. Buckle up and pay attention to everything that is going to happen here on!!!\n");
		printf("\nIn order to prove your worth, you must pass three 'cryptic' trials. Are you ready?");
		printf("\nPress 'Enter' to continue");
			
		cont = fgetc(stdin);
		if(cont == '\n')
		{
			fflush(stdout);
			First_Trial();
		}
	}
	else
	{	
		system("clear");
		printf("Uncle Iroh: \
				\nSelf-awareness is the greatest awareness one can have!\
				\nNo worries! Come back to me when you feel ready!!");
		sleep(3);
		//exit(0);		
		GameOver();
	}
}


void First_Trial()
{
	//signal(SIGINT,GameOver);
	system("clear");
	int cnt = 0;
	char cont;
	char bring[7];
	char overwrite[10];
	char buf[30];
	printf("Uncle Iroh: \
			\nWelcome to the first trial!!! \
			\n* Sips tea * \
			\nHere is your task!\
			\n\n\"I am like the water that broke a 30 meter dam and flooded the nearby 17 villages.\" \
			\n\nNow... Become that *water* that brings 'deluge' with it at the *end*!\n");

	scanf("%s",buf);
	while(1)
	{	
		if(strcmp(bring,"deluge")!=0)
		{	
			cnt++;
			printf("\nUncle Iroh:\
				\nWrong, try again\n");
			if(cnt >= 3)
			{
				printf("Let me give you a hint. It's buffer overflow. Pay attention to the figures there.\n");
			}
			else if (cnt > 5 && cnt < 7)
			{
				printf("Let me give you another hint. Try appending 'deluge' to your input.\n");
			}
			else if (cnt > 7)
			{
				printf("Let me give you another hint. Give input of 40 characters + 'deluge'.\n");
			}
			scanf("%s",buf);
		}
		else
			break;
	}
	//sleep(5);
	if(strcmp(bring,"deluge")==0)
	{	
		system("clear");
		printf("Uncle Iroh:\
				\nWell Done!!! You have passed the first trial!.\
				\nMoving on to the next trial...");
		
		fflush(stdout);

		sleep(5);
		
		Second_Trial();
			
	}
}


void Second_Trial()
{
	//signal(SIGINT,GameOver);
	system("clear");
	char cont;
	unsigned int rice , value = 4294962065;
	int cnt = 0;
	printf("Uncle Iroh: \
			\nWelcome to the second trial!!! \
			\n* Sips more tea * \
			\nHere is a tale and a riddle.\
			\n\nYou would have heard about the limitless cornucopia. There is yet another such wonder.\
			\nHere... This 4 feet rice jar contains rice grains to the brim. Once you empty it completely,\
			\nand again dip in to fill a cup of rice... Magically, the entire jar fills itself fully.\
			\n\nNow tell me, after emptying completely, if you try to fill a cup with 5231 grains, how many grains of rice will be left in the jar?\n");

	scanf("%u", &rice);

	while(1)
	{	

		if( rice != value)
		{	
			cnt++;
			printf("\nUncle Iroh:\
				\nWrong, try again\n");
			if(cnt >= 3)
			{
				printf("Let me give you a hint. 'Wrap around'\n");
			}
			scanf("%u", &rice);
		}
		else
			break;
	}
	if(rice == value)
	{
		system("clear");
		printf("Uncle Iroh:\
				\nOh Well Done!!! Well Done!!! You have passed the second trial too!!!.\
				\nMoving on to the last and final trial...\
				\nYou will need the strength of a 'Lionturtle' for the next trial.");
	
		fflush(stdout);
		sleep(5);	
		
		Third_Trial();
	}
	
}


void Third_Trial()
{
	//signal(SIGINT,GameOver);
	system("clear");
	char cont;
	char secrettext[]="CS5231_F1n4l_Pr0j3ct";
	char getsecret[22];
	int cnt = 0;
	printf("Uncle Iroh: \
			\nWelcome to the final trial!!! \
			\nYou have performed admirably!!!\
			\nHere is the final riddle:\
			\n\nThings are never what you see. But if you are open to change, you can unlock the secrets of life.\
			\n\n * Hands you a map *\
			\nWhat is that you learn from this map?\n");

	scanf("%s", getsecret);
	
	while(1)
	{	
		if(strcmp(getsecret,secrettext)!=0)
		{	
			cnt++;
			printf("\nUncle Iroh:\
				\nWrong, Do you want to try again?\n");
			if(cnt == 3)
			{
				printf("Let me give you a hint. It is the art of hiding things inside other things\n");
			}
			else if (cnt > 3 && cnt < 5)
			{
				printf("Let me give you another hint. I already told you will be needing the strength of a 'Lionturtle'\n");
			}
			else if (cnt > 5)
			{
				printf("Let me give you another hint. You might want to analyse the metadata of the image\n");
			}
			scanf("%s",getsecret);
		}
		else
			break;
	}
	if(strcmp(getsecret,secrettext)==0)
	{
		system("clear");
		printf("Uncle Iroh:\
				\nWonderful!!! You have passed the third and final trial successfully!!!");
		
		fflush(stdout);
		sleep(3);
		
		Worthy();
	}
}


void GameOver()
{
	char yorn;
	system("clear");
	printf("Do you want to try again?(y/n)\n");
	scanf(" %c", &yorn);
	if(yorn == 'y' || yorn == 'Y')
		GameOn();
	else
		exit(0);
}


void Worthy()
{	
	system("clear");
	printf("Uncle Iroh:\
			\nMany Congratulations!!!\
			\nYou have proved your worth!!!\
			\nWelcome to the Order of the White lotus!!!");
	sleep(3);
	exit(0);
}
