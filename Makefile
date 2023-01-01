CXXFLAGS=-std=c++14 -Wall -O2 -I src
CXX=g++
GR_DIR=src/parse
BISON_GEN_DIR=$(GR_DIR)/gen
SRCS=src/main.cpp src/symbol_table.cpp
TARGET_NAME=expression_truthfulness.zip

all: build
	$(CXX) $(CXXFLAGS) \
		-o build/app \
		$(SRCS) \
		$(BISON_GEN_DIR)/expression.lexer.cpp \
		$(BISON_GEN_DIR)/expression.tab.cpp

# Generate parser
$(BISON_GEN_DIR)/%.lexer.cpp $(BISON_GEN_DIR)/%.lexer.hpp: $(GR_DIR)/%.lex $(BISON_GEN_DIR)
	flex $(GR_DIR)/$*.lex
	mv lex.cpp $(BISON_GEN_DIR)/$*.lexer.cpp
	mv lex.hpp $(BISON_GEN_DIR)/$*.lexer.hpp

$(BISON_GEN_DIR)/%.tab.cpp $(BISON_GEN_DIR)/%.tab.hpp: $(GR_DIR)/%.y $(BISON_GEN_DIR)
	bison -d -v $< -o $(BISON_GEN_DIR)/$*.tab.cpp

$(BISON_GEN_DIR):
	mkdir -p $(BISON_GEN_DIR)

build/app: $(SRCS) $(BISON_GEN_DIR)/expression.lexer.cpp $(BISON_GEN_DIR)/expression.tab.cpp
	$(CXX) $(CXXFLAGS) -o build/app $^

bison-codegen: $(BISON_GEN_DIR)/expression.lexer.cpp $(BISON_GEN_DIR)/expression.tab.cpp

compile: build build/app

run: 
	./build/app

build:
	mkdir -p build

dist:
	mkdir -p dist

clean:
	rm -rf build
	rm -rf dist
	rm -rf $(BISON_GEN_DIR)

release: clean bison-codegen dist
	zip dist/$(TARGET_NAME) -r Makefile src
