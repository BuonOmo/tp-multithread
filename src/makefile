COMP     = g++
EDL      = g++
RM       = rm
RMFLAGS  = -f
OUTPUT   = -o
CPPFLAGS = -Wall -std=c++11 #-DMAP
INT      = Mere.h GestionClavier.h PorteSortie.h PorteEntree.h
INTPATH  = ../src/
REALPATH = ../src/
REAL     = $(REALPATH)$(INT:.h=.cpp)
OBJPATH  = ../src/
OBJ      = $(OBJPATH)$(INT:.h=.o)
LIBPATH  =
LIBS     = -ltp -ltcl -lncurses
LIBPATH  = -L ../lib/
INCPATH  = #-I
#LIBS     = ../lib/libtp.a -ltcl -lncurse

ECHO  = @echo
EXE   = parking
CLEAN = clean
#.SILENT:
.PHONY: $(CLEAN)

$(EXE) : $(OBJ)
	$(EDL) $(OUTPUT) $(EXE) $(OBJ) $(LIBPATH) $(LIBS)

%.o : $(INTPATH)%.cpp
	$(ECHO) "compilation de <$<>"
	$(COMP) $(CPPFLAGS) $(INCPATH) -c $<

Mere.o : $(INTPATH)Mere.h

clean :
	$(ECHO) "Nettoyage du repertoire..."
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
