COMP     = g++
EDL      = g++
RM       = rm
RMFLAGS  = -f
OUTPUT   = -o
CPPFLAGS = -Wall -std=c++11 #-DMAP
INTPATH  = ../src/
INT      = Menu.h Heure.h
INTPATH  =../src/
#REAL     = $(REALPATH)$(INT:.h=.cpp)
REALPATH =../src/
OBJ      = $(OBJPATH)$(INT:.h=.o) Mere.o
OBJPATH  =
LIBPATH  = #-L
INCPATH  = #-I
LIBS     = #-lm

ECHO  = @echo
EXE   = Heritage
CLEAN = clean
#.SILENT:
.PHONY: $(CLEAN) tests

$(EXE) : $(OBJ)
	$(EDL) $(OUTPUT) $(EXE) $(OBJ)

%.o : $(INTPATH)%.cpp
	$(ECHO) "compilation de <$<>"
	$(COMP) $(CPPFLAGS) $(INCPATH) -c $<

clean :
	$(ECHO) "Nettoyage du repertoire..."
	$(RM) $(RMFLAGS) $(EXE) $(OBJ) core
