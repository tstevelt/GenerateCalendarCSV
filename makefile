PROG = GenerateCalendarCSV
LIB = GenerateCalendarCSV.a
DEF = /usr/local/include
PRG = /usr/local/bin/gencal

FILES = \
	$(LIB)(gencal.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

