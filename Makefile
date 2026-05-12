CC := g++
CFLAGS := -Wextra -g --std=c++17 
SRC := $(shell find src debug -name "*.cpp")
OBJ := $(SRC:%.cpp=build/%.o)
TARGET := bin/main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin