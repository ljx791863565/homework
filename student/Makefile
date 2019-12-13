object=main.o common.o student.o teacher.o root.o

all:$(object)
	gcc -o test $(object)
main.o:common.h student.h teacher.h root.h
common.o:common.h
student.o:common.h student.h
teacher.o:common.h teacher.h
root.o:common.h root.h

.PHONY:clean
clean:
	-rm test $(object)
