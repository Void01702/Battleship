#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<errno.h>
#include"c_a_chier/init.c"
#include"c_a_chier/map.c"

#define DIM 10
#define DEF_BOAT 20
#define DEF_BOAT_TYPE 8

typedef struct{
	int map[DIM][DIM];
	int boat_table[DEF_BOAT];
} Player;

typedef struct{
	char Name[15];
	int Score;
	int Encryption;
	int Seed;
} HighScore;

int max_(int a, int b){ //take the greater of two
	if(a>b){
		return a;
	}
	return b;
}

bool is_winner(int boat_table[DEF_BOAT]){ //test if there are boats alive left
	for(int i=0; i<DEF_BOAT; i++){
		if(boat_table[i]>0){
			return false;
		}
	}
	return true;
}

int turn(int map[DIM][DIM], int boat_table[DEF_BOAT]){ //the basic turn which return a value depending on the result
	int abs;
	int ord;
	char txt;
	int x=0;
	do{
		printf("On what square do you want to shoot?\n");
		fflush(stdin);
		do{
			scanf("%c", &txt);
		}while(txt<'A' || txt>'J');
		abs = txt-'A';
		//printf("%d\n", abs);
		do{
			scanf("%d", &ord);
		}while(ord<1 || ord>10);
		ord--;
		//printf("%d\n", abs);
		if(map[abs][ord]<0){
			printf("You already shot there, choose another spot.\n\n");
		}
		else{
			x=1;
		}
	}while(x==0);
	if(map[abs][ord]==0){
		map[abs][ord]=-1;
		return 0;
	}
	else if(map[abs][ord]>0){
		boat_table[map[abs][ord]-1]--;
		//printf("Boat: %d\nHP: %d\n", map[abs][ord], boat_table[map[abs][ord]-1]);
		if(boat_table[map[abs][ord]-1]<1){
			map[abs][ord]=-map[abs][ord]-1;
			if(is_winner(boat_table)){
				return 3;
			}
			else{
				printf("\n");
				return 2;
			}
		}
		else{
			map[abs][ord]=-map[abs][ord]-1;
			return 1;
		}
	}
}

int turn_1p(int map[DIM][DIM], int boat_table[DEF_BOAT], int score, int combo){ //the turn for score playing with scorekeeping and looping
	int x = turn(map, boat_table);
	if(x==0){
		score = max_(score-10, 0);
		map_print_1p(map, score);
		printf("MISS!\n");
		return turn_1p(map, boat_table, score, 1);
	}
	else if(x==1){
		score = score+(100*combo);
		map_print_1p(map, score);
		printf("HIT!\n");
		return turn_1p(map, boat_table, score, combo+1);
	}
	else if(x==2){
		score = score+(200*combo);
		map_print_1p(map, score);
		printf("SUNK!\n");
		return turn_1p(map, boat_table, score, combo+1);
	}
	else if(x==3){
		map_print_1p(map, score);
		printf("VICTORY!\n");
		return score+(200*combo);
	}
	else{
		printf("Fatal error\nError: Invalid turn ending");
		return 0;
	}
}

int menu(){ //main menu of the game
	int boat_type[DEF_BOAT_TYPE]={};
	int map[DIM][DIM]={0};
	int boat_table[DEF_BOAT]={0};
	int x=0;
	int seed = rand();
	srand(seed);
	printf(" [1] Play\n [2] High Scores\n [3] Credits\n [4] Exit\n\n\n");
	do{
		scanf("%d", &x);
	}while(x<1 || x>4);
	if(x==1){
		printf(" [1] Classic\n [2] Belgian Variant\n [3] CY Variant\n [4] Custom\n [5] Back\n\n\n");
		char y[4]={};
		do{
			fflush(stdin);
			scanf("%s", y);
			if(y[0]=='s' && y[1]=='e' && y[2]=='e' && y[3]=='d'){
				printf("Please enter the seed\n");
				scanf("%d", &seed);
				srand(seed);
				printf("Seed: %d\n", seed);
			}
			//printf("Char 1: %c\nChar 2: %c\nChar 3: %c\nChar 4: %c\n", y[0], y[1], y[2], y[3]);
		}while(y[0]<'1' || y[0]>'5');
		if(y[0]=='1'){
			boat_type[1] = 1;
			boat_type[2] = 2;
			boat_type[3] = 1;
			boat_type[4] = 1;
		}
		else if(y[0]=='2'){
			boat_type[0] = 4;
			boat_type[1] = 3;
			boat_type[2] = 2;
			boat_type[3] = 1;
		}
		else if(y[0]=='3'){
			boat_type[1] = 4;
			boat_type[2] = 2;
			boat_type[3] = 1;
			boat_type[4] = 1;
		}
		else if(y[0]=='4'){
			int z=0;
			while(z==0){
				for(int i=0; i<DEF_BOAT_TYPE; i++){
					printf("Numer of boats of length %d:\n", i+1);
					scanf("%d", &boat_type[i]);
					while(boat_type[i]<0 || boat_type[i]>DEF_BOAT){
						printf("Too many boats. Try again with less boats.");
						scanf("%d", &boat_type[i]);
					}
				}
				int boat_total=0;
				for(int i=0; i<DEF_BOAT_TYPE; i++){
					boat_total=boat_total+boat_type[i];
				}
				if(boat_total>DEF_BOAT){
					printf("Too many boats. Try again with less boats.");
				}
				else{
					z=1;
				}
			}
		}
		else if(y[0]=='5'){
			return menu();
		}
		int z=0;
		printf(" [1] Arcade mode\n [2] Versus Bot\n [3] Versus Player\n\n\n");
		do{
			scanf("%d", &z);
		}while(z<1 || z>3);
		if(z==1){
			if(map_init_1p(map, boat_table, boat_type)==1){
				printf("Sorry, but there was a problem with the generation of the map.\nIf you used a custom configuration, please try using less boats, or smaller boats.\nWe are sorry for the inconvenience\n");
				return menu();
			}
			else{
				HighScore PlayerHS;
				PlayerHS.Score = 0;
				map_print_1p(map, 0);
				PlayerHS.Score = turn_1p(map, boat_table, PlayerHS.Score, 1);
				if(PlayerHS.Score!=0){
					PlayerHS.Seed = seed;
					printf("Please enter your username (max 10 char)\n");
					PlayerHS.Name[0] = ' ';
					do{
						fflush(stdin);
						scanf("%s", PlayerHS.Name);
					}while(PlayerHS.Name[0]==' ');
					PlayerHS.Encryption = 0;
					FILE* fPtr = fopen("battleship_save.txt", "ab+");
					if(fPtr == NULL){
						printf("Sorry, an error has occured during the saving process\nError: %s",  strerror(errno));
						fclose(fPtr);
						return menu();
					}
					else{
						//int print1 = fprintf(fPtr, "%s %d %d %d\n", PlayerHS.Name, PlayerHS.Score, PlayerHS.Encryption, PlayerHS.Seed);
						fflush (stdout);
						/*if(print1!=0){
							printf("Save completed\n");
						}
						else{
							printf("Sorry, an error has occured during the saving process\nError: %s",  strerror(errno));
						}*/

						rewind(fPtr);
						int currentChar = EOF+1;
						int sizeHS = 0;
						while(currentChar != EOF){ //get the number of HS by counting lines
							currentChar = fgetc(fPtr);
							if(currentChar == '\n'){
								sizeHS++;
							}
						}

						printf("Nb of lines gathered\n");

						HighScore* tabHS = NULL;
						tabHS = malloc(sizeof(HighScore) * (1 + sizeHS));
						if(tabHS = NULL){
							printf("Sorry, an error has occured during the saving of high scores\nError: %s",  strerror(errno));
						}
						printf("SizeHS: %d\n", sizeHS);

						rewind(fPtr);
						
						scanf("");

						int j=0;
						/*fscanf(fPtr, "%s", tabHS[j].Name);
						printf("Name player %d: %s\n", j, tabHS[j].Name);
						fscanf(fPtr, "%d", &tabHS[j].Score);
						printf("Score player %d: %d\n", j, tabHS[j].Score);
						fscanf(fPtr, "%d", &tabHS[j].Encryption);
						printf("Encryption player %d: %d\n", j, tabHS[j].Encryption);
						fscanf(fPtr, "%d", &tabHS[j].Seed);
						printf("Seed player %d: %d\n", j, tabHS[j].Seed);
						printf("Tab created\n");

						for(int i=0; i<sizeHS; i++){
							fscanf(fPtr, "%s", tabHS[i].Name);
							printf("Name player %d: %s\n", i, tabHS[i].Name);
							fscanf(fPtr, "%d", &tabHS[i].Score);
							printf("Score player %d: %d\n", i, tabHS[i].Score);
							fscanf(fPtr, "%d", &tabHS[i].Encryption);
							printf("Encryption player %d: %d\n", i, tabHS[i].Encryption);
							fscanf(fPtr, "%d", &tabHS[i].Seed);
							printf("Seed player %d: %d\n", i, tabHS[i].Seed);
						}*/

						printf("HS got\n");

						/*for(int i=0; i<sizeHS; i++){
							printf("Name player %d: %s\n", i, tabHS[i].Name);
							printf("Score player %d: %d\n", i, tabHS[i].Score);
							printf("Encryption player %d: %d\n", i, tabHS[i].Encryption);
							printf("Seed player %d: %d\n", i, tabHS[i].Seed);
						}*/

						printf("HS displayed\n");

						/*char testlol[100];
						fscanf(fPtr, "%s", testlol);
						printf("1: %s\n", testlol);

						int test2;x
						fscanf(fPtr, "%d", &test2);
						printf("2: %d\n", test2);*/

						fclose(fPtr);
						return menu();
					}
				}
			}
		}
		else if(z==2){
			printf("WIP\n\n");
			return menu();
		}
		else if(z==3){
			printf("WIP\n\n");
			return menu();
		}
	}
	else if(x==2){
		printf("WIP\n\n");
		return menu();
	}
	else if(x==3){
		printf("        ______      __  __   _          __     _______\n");				// ______      __  __   _          __     _______
		printf("        | ___ \\     | | | | | |         | |   (_) ___ \\\n");			// | ___ \     | | | | | |         | |   (_) ___ \ .
		printf("        | |_/ / __ _| |_| |_| | ___  ___| |__  _| |_/ /\n");			// | |_/ / __ _| |_| |_| | ___  ___| |__  _| |_/ /
		printf("        | ___ \\/ _` | __| __| |/ _ \\/ __| '_ \\| |  __/\n");			// | ___ \/ _` | __| __| |/ _ \/ __| '_ \| |  __/
		printf("        | |_/ / (_| | \\_| \\_| |  __/\\__ \\ | | | | |\n");			// | |_/ / (_| | \_| \_| |  __/\__ \ | | | | |
		printf("        \\____/ \\__,_|\\__|\\__|_|\\___\\/___/_| |_\\_\\_|\n\n\n");	// \____/ \__,_|\__|\__|_|\___\/___/_| |_\_\_|
		printf("       __  __         _       _          __   __   _    _\n");			//  __  __         _       _          __   __   _    _
		printf("      |  \\/  |__ _ __| |___  | |____  _  \\ \\ / /__(_)__| |\n");		// |  \/  |__ _ __| |___  | |____  _  \ \ / /__(_)__| |
		printf("      | |\\/| / _` | _` / -_) | '_ \\ \\| |  \\ V / _ \\ | _` |\n");	// | |\/| / _` | _` / -_) | '_ \ \| |  \ V / _ \ | _` |
		printf("      |_|  |_\\__,_|__,_\\___\\ |_.__/\\_, |   \\_/\\___/_|__,_|\n");	// |_|  |_\__,_|__,_\___\ |_.__/\_, |   \_/\___/_|__,_|
		printf("                                   |__/\n\n\n");						//                              |__/
		printf("__      ___ _   _      _   _          _        _             __\n");			// __      ___ _   _      _   _          _        _             __
		printf("\\ \\    / (_) |_| |_   | |_| |_  ___  | |_  ___| |_ __   ___ / _|\n");			// \ \    / (_) |_| |_   | |_| |_  ___  | |_  ___| |_ __   ___ / _|
		printf(" \\ \\/\\/ /| |  _| ' \\  |  _| ' \\/ -_) | ' \\/ -_) | '_ \\ / _ \\  _|\n");	//  \ \/\/ /| |  _| ' \  |  _| ' \/ -_) | ' \/ -_) | '_ \ / _ \  _|
		printf("  \\_/\\_/ |_|\\__|_||_|  \\__|_||_\\___| |_||_\\___|_| .__/ \\___/_|\n");		//   \_/\_/ |_|\__|_||_|  \__|_||_\___| |_||_\___|_| .__/ \___/_|
		printf("                                                |_|\n\n\n");					//                                                 |_|
		printf("       ____  _____   ____   __      ___ _ _             _\n");				// ____  _____   ____   __      ___ _ _             _
		printf("       | __| | __ \\ / ___|  \\ \\    / (_) | |__ _ _ ____| |\n");			// | __| | __ \ / ___|  \ \    / (_) | |__ _ _ ____| |
		printf("       | _/  | |/ / \\ \\___   \\ \\/\\/ /| | | / _` | '_/ _` |\n");		// | _/  | |/ / \ \___   \ \/\/ /| | | / _` | '_/ _` |
		printf("       \\_| O |___/O  \\___/O   \\_/\\_/ |_|_|_\\__,_|_| \\__,_|\n\n\n");	// \_| O |___/O  \___/O   \_/\_/ |_|_|_\__,_|_| \__,_|
		return menu();
	}
	else if(x==4){
		return 0;
	}
}

int main(){
	if(DEF_BOAT>127 || DEF_BOAT_TYPE>127 || (DIM*DIM)>127){ //if def above expected (with margin)
		printf("Fatal error\nError: Invalid variable size");
		return 0;
	}
	srand(time(NULL));
	for(int i=0; i<20; i++){ //make rand more chaotic
		srand(rand());
	}
	char command[15];
	strcpy(command, "chcp 65001");
	system(command);
	printf("     ______      __  __   _          __     _______\n");         // ______      __  __   _          __     _______    // ______      __  __   _         __     _______
	printf("     | ___ \\     | | | | | |         | |   (_) ___ \\\n");      // | ___ \     | | | | | |         | |   (_) ___ \ . // | ___ \     | | | | | |        | |   (_) ___ \ .
	printf("     | |_/ / __ _| |_| |_| | ___  ___| |__  _| |_/ /\n");        // | |_/ / __ _| |_| |_| | ___  ___| |__  _| |_/ /   // | |_/ / __ _| |_| |_| | _______| |__  _| |_/ /
	printf("     | ___ \\/ _` | __| __| |/ _ \\/ __| '_ \\| |  __/\n");      // | ___ \/ _` | __| __| |/ _ \/ __| '_ \| |  __/    // | ___ \/ _` | __| __| |/ _ \ __| '_ \| |  __/ 
	printf("     | |_/ / (_| | \\_| \\_| |  __/\\__ \\ | | | | |\n");        // | |_/ / (_| | \_| \_| |  __/\__ \ | | | | |       // | |_/ / (_| | \_| \_| |  __/__ \ | | | | |
	printf("     \\____/ \\__,_|\\__|\\__|_|\\___\\/___/_| |_\\_\\_|\n\n\n");// \____/ \__,_|\__|\__|_|\___\/___/_| |_\_\_|       // \____/ \__,_|\__|\__|_|\___/___/_| |_\_\_|
	return menu();
}
//add saves
//add cheat detector
//add settings
//add 2p
//add makefile