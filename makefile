# ============================
#   Makefile Proyecto Dijkstra
# ============================

# Compilador
CXX = g++
CXXFLAGS = -Iinclude -IC:/Users/djgam/OneDrive/Documentos/Universidad/SFML-2.5.1/include -std=c++17 -Wall

# Librerías SFML
LDFLAGS = -LC:/Users/djgam/OneDrive/Documentos/Universidad/SFML-2.5.1/lib \
          -lsfml-graphics -lsfml-window -lsfml-system

# Archivos fuente y binario final
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXE = Proyecto_Dijkstra.exe

# Regla principal
all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $(EXE) $(LDFLAGS)

# Compilación de cada .cpp → .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	del /Q src\*.o 2>NUL
	del /Q $(EXE) 2>NUL

# Ejecutar desde terminal
run: all
	./$(EXE)
