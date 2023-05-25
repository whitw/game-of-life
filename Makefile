OUT=out
CC=g++
NAME=main
OPTION=-lpthread -lncurses

$(OUT): $(NAME).cpp
	$(CC) -o $@ $^ $(OPTION)

.PHONY: run
run: $(OUT)
	./$(OUT)
