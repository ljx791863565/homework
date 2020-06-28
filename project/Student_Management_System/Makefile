ser:
	gcc server.c threadpool.c -o ser -lpthread  -L/usr/local/lib/mysql -lmysqlclient
cli:
	gcc client.c menu.c student.c -lpthread -o cli
clean:
	rm ser cli
