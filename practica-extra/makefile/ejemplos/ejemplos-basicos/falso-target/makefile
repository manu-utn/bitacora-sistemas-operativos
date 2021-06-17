all: cliente servidor

cliente: cliente.o jugadores.o monstruos.o
	gcc -o cliente cliente.o jugadores.o monstruos.o

servidor: servidor.o configuraciones.o
	gcc -o servidor servidor.o configuraciones.o

clean:
	rm *.o;\
	rm cliente servidor
