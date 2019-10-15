/*
 * This example scans terminal input in thread, puts the scanned data into the ring buffer
 * and in the main loop processes the buffer
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h> 

#include "ByteArray.h"
#include "buffer_ring.h"

#define CMD_QUIT            "quit"

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

    buffer.overwriteMode = false;
    buffer_clear(&buffer);      // init with zero values

    while(1)
    {
        processBuffer(&buffer);
    }
    return 0;
}

////////////////////////////////////////////////////////////
/*
 * @brief do something with the command...
 */
void processCmd(uint8_t* data, uint8_t data_len)
{
	pAscii_tit("Received: ", data, data_len);

    if (data_len > 0) {
        if (arraysEqual(data, (uint8_t*)CMD_QUIT, data_len)) {
            printf("Quitting...\n");
            exit(0);
        }
    }
}

////////////////////////////////////////////////////////////
void processBuffer(bufferRing_t* buffer)
{
	uint8_t cmd[BUFFER_SIZE];
	int cmd_len;

	do {
        /* get the first command in the buffer */
		cmd_len = buffer_readTillCR(cmd, buffer);
        /* process the command */
		if(cmd_len != -1) {
			processCmd(cmd, cmd_len);
        }
	} while (cmd_len);
}
////////////////////////////////////////////////////////////
/*
 * @brief continuous receiving data from the terminal
 *                  and adding it to the buffer
 */
void* thread_processBuffer(void *vargp)
{
    while (1) 
    {
        char inputChar = getchar();
        buffer_write(&buffer, (uint8_t*)&inputChar, 1);
    }
}
////////////////////////////////////////////////////////////
















