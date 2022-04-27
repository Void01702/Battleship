#define DIM 10
#define DEF_BOAT 20
#define DEF_BOAT_TYPE 8

short boat_placement(int map[DIM][DIM], int abs, int ord, int ang, int length, int id){ //place the boats on the grid
	for(short i = 0; i<length; i++){
		if(map[abs][ord] != 0 || abs > DIM-1 || ord > DIM-1){
			return 0;
		}
		if(ang == 0){
			abs++;
		}
		else if(ang == 1){
			ord++;
		}
	}
	for(short i = 0; i<length; i++){
		if(ang == 0){
			abs--;
		}
		else if(ang == 1){
			ord--;
		}
		map[abs][ord] = id+1;
	}
	return 1;
}

short boat_init_1p(int map[DIM][DIM], int boat_table[DEF_BOAT], int length, int id){ //initialize the boats
	int abs;
	int ord;
	int ang;
	int x=0;
	int count=0;
	do{
		count++;
		if(count>1000){
			return 1;
		}
		abs = rand()%10;
		ord = rand()%10;
		ang = rand()%2;
		x = boat_placement(map, abs, ord, ang, length, id);
	}while(x==0);
	boat_table[id] = length;
	return 0;
}

short map_init_1p(int map[DIM][DIM], int boat_table[DEF_BOAT], int boat_type[DEF_BOAT_TYPE]){ //test for every kind of boat, gives them an ID and initialize them
	int id = 0;
	for(int length=0; length<DEF_BOAT_TYPE; length++){
		for(int x=0; x<boat_type[length]; x++){
			if(boat_init_1p(map, boat_table, length+1, id)==1){
				return 1;
			}
			id++;
		}
	}
}