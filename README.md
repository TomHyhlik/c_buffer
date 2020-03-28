# c_Buffer
Basic library for buffering received data via serial interface such as UART in embedded system.
The Buffer has fixed size 256 B and data are buffered circularily in overwrite or non-overwrite mode. That means that if the buffer is full and remains unprocessed, the new received data will overwrite the old data or the new received data will be lost.

## todo:
	Example with overwrite mode
