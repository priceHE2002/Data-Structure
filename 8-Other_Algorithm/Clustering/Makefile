# Makefile — 聚类算法实验项目
# 构建: make
# 运行: make run
# 清理: make clean

CXX      = g++
CXXFLAGS = -std=c++11 -O2 -Wall
TARGET   = cluster_experiment
SRC      = src/main.cpp
INCLUDE  = -Iinclude -Idata

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
