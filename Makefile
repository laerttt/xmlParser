CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = xml_parser

all: $(TARGET)

$(TARGET): main.o xml_parser.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o xml_parser.o

main.o: main.c xml_parser.h
	$(CC) $(CFLAGS) -c main.c

xml_parser.o: xml_parser.c xml_parser.h
	$(CC) $(CFLAGS) -c xml_parser.c

clean:
	rm -f *.o $(TARGET)
