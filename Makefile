CC := g++
CFLAGS := -Wextra -g -O0 --std=c++17 -MMD -MP

SRC := $(shell find src debug -name "*.cpp")
OBJ := $(patsubst %.cpp,build/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

TARGET := bin/main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)

clean:
	rm -rf build bin