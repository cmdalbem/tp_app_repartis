CC = g++
FLAGS = -Wall -Wextra -I ../File
LIBS = -ljson_linux-gcc-4.7_libmt

# Seulement ajoutez ici le nom du nouveaux module qui tu a crée ;)
OBJS = pubsub.o File.o FilesManager.o 

%.o: %.cpp %.h
		$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 
all: $(OBJS)

clean:
		rm -f *.o
