# - Se compilará un archivo llamado "programa.o"
# - La macro $< obtendrá unicamente la primer dependencia (players.c)
# - La segunda dependencia (players.h) nunca será usada por $<
# - La macro $@ obtiene el nombre del target (programa.o)
programa.o: players.c players.h
	gcc -c $< -o $@

# codigo de abajo no cuenta para el ejemplo
init:
	touch players.c players.h

.PHONY: clean
clean:
	-rm *.{c,h}
