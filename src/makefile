COMP     = g++
EDL      = g++
RM       = rm
RMFLAGS  = -f
OUTPUT   = -o
CPPFLAGS = -Wall -std=c++11 #-DMAP
INT      =
INTPATH  = ../src/
REALPATH = ../src/
REAL     = Mere.cpp #$(REALPATH)$(INT:.h=.cpp)
OBJPATH  =
OBJ      = Mere.o #$(OBJPATH)$(INT:.h=.o)
LIBPATH  =
LIBS=-L . -ltp -ltcl -lncurses
LIBPATH=
INCPATH  = #-I
#LIBS     = ../lib/libtp.a -ltcl -lncurse

ECHO  = @echo
EXE   = Heritage
CLEAN = clean
#.SILENT:
.PHONY: $(CLEAN)

$(EXE) : $(OBJ)
	$(EDL) $(OUTPUT) $(EXE) $(OBJ) $(LIBPATH)$(LIBS)

%.o : $(INTPATH)%.cpp
	$(ECHO) "compilation de <$<>"
	$(COMP) $(CPPFLAGS) $(INCPATH) -c $<

Mere.o : Menu.h Outils.h Heure.h

clean :
	$(ECHO) "Nettoyage du repertoire..."
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
