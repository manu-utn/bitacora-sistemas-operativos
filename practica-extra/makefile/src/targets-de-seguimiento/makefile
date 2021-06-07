# - con "cat" imprimimos contenido de un archivo
# - el $? se expande con los archivos de timestamp mas reciente que imprime_actualizados
# - imprimimos el contenido de los archivos actualizados/modificados
# es decir su timestamp es más reciente que el del archivo imprime_actualizados
imprime_actualizados: main.c jugadores.c items.c
	cat $?
	touch imprime_actualizados

# Agregamos esta reglas a modo de simular el código principal de arriba
.PHONE: init
init:
	touch {main,jugadores,items}.c

.PHONE: clean
clean:
	rm *.c

.PHONY: watch
watch:
	while true; do $(MAKE) -q || $(MAKE); sleep 1; done
