.PHONY: cleanall cleanobj cleantxt
cleanall: cleanobj cleantxt
	rm miprograma

cleanobj:
	rm *.o

cleantxt:
	rm *.txt

###################################################
# esto es adicional para simular lo de arriba
crear_archivos: crearobj creartxt
	touch miprograma

crearobj:
	touch archivo{0..5}.o

creartxt:
	touch archivo{0..5}.txt
