ho/player/player.o: player/items.c player/account.c
	@echo "Carpeta:" $(@D)
	@echo "Archivo:" $(@F)

# el codigo de abajo no cuenta para el ejemplo
.PHONY: init
init: crearDirectorios crearArchivos
	@echo "Terminamos de ejecutar las dependencias :)"

crearArchivos:
	touch player/items.c player/account.c
crearDirectorios:
	mkdir player

.PHONY: clean
clean:
	-rm player/*.{c,o}
