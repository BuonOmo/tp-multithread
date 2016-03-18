COMP     = g++
EDL      = g++
RM       = rm
RMFLAGS  = -f
OUTPUT   = -o
CPPFLAGS = -Wall -std=c++11 #-DMAP
INT      = Menu.h Heure.h Outils.h GestionClavier.h
INTPATH  =../src/
REALPATH =../src/
REAL     = $(REALPATH)$(INT:.h=.cpp)
OBJPATH  =
OBJ      = Mere.o #$(OBJPATH)$(INT:.h=.o)
LIBPATH  = ../lib/
INCPATH  = #-I
LIBS     = libtp.a

ECHO  = @echo
EXE   = Heritage
CLEAN = clean
#.SILENT:
.PHONY: $(CLEAN)

$(EXE) : $(OBJ)
	$(EDL) $(OUTPUT) $(EXE) $(OBJ)

%.o : $(INTPATH)%.cpp
	$(ECHO) "compilation de <$<>"
	$(COMP) $(CPPFLAGS) $(INCPATH) $(LIBPATH)$(LIBS) -c $<

Outils.o : Outils.h
Heure.o : Heure.h
Mere.o : Outils.h Heure.h

clean :
	$(ECHO) "Nettoyage du repertoire..."
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
