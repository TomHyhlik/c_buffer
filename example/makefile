
CC = gcc
FLAGS = -pthread

TARGET = bin

C_INCLUDES = \
-I../../ByteArray/ \
-I../

C_Sources = \
main.c \
../buffer_ring.c \
../../ByteArray/ByteArray.c \

$(TARGET): *.c
	$(CC) $(FLAGS) $(C_INCLUDES) $(C_Sources) -o $(TARGET)

clean:
	-rm -f $(TARGET)

run:
	./$(TARGET)