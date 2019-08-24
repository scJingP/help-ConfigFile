SOURCE_FILES=\
    main.cpp\
    ConfigFile.h\
    ConfigFile.cpp

INCLUDES=\
	-I.\

CFLAGS=\
    -D_LARGEFILE64_SOURCE\
    -fPIC\
	-g -Wall

LIBS_PATH=\
	-L.\


LIBS=\
	
main : main.o ConfigFile.o
	g++ main.o ConfigFile.o -o main $(CFLAGS) $(INCLUDES) $(LIBS_PATH) $(LIBS)

main.o : main.cpp ConfigFile.h
	g++ -c main.cpp $(CFLAGS) $(INCLUDES)

ConfigFile.o : ConfigFile.cpp ConfigFile.h
	g++ -c ConfigFile.cpp $(CFLAGS) $(INCLUDES)
	
.PHONY : clean
clean:
	rm -f main
	rm -f *.o
	


