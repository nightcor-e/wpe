SRC := ./src
OUT := ./out

COMPILER := gcc
CFLAGS := -O3

LIBS := -lSDL2main -lSDL2 -lSDL2_Image -lavformat -lavcodec -lavutil -mwindows
OBJS := $(patsubst %.c, $(OUT)/%.o, main.c engine.c player.c utils.c)

$(OUT)/%.o: $(SRC)/%.c
	$(COMPILER) $(CFLAGS) -c $< -o $@ -I $(SRC) $(LIBS)

$(OUT)/WPE.exe: $(OBJS)
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LIBS)

all: $(OUT)/WPE.exe

run: $(OUT)/WPE.exe
	@$(OUT)/WPE.exe

clean:
	rm -f $(OUT)/*.o $(OUT)/WPE.exe