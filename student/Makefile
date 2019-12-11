student.o:
	gcc -c student.c student.h
teacher.o:
	gcc -c teacher.c teacher.h
root.o:
	gcc -c root.c root.h
common.o:
	gcc -c common.c common.h
all:
	gcc  -o test main.c student.c root.c teacher.c common.c
clean:
	rm test *.o
