MAIN_FILES=main.o utils.o
trabalho_pesquisa_operacional_II: $(MAIN_FILES)
	gcc -o trabalho_pesquisa_operacional_II $(MAIN_FILES)

main.o: main.c utils.h
	gcc -c main.c