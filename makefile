
CFLAGS = -Wall
PROG = gencal
OBJ = gencal.o
DIR = /usr/local/bin

$(PROG): $(OBJ)
	gcc $(LDFLAGS) -o $(PROG) $(OBJ)

$(OBJ): gencal.c
	gcc $(CFLAGS) -c gencal.c -o $(OBJ)

install:
	cp -pv $(PROG) $(DIR)/$(PROG)

clean:
	rm -f $(OBJ) $(PROG)
	
all:
	make clean
	make

