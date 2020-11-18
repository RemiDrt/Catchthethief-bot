CC = gcc -Wall -Wextra -std=c99

all : scriptCTT

scriptCTT : scriptCTT.o
	$(CC) scriptCTT.o -o scriptCTT

%.o : %.c %.h
	$(CC) -c $< -o $@