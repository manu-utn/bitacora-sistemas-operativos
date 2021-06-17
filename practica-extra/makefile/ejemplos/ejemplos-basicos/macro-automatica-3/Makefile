# - La macro $? obtendrá sólo la dependencia más reciente
# - Se intentará compilar un archivo con el último .c modificado
main.o: main.c players.h players.c
	@echo "Se compilaron los archivos" $?
#	gcc $? -c $@

# el codigo de abajo no cuenta para el ejemplo
init:
	touch main.o main.c players.c players.h

.PHONY: clean
clean:
	-rm *.{c,h,o}
