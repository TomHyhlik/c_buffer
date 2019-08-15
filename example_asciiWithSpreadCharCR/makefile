
CC = gcc
FLAGS = -pthread

TARGET = bin

C_INCLUDES = \
-I../../c_ByteArray/ \
-I../

C_Sources = \
main.c \
../buffer_ring.c \
../../c_ByteArray/ByteArray.c \

$(TARGET): *.c
	$(CC) $(FLAGS) $(C_INCLUDES) $(C_Sources) -o $(TARGET)

clean:
	-rm -f $(TARGET)

run:
	./$(TARGET)