all:
	gcc htbp.c --std=c99 -Wall -Werror -pedantic -o htbp.exe
	
packed: clean all
	upx htbp.exe
	
clean:
	rm -f htbp.exe
	