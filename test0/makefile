ALL = main
all: $(ALL)

main: main.o a.o
	g++ $^ -o $@

%.o: %.cpp a.h
	g++ -c $< -o $@

blah:
	echo blah

clean:
	rm *.o *~ $(ALL) 
