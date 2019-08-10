#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ByteArray.h"
// #include "../ByteArray/ByteArray.h"

#include "buffer_ring.h"



///////////////////////////////////////////////////////////
/*
 * @brief initialize the buffer with zero values 
 */
void cb_clear(bufferRing_t* buffer)
{
    memset(buffer->data, 0, BUFFER_SIZE);
    buffer->head = 0;
    buffer->tail = 0;
}
///////////////////////////////////////////////////////////
bool cb_writeByte(bufferRing_t* buffer, uint8_t byteToBeWritten)
{
	if (buffer->overwriteMode == false) {
		if (cb_isFull(buffer)) {
			printf("ERROR: Failed to write to buffer, it's full\r\n");
			return false;
		}
	}
	/* write */
	buffer->data[buffer->tail++] = byteToBeWritten;
	
	return true;
}
///////////////////////////////////////////////////////////
/*
 * @brief write data to the buffer
 * @return false if the buffer is full
 */
bool cb_write(bufferRing_t* buffer, uint8_t* data, int data_len)
{
	if (buffer->overwriteMode == false) {
		/* check if there is enough free space in the buffer */
		if ((int)cb_avaiolableSpace(buffer) < data_len) {
			printf("ERROR: write to buffer, not enough space\r\n");
			return false;
		}
	}
	/* write */
	for (int i = 0; i < data_len; i++, buffer->tail++) {
		buffer->data[buffer->tail] = data[i];
	}
	return true;
}
///////////////////////////////////////////////////////////
/* 
 * @brief returns the number of bytes in the buffer
 */
uint8_t cb_writtenBytesCnt(bufferRing_t* buffer)
{
	int writtenBytesCnt;
	if (buffer->tail >= buffer->head) {
		writtenBytesCnt = buffer->tail - buffer->head;
	} else {
		writtenBytesCnt = BUFFER_SIZE - (buffer->head - buffer->tail);
	} 
	return (uint8_t) writtenBytesCnt;
}
///////////////////////////////////////////////////////////
/* 
 * @brief returns available space in the buffer
 */
uint8_t cb_avaiolableSpace(bufferRing_t* buffer)
{
	return (BUFFER_SIZE - cb_writtenBytesCnt(buffer) - 1);
}
///////////////////////////////////////////////////////////
bool cb_isEmpty(bufferRing_t* buffer)
{
	return (buffer->head == buffer->tail);
}
///////////////////////////////////////////////////////////
bool cb_isFull(bufferRing_t* buffer)
{
	uint8_t tail_new = buffer->tail;
	tail_new += 1;
	return (tail_new == buffer->head);
}
///////////////////////////////////////////////////////////
/*
 * @brief read All data from the circular buffer at concrete position
 * @param destination must hav at least size as BUFFER_SIZE
 * @return number of bytes that were loaded into the destination
 */
uint8_t cb_readAll(void* destination, bufferRing_t* buffer)
{
	if (cb_isEmpty(buffer))
		return 0;

   uint8_t *dest = (uint8_t *)destination;

   uint8_t destPtr;
	for (destPtr = 0; buffer->head != buffer->tail; destPtr++, buffer->head++) {
		dest[destPtr] = buffer->data[buffer->head];
		buffer->data[buffer->head] = 0;
	}
	return destPtr;
}
///////////////////////////////////////////////////////////
/*
 * @brief read single byte from the buffer and removes it
 * 	in the buffer
 */
uint8_t cb_readByte(bufferRing_t* buffer)
{
	if (cb_isEmpty(buffer))
		return 0;

	uint8_t outputByte = buffer->data[buffer->head];
	buffer->data[buffer->head] = 0;
	buffer->head++;
	return outputByte;
}
///////////////////////////////////////////////////////////
void cb_cpy(void* destination, bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size)
{
   uint8_t *dest = (uint8_t *)destination;

	for (uint8_t destPtr = 0; destPtr < size; destPtr++, bufferPtr++) {
		dest[destPtr] = buffer->data[bufferPtr];
	}
}
///////////////////////////////////////////////////////////
void cb_rm(bufferRing_t* buffer, uint8_t bufferPtr, uint8_t size)
{
	for (; size != 0; size--, bufferPtr++) {
		buffer->data[bufferPtr] = 0;
	}
}

///////////////////////////////////////////////////////////
/*
 * @brief reads data till the splitting bytes CR (Carriage Return) or LF (Line Feed)
 * 		Aka \r\n, the sequence is accepted and each single byte is accepted also
 * 		So one command is red from the buffer to the destinnation and its size
 * 		is returned
 * @note if the buffer is full and no splitting byte was found, the buffer is cleared
 * @note the buffer must not be in overvrite mode
 * @return the size of cmd that was red from the buffer, 
 * 			if no split factor found, return -1
 */
int cb_readTillCR(void* destination, bufferRing_t* buffer)
{
	if (buffer->overwriteMode) {
		printf("ERROR: \r\n");
		return -1;
	}

	if (cb_isEmpty(buffer)){
		return -1;
	}

   uint8_t *dest = (uint8_t *)destination;

   /* search the split factor in the buffer from its beginning */
   for (uint8_t cmdEnd = buffer->head; cmdEnd != buffer->tail; cmdEnd++)
   {
	   if (isCR(buffer->data[cmdEnd]))
	   {
			/* splitting factor found at position cmdEnd */
			int cmdSize;
			if (cmdEnd >= buffer->head) {
				cmdSize = cmdEnd - buffer->head;
			} else {
				cmdSize = BUFFER_SIZE - (buffer->head - cmdEnd);
			} 

			/* feed the buffer data to the dest (destination */
			for (int i = 0; i < cmdSize; i++) {
				dest[i] = cb_readByte(buffer);
			}
			dest[cmdSize] = '\0';

			HAL_Delay(1);
			cb_rmCR_atBegin(buffer);

			return cmdSize;
	   }
   }
   /* if the buffer is full and no splitting byte was found, clear the buffer */
   if (cb_isFull(buffer)){
	   cb_clear(buffer);
   }
   return -1;
}
///////////////////////////////////////////////////////////
/* 
 * @brief remove all spreading factors '\r' or '\n' at the beginning
 * 		of the buffer
 */
void cb_rmCR_atBegin(bufferRing_t* buffer)
{
	if (cb_isEmpty(buffer))
		return;


		while (isCR(buffer->data[buffer->head])) {
			cb_readByte(buffer);	// remove byte from buffer
		}
}

///////////////////////////////////////////////////////////
bool isCR(uint8_t byte)
{
	return (byte == '\r' || byte == '\n');
}
///////////////////////////////////////////////////////////
void cb_print(bufferRing_t* buffer)
{
	printf("BUFFERCONTENT: ");
	for (uint8_t ptr = buffer->head; ptr != buffer->tail; ptr++)
	{
		printf("%.2X ", buffer->data[ptr]);
	}
	printf("\r\n");
}

///////////////////////////////////////////////////////////








