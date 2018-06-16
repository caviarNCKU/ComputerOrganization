#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_CACHE 500000
int main(int argc, char *argv[]){
	FILE *f,*f2;
	char line[1024];
	int cache_size, block_size, associate, algo;
	int block_num, tag, temp;
	int count = 0, index = 0, byte_offset = 0;
	int i,j,cache_index = 0,cache_tag = 0,cache_offset = 0,line_num = 1;
	int cache_miss[MAX_CACHE];
	//int *cache_miss;
	double miss_rate;
	typedef struct cache {
		int valid;
		int tag;
		int data;
	} CACHE;	

	CACHE cache_data[MAX_CACHE];
	//CACHE *cache_data;
	//cache_data = (CACHE*)malloc(line_num*sizeof(CACHE));
	//cache_miss = (int*)malloc(line_num*sizeof(int));

	f  = fopen(argv[1],"r");
	f2 = fopen(argv[2],"w");

	while(fgets(line, sizeof(line), f)){
		if(count == 0)
			cache_size = atoi(line);
		else if(count == 1)
			block_size = atoi(line);
		else if(count == 2)
			associate = atoi(line);
		else if(count == 3){
			algo = atoi(line);
			break;
		}
		count++;
	}
	/*count cache entries, tag, index, byte_offset*/
	block_num = cache_size * 1024 / block_size;
	temp = block_num;
	while(block_num % 2 == 0){
		index++;
		block_num /= 2;
	}
	block_num  = temp;
	temp = block_size;
	while(block_size % 2 == 0){
		byte_offset++;
		block_size /= 2;
	}
	block_size = temp;
	tag = 32 - index - byte_offset;

	long int x;
	int binaryNum[32];
	int digit;
	while(fgets(line, sizeof(line), f)){
		x = strtol(line,NULL,16);
		i = 0, j = 0, cache_index = 0, cache_tag = 0, cache_offset = 0;
		/*split binary*/
		while(x > 0){
			binaryNum[i] = x % 2;
			x /= 2;
			i ++;
		}
		digit = index;
		for(j = 32-tag-1; j >= byte_offset; j--){
			if(binaryNum[j] == 1){
				cache_index = cache_index + (int)pow(2,digit);
			}
			digit--;
		}
		digit = tag;
		for(j = 31; j >= index + byte_offset; j--){
			if(binaryNum[j] == 1){
				cache_tag = cache_tag + (int)pow(2,digit);
			}
			digit--;
		}
		for(j = byte_offset - 1; j >= 0; j--){
			if(binaryNum[j] == 1){
				cache_offset = cache_offset + (int)pow(2,j);
			}
		}
		/*Associative*/
		if(associate == 1){
			printf("1\n");
			if(cache_data[cache_index].valid != 1){
				cache_data[cache_index].valid = 1;
				cache_data[cache_index].tag = tag;
				cache_miss[line_num] = 1;
			}
			if(cache_data[cache_index].tag != tag){
				cache_miss[line_num] = 1;
			}		
		}
		else if(associate == 2){
			printf("2\n");
			if(cache_data[cache_index].valid != 1){
				cache_data[cache_index].valid = 1;
				cache_data[cache_index].tag = tag;
				cache_miss[line_num] = 1;
			}
			if(cache_data[cache_index].tag != tag){
				cache_miss[line_num] = 1;
			}	
		}
		else if(associate == 3){
			printf("3\n");
			if(cache_data[cache_index].valid != 1){
				cache_data[cache_index].valid = 1;
				cache_data[cache_index].tag = tag;
				cache_miss[line_num] = 1;
			}
			if(cache_data[cache_index].tag != tag){
				cache_miss[line_num] = 1;
			}	
		}
		
		line_num ++;
	}

	fprintf(f2,"Hit instructions:");
	int temp2  = 0, total_miss = 0;
	for(i = 1 ; i < line_num; i++){
		if(cache_miss[i] != 1){
			if(temp2 == 0){
				fprintf(f2,"%d",i);
				temp2 = 1;
			}
			else{
				fprintf(f2,",%d",i);
			}
		}
	}
	temp2 = 0;
	fprintf(f2,"\nMisses instructions:");
	for(i = 1; i < line_num; i++){
		if(cache_miss[i] == 1){
			if(temp2 == 0){
				fprintf(f2,"%d",i);
				temp2 = 1;
			}
			else{
				fprintf(f2,",%d",i);
			}
			++total_miss;
		}
	}
	miss_rate = (double)total_miss / (double)(line_num - 1);
	fprintf(f2,"\nMiss rate: %f\n", miss_rate);
	//free(cache_data);
	//free(cache_miss);
	fclose(f);
	fclose(f2);
}