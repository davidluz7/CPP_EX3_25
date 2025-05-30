CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

SRC = Game.cpp Player.cpp Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp
GUI_SRC = Gui.cpp
DEMO_MAIN = Demo.cpp
GUI_MAIN = GuiMain.cpp

DEMO_OBJ = $(SRC:.cpp=.o) Gui.o Demo.o
GUI_OBJ = $(SRC:.cpp=.o) Gui.o GuiMain.o

DEMO_EXE = demo
GUI_EXE = gui

all: Main gui

Main: $(DEMO_OBJ)
	$(CXX) $(CXXFLAGS) -o $(DEMO_EXE) $^ -lsfml-graphics -lsfml-window -lsfml-system
	./$(DEMO_EXE)

Demo.o: Demo.cpp Gui.hpp
	$(CXX) $(CXXFLAGS) -c Demo.cpp -o Demo.o

Gui.o: Gui.cpp Gui.hpp
	$(CXX) $(CXXFLAGS) -c Gui.cpp -o Gui.o

GuiMain.o: GuiMain.cpp Gui.hpp
	$(CXX) $(CXXFLAGS) -c GuiMain.cpp -o GuiMain.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

gui: $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -o $(GUI_EXE) $^ -lsfml-graphics -lsfml-window -lsfml-system
	./$(GUI_EXE)

test: Test.cpp $(SRC) Gui.cpp
	$(CXX) $(CXXFLAGS) -o test Test.cpp $(SRC) Gui.cpp -lsfml-graphics -lsfml-window -lsfml-system
	./test

valgrind: $(DEMO_EXE)
	valgrind ./$(DEMO_EXE)

clean:
	rm -f *.o $(DEMO_EXE) $(GUI_EXE) test