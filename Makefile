# 编译器设置
CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# 目标文件（所有 .cpp 文件编译成 .o 文件）
OBJECTS = Grid1D.o Restrictor.o Prolongator.o Smoother.o \
          VCycle.o FMG.o TestFunction.o JSONParser.o LinearSolver.o main.o

# 可执行文件
TARGET = multigrid

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译各个模块
Grid1D.o: Grid1D.cpp Grid1D.h
	$(CXX) $(CXXFLAGS) -c Grid1D.cpp -o $@

Restrictor.o: Restrictor.cpp Restrictor.h Grid1D.h
	$(CXX) $(CXXFLAGS) -c Restrictor.cpp -o $@

Prolongator.o: Prolongator.cpp Prolongator.h Grid1D.h
	$(CXX) $(CXXFLAGS) -c Prolongator.cpp -o $@

Smoother.o: Smoother.cpp Smoother.h Grid1D.h
	$(CXX) $(CXXFLAGS) -c Smoother.cpp -o $@

VCycle.o: VCycle.cpp VCycle.h Grid1D.h Restrictor.h Prolongator.h Smoother.h LinearSolver.h
	$(CXX) $(CXXFLAGS) -c VCycle.cpp -o $@

FMG.o: FMG.cpp FMG.h Grid1D.h Restrictor.h Prolongator.h Smoother.h VCycle.h LinearSolver.h
	$(CXX) $(CXXFLAGS) -c FMG.cpp -o $@

TestFunction.o: TestFunction.cpp TestFunction.h
	$(CXX) $(CXXFLAGS) -c TestFunction.cpp -o $@

JSONParser.o: JSONParser.cpp JSONParser.h
	$(CXX) $(CXXFLAGS) -c JSONParser.cpp -o $@

LinearSolver.o: LinearSolver.cpp LinearSolver.h
	$(CXX) $(CXXFLAGS) -c LinearSolver.cpp -o $@

main.o: main.cpp Grid1D.h Restrictor.h Prolongator.h Smoother.h VCycle.h FMG.h TestFunction.h JSONParser.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

# 运行程序（使用默认配置文件）
run: $(TARGET)
	./$(TARGET) input.json

# 清理目标文件和可执行文件
clean:
	rm -f $(OBJECTS) $(TARGET)

# 完全清理（包括结果文件）
cleanall: clean
	rm -rf results

# 帮助信息
help:
	@echo "========================================"
	@echo "1D Multigrid Solver - Makefile"
	@echo "========================================"
	@echo "Available targets:"
	@echo "  make           - 编译程序"
	@echo "  make run       - 编译并运行（使用 input.json）"
	@echo "  make clean     - 删除目标文件和可执行文件"
	@echo "  make cleanall  - 删除所有（包括 results 目录）"
	@echo "  make help      - 显示帮助信息"
	@echo ""
	@echo "Example:"
	@echo "  ./multigrid input.json"
	@echo "========================================"

.PHONY: all run clean cleanall help