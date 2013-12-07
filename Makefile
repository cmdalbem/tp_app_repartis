CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = 

all:
		$(MAKE) -C File 
		$(MAKE) -C Sender 
		$(MAKE) -C Interface
		$(MAKE) -C Server

test: test.cpp test.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

clean:
		$(MAKE) -C File clean 
		$(MAKE) -C Sender clean
		$(MAKE) -C Interface clean
		$(MAKE) -C Server clean
