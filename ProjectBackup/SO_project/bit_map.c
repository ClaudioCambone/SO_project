#include "bit_map.h"
#include <assert.h>
#include <stdio.h>


// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + (bits%8!=0);
}
// initializes a bitmap on an external array
void BitMap_init(BitMap *bit_map, int num_bits, uint8_t *buffer) {
  bit_map->buffer = buffer;
  bit_map->num_bits = num_bits;
  bit_map->buffer_size = BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=byte_num&0x03;
  if (status) {
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
}

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap *bit_map, int bit_num) {
  int byte_num = bit_num >> 3;
  assert(byte_num < bit_map->buffer_size);
  int bit_in_byte = byte_num % 8;
  return (bit_map->buffer[byte_num] & (1 << bit_in_byte)) != 0;
}





void BitMap_setall(BitMap* bit_map,int status){
    for(int i = 0; i < bit_map->num_bits; i++){
       BitMap_setBit(bit_map, i, status);
    }

}

void BitMap_printall(BitMap* bit_map){
    for(int i = 0; i < 50; i++  ){
      printf("valore: %d numero: %d \n", BitMap_bit(bit_map,i), i);
      //if(BitMap_bit(bit_map,i) == 1)
        //printf("TROVATO \n\n\n");
    }
      printf("\n ---END OF BITMAP--- \n\n");
}