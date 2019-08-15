#ifndef BUFFER_RING_H_
#define BUFFER_RING_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>


#define BUFFER_SIZE		256

typedef struct {
	uint8_t data[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	bool overwriteMode;
} bufferRing_t;



/* Write data to buffer */
bool buffer_write(bufferRing_t* cb, uint8_t* data, int data_len);
bool buffer_writeByte(bufferRing_t* buffer, uint8_t byteToBeWritten);

/* read the data from the buffer into destination and remove it in the buffer */
uint8_t buffer_readAll(void* destination, bufferRing_t* cb);
uint8_t buffer_readByte(bufferRing_t* buffer);
int buffer_readTillCR(void* destination, bufferRing_t* buffer);

void buffer_rmCR_atBegin(bufferRing_t* buffer);

void buffer_cpy(void* destination, bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size);
void buffer_rm(bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size);

uint8_t buffer_readByte(bufferRing_t* cb);
bool buffer_isEmpty(bufferRing_t* buffer);
bool buffer_isFull(bufferRing_t* buffer);
uint8_t buffer_avaiolableSpace(bufferRing_t* buffer);
uint8_t buffer_writtenBytesCnt(bufferRing_t* buffer);

void buffer_print(bufferRing_t* buffer);


bool buffer_find(uint8_t* foundPosition, bufferRing_t* buffer,
	uint8_t searchedByte);
bool isCR(uint8_t byte);


void buffer_clear(bufferRing_t* buffer);




#endif
