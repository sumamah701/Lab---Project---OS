# =====================================================
#  Makefile - Thread-pool Based Parallel Task Executor
# =====================================================

CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2
TARGET = threadpool_demo
SRC = main.cpp threadpool.cpp
HEADERS = threadpool.h

# ===========================
# Default build
# ===========================
all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# ===========================
# Run program
# ===========================
run: $(TARGET)
	./$(TARGET)

# ===========================
# Build with ThreadSanitizer
# ===========================
tsan: CXXFLAGS += -fsanitize=thread -g
tsan: clean $(TARGET)

# ===========================
# Build for Valgrind testing
# ===========================
valgrind: CXXFLAGS += -g -O0
valgrind: clean $(TARGET)

# ===========================
# Clean build files
# ===========================
clean:
	rm -f $(TARGET)
