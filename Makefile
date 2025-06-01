CC := g++
CFLAGS := -g -Iglad
LDFLAGS := -lglfw -lGL -ldl
TARGET := bin/main

.PHONY: build clean run

run: build
	@./$(TARGET)
	
build:
	@mkdir -p bin
	$(CC) $(CFLAGS) main.cpp glad/glad.c $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

