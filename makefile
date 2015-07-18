all:
	gcc htbp.c --std=c99 -Wall -Werror -pedantic -o htbp.exe
    
clean:
	rm htbp.exe
    