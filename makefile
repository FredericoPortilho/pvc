# Executavel alvo
TARGET = pcv

DIR_OBJ = ./obj/

TARGET_DIR = ./bin/

# Arquivos objetos
OBJS =  \
	./obj/Arquivos.o \
	./obj/Construcao.o \
	./obj/Menus.o \
	./obj/Util.o \
	./obj/main.o \
	./obj/Descida.o
#	./obj/GRASP.o \
#	./obj/AG.o \
#	./obj/BT.o \
#	./obj/ILS.o \
#	./obj/MS.o \
#	./obj/SimulatedAnnealing.o \
#	./obj/VNS.o \ 
#	./obj/Memeticos.o \

REBUILDABLES = $(OBJS) $(DIR_OBJ)*.dep $(TARGET_DIR)$(TARGET)

#Apaga os objs e o exec
clean :
	rm -f $(REBUILDABLES) 
	@echo Clean done

#all compila o alvo
all : $(TARGET)
	@echo All done

# $@ expands to the rule's target, in this case "ParticaoNumeros".
# $^ expands to the rule's dependencies, in this case the obj files
$(TARGET) : $(OBJS)
	g++ -O3 -o $(TARGET_DIR)$@ $^

# Regra para compilar os .cpp e gerar os .o
# $@ for the pattern-matched target
# $< for the pattern-matched dependency
$(DIR_OBJ)%.o : %.cpp
	g++ -O3 -o $@ -c $<

# Alternatively to manually capturing dependencies, several automated
# dependency generators exist.  Here is one possibility (commented out)...
$(DIR_OBJ)%.dep : %.cpp
	g++ -M $(FLAGS) $< > $@
include $(OBJS:.o=.dep)