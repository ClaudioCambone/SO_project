#include <stdio.h>
#include <assert.h>
#include <math.h> 
#include "my_buddy_allocator.h"
#include <stdlib.h>

// these are trivial helpers to support you in case you want
// to do a bitmap implementation

int levelIdx(size_t size){
  return MAXLEVELS - (ceil(log2(size)));
}

int buddyIdx(int idx){
  if (idx&0x1){
    return idx-1;
  }
  return idx+1;
}

int parentIdx(int idx){
  return idx/2;
}

int leftchild(int parentindx){
    return parentindx * 2 + 1;
}

int rightchild(int parentindx) {
  return parentindx * 2 + 2;
}

int mybuddy_getNumBuddies(const BitMap* bit_map, int level) {
    int total_num_bits = bit_map->num_bits;
    int buddy_size = 1 << (MAXLEVELS - level); // Calculate the size of the buddy at the specified level

    int num_buddies = total_num_bits / buddy_size;
    return num_buddies;
}

void mybuddy_init (mybuddy *buddy_alloc,char *memory, uint8_t *buffer){
    int num_bits = (1 << MAXLEVELS); //CALCOLATE NUM_BITS REQUIRED TO REPRESENT A TREE WITH MAXLEVELS levels (like pow(2,maxlevels +1) -1)
    BitMap_init(&buddy_alloc->bitmap,num_bits,buffer);
    buddy_alloc->memory = memory;
    buddy_alloc->num_levels = MAXLEVELS;

    // for(int i = 0; i <= MAXLEVELS; i++){
    //     buddy_alloc->num_nodes[i] = (1 << i);  //each floor the number of nodes is 2^i, e.g. floor i = 3 has 2^3 = 8 nodes  (like pow(2,i) but it is an integer and not float)
    // }
}

int mybuddy_getFree(BitMap* bit_map, int level) {
  int start = (1 << level) - 1;
  int end = (1 << (level + 1)) - 1;
  for(int i = start; i <= end; i++) {
    if(BitMap_bit(bit_map, i) == FREE) {
      printf("Free buddy at level %d, It has index %d in the BitMap \n\n", level, i);
      return i;
    }
  }
  printf("No available buddies found at level %d\n", level);
  return -1;
}

// TO-DO 
int mybuddy_allocBuddy(mybuddy *buddy_alloc, int nec_level){
    if( (1 << nec_level) > (1 << MAXLEVELS)){
        perror("memory requested overflow available");
        return -1;
    }
    if(nec_level < 0)
        return -1;


    assert(nec_level >= 0);
    //int numbuddies =  (1 << nec_level) - 1;  
    //printf("num buddies %d level %d \n\n",numbuddies,nec_level);
    int idx = mybuddy_getFree(&buddy_alloc->bitmap,nec_level);   
    if(idx == -1){

      printf("N.A. Memory, cheching lv below \n\n");
      int  parent =  mybuddy_allocBuddy(buddy_alloc, nec_level - 1);

      if(parent == -1){
        //if no memory is available goes here
        printf("Not enough memory, abort allocBuddy \n");
        return -1;
      }
      else{
      BitMap_setBit(&buddy_alloc->bitmap,parent,1);
      return leftchild(parent);
      }
        
    }
    else{
      BitMap_setBit(&buddy_alloc->bitmap, idx , ALLOCATED);
      printf("memory allocated, buddy has index %d \n",idx);
      return idx;
    }
}

void printbuddy(mybuddy *buddyalloc){
    printf("BITMAP PRINT \n");
    BitMap_printall(&buddyalloc->bitmap);
    printf("BUFFER SIZE IN BYTE %d \n", buddyalloc->bitmap.buffer_size);
    printf("MEMORY SIZE %d \n", MEMORYSIZE);
    printf("NUMBITS %d \n", buddyalloc->bitmap.num_bits);
    printf("NUMLEVELS %d \n",buddyalloc->num_levels);

}





int main(){
    char memory[MEMORYSIZE];
    //TESTING TIME
    uint8_t buffer[BUFFER_SIZE]; 
    mybuddy buddy;
    mybuddy_init(&buddy,memory,buffer);
    //BitMap_setBit(&buddy.bitmap, 2097149, 1);
    //BitMap_setBit(&buddy.bitmap, 9, 1);
    //BitMap_setBit(&buddy.bitmap, 2049, 1);
    //BitMap_setbyte(buffer,32);
    //BitMap_setchunk(buffer,2060,32);
    //printbuddy(&buddy);
    
    //int result = BitMap_checkchunk(buffer, 44,63);
    //printf("risultato %d",result);
    //int i = levelIdx(1025); 
    //printf("%d",i);
    //i = BuddyAllocator_getFreeBuddy(&buddy.bitmap, i);

    //mybuddy_allocBuddy(&buddy,0);

    printbuddy(&buddy);
    
    for( int i = 0; i < 8; i++){
          mybuddy_allocBuddy(&buddy,3);
    }

    printbuddy(&buddy);

    /*
    FOR TESTING PURPOSES
    mybuddy_allocBuddy(&buddy,0);
    mybuddy_allocBuddy(&buddy,1);
    mybuddy_allocBuddy(&buddy,1);
    mybuddy_allocBuddy(&buddy,2);
    mybuddy_allocBuddy(&buddy,2);
    mybuddy_allocBuddy(&buddy,2);

    */

}





