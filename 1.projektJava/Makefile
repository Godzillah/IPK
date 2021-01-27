JVM=javac
SRC=*.java 
SERVERCLASS = Server.java

all:
	$(JVM) $(SERVERCLASS)
run: 
	java Server port=$(port)

build: all
rebuild: clean all

clean:
	rm -f *.class
	rm -f output.txt
	rm -f log.*

zip:
	zip xorsak02.zip $(SRC) Makefile copyright ipkDokumentaciaProj1.pdf readme.md    
