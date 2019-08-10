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
bool cb_write(bufferRing_t* cb, uint8_t* data, int data_len);
bool cb_writeByte(bufferRing_t* buffer, uint8_t byteToBeWritten);

/* read the data from the buffer into destination and remove it in the buffer */
uint8_t cb_readAll(void* destination, bufferRing_t* cb);
uint8_t cb_readByte(bufferRing_t* buffer);
int cb_readTillCR(void* destination, bufferRing_t* buffer);
void cb_cpy(void* destination, bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size);
void cb_rm(bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size);

uint8_t cb_readByte(bufferRing_t* cb);
bool cb_isEmpty(bufferRing_t* buffer);
bool cb_isFull(bufferRing_t* buffer);
uint8_t cb_avaiolableSpace(bufferRing_t* buffer);
uint8_t cb_writtenBytesCnt(bufferRing_t* buffer);

void cb_print(bufferRing_t* buffer);


bool cb_find(uint8_t* foundPosition, bufferRing_t* buffer,
	uint8_t searchedByte);
bool isCR(uint8_t byte);


void cb_clear(bufferRing_t* buffer);




#endif
