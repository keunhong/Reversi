REVERSI_BIN = reversi
REVERSI_OBJ = AIPlayer.cpp Gameboard.cpp Reversi.cpp

CXX = g++
CXX_OPTS = -g -O0 -Wall

all: $(REVERSI_BIN)

clean:
	@rm *.o $(REVERSI_BIN) 2>/dev/null

$(REVERSI_BIN): $(SQUARE_MAZE_OBJS)
	$(CXX) $(CXX_OPTS) $(REVERSI_OBJ) -o $(REVERSI_BIN)
