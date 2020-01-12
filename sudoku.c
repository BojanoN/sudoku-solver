#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int* load_puzzle(const char* filename){

  int* ret = (int *)malloc(sizeof(int)*81);

  if(ret==NULL){
    perror(strerror(errno));
    return ret;
  }

  FILE* file = fopen(filename, "r");

  if(file==NULL){
    perror(strerror(errno));
    free(ret);
    return NULL;
  }
  char buff[512] = {0};
  char* tmp;
  char* tmp_buff = buff;
  int i, j;
  i = 0;
  while(fgets(buff, 128, file)){
    for(j=0, tmp=strtok(tmp_buff, " "); j<9; j++, tmp=strtok(NULL, " ")){
      // TODO: prebacit na BSD strtoi
      ret[i*9 + j] = atoi(tmp);
    }
    i++;
  }

  fclose(file);
  return ret;
}

int solve_puzzle(int* field, int i, int j){

  char mask[10] = {0};

  if(i >= 9){
    return 0;
  }

  if(field[i*9 + j] == 0){
    // Row check
    for(int k=0; k<9; k++){
      mask[field[i*9 + k]] = 1;
    }
    // Column check
    for(int k=0; k<9; k++){
      mask[field[k*9 + j]] = 1;
    }

    // Box check
    for(int o = (i/3) * 3; o< ((i/3)+1)*3; o++){
      for(int p = (j/3) * 3; p< ((j/3)+1)*3; p++){
        mask[field[o*9 + p]] = 1;
      }
    }
    #ifdef DEBUG
    printf("%d %d\n", i, j);
    printf("%d %d\n\n", (i/3)*3, (j/3)*3);
    printf("%d %d\n\n", (i/3)*3, (j/3)*3);

    #endif

    for(int n=1; n<10; n++){
      if(mask[n] == 0){
        field[i*9 + j] = n;
        int ret = solve_puzzle(field, (j+1 < 9) ? i : i+1, (j+1) % 9);
        if(ret){
          field[i*9 + j] = 0;
        }else{
          return 0;
        }
      }
    }
    return 1;
  }else{
    for(int k=i; k<9; k++){
      for(int l=j; l<9; l++){
        if(field[k*9 + l] == 0){
          int ret = solve_puzzle(field, k, l);
          if(ret){
            return 1;
          }
        }
      }
    }

  }

  return 0;
}

int check_solution(int* field){
  int sum = 0;
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){

      // Row check
      sum = 0;
      for(int k=0; k<9; k++){
        sum += field[i*9 + k];
      }

      if(sum != 45){
        return 1;
      }
      // Column check
      sum = 0;
      for(int k=0; k<9; k++){
        sum += field[k*9 + j];
      }

      if(sum != 45){
        return 1;
      }
      // Box check
      sum = 0;
      for(int o = (i/3) * 3; o< ((i/3)+1)*3; o++){
        for(int p = (j/3) * 3; p< ((j/3)+1)*3; p++){
          sum += field[o*9 + p];
        }
      }

      if(sum != 45){
        return 1;
      }

    }
  }

  return 0;
}


int main(int argc, char** argv){

  if(argc != 2){
    perror("Input file needed!\n");
    return -1;
  }

  int* field;
  field = load_puzzle(argv[1]);

  if(field == NULL){
    return -2;
  }

  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      printf("%d ", field[i*9 + j]);
      if((j+1)%3 == 0){
        printf(" ");
      }
    }
    printf("\n");
    if((i+1)%3 == 0){
      printf("\n");
    }
  }
  printf("\n");

  int ret = solve_puzzle(field, 0, 0);

  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      printf("%d ", field[i*9 + j]);
      if((j+1)%3 == 0){
        printf(" ");
      }
    }
    printf("\n");
    if((i+1)%3 == 0){
      printf("\n");
    }
  }

  int valid = check_solution(field);

  printf("Solution is %s!\n", valid ? "invalid" : "valid");

  free(field);
  return ret;
}
