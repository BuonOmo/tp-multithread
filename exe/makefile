COMP     = g++
EDL      = g++
RM       = rm
RMFLAGS  = -f
OUTPUT   = -o
CPPFLAGS = -Wall -std=c++11 #-DMAP
INT      =
INTPATH  = ../src/
<<<<<<< HEAD
REALPATH = ../src/
REAL     = Mere.cpp #$(REALPATH)$(INT:.h=.cpp)
=======
INT      = Menu.h Heure.h
INTPATH  =../src/
#REAL     = $(REALPATH)$(INT:.h=.cpp)
REALPATH =../src/
OBJ      = $(OBJPATH)$(INT:.h=.o) Mere.o
>>>>>>> 1bd3e387c347d0c2d13f89ab88c70dbac2952a06
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
