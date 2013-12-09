CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = 

all:
		$(MAKE) -C File 
		$(MAKE) -C Sender 
		$(MAKE) -C Interface
		$(MAKE) -C CreateServer
		$(MAKE) -C Server

clean:
		$(MAKE) -C File clean 
		$(MAKE) -C Sender clean
		$(MAKE) -C CreateServer clean
		$(MAKE) -C Interface clean
		$(MAKE) -C Server clean
