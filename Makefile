SRC := ./src
OUT := ./out

COMPILER := g++
CFLAGS := -O3

LIBS := -lSDL2main -lSDL2 -lSDL2_Image -lavformat -lavcodec -lavutil -mwindows
OBJS := $(patsubst %.cpp, $(OUT)/%.o, main.cpp player.cpp)

$(OUT)/%.o: $(SRC)/%.cpp
	$(COMPILER) $(CFLAGS) -c $< -o $@ -I $(SRC) $(LIBS)

$(OUT)/WPE.exe: $(OBJS)
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LIBS)

all: $(OUT)/WPE.exe

run: $(OUT)/WPE.exe
	@$(OUT)/WPE.exe

clean:
	rm -f $(OUT)/*.o $(OUT)/WPE.exe