SRCS = Employee.cpp BST.cpp main.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

database:	main.o Employee.o BST.o 
	g++ -std=c++11 $^ -o $@
main.o:	main.cpp
	g++ -std=c++11 -c -g main.cpp 
Employee.o:	Employee.cpp Employee.h
	g++ -std=c++11 -c -g Employee.cpp
BST.o:	BST.cpp BST.h Employee.h
	g++ -std=c++11 -c -g BST.cpp
clean:
	rm -f *.o  database core

