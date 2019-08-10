/*
 * This example scans terminal input in thread, puts the scanned data into the ring buffer
 * and in the main loop processes the buffer
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h> 

#include "ByteArray.h"
#include "buffer_ring.h"


////////////////////////////////////////////////////////////
bufferRing_t buffer;

void processBuffer(bufferRing_t* buffer);
void processCmd(uint8_t* data, uint8_t data_len);

void* thread_processBuffer(void *vargp);

////////////////////////////////////////////////////////////
int main(void)
{
    printf("\nAppStert........................................\n");

    /* start the thread of processing buffer */ 
    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, thread_processBuffer, NULL); 

    cb_clear(&buffer);      // init with zero values

    /* create message */
    char message[] = "AT+CEREG?\nAT+CMEE=1\nAT+CONFIG\nAT+00000000\n"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
    "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n";

    /* put the message to buffer, every single byte extra */
    for (int i = 0; i < strlen(message); i++) {
    	cb_write(&buffer, (uint8_t*)&message[i], 1);
    }

    // printf("Number of bytes in the buffer: %u\n", buffer.count);

    while(1)
    {
    //////////////////////////////////// MAIN LOOP

        /* get data from the keyboard input and write it to the buffer */
        char inputChar = getchar();
        cb_write(&buffer, (uint8_t*)&inputChar, 1);

    ////////////////////////////////////
    }
    return 0;
}
////////////////////////////////////////////////////////////
void processBuffer(bufferRing_t* buffer)
{
	uint8_t cmd[BUFFER_SIZE];
	uint8_t cmd_len;

	do {
        /* get the first command in the buffer */
		cmd_len = cb_readTillCR(cmd, buffer);
        /* process the command */
		if(cmd_len)
			processCmd(cmd, cmd_len);
	} while (cmd_len);
}
////////////////////////////////////////////////////////////
/*
 * @brief do something with the command... implement as you wish.
 */
void processCmd(uint8_t* data, uint8_t data_len)
{
	// pHex_nl(data, data_len);
	pAscii_tit("CMD: ", data, data_len);
}
////////////////////////////////////////////////////////////
/*
 * @brief continuous processing of the buffer
 */
void* thread_processBuffer(void *vargp)
{
    while (1) {
        processBuffer(&buffer);
    }
}
////////////////////////////////////////////////////////////
















