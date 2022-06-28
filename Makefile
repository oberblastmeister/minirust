# compilation
CC = gcc
# add -I $(SRC) for the generated files
CFLAGS = -Wextra -Wall -std=gnu17 -I $(GEN_INCLUDE) -I $(SRC)
LIBS = -lm -lgmp

# directories
TARGET = ./target
DEP = $(TARGET)/dep
TEST_DEP = $(TARGET)/test_dep
SRC = ./src
TEST_SRC = ./test
OBJ = $(TARGET)/obj
TEST_OBJ = $(TARGET)/test_obj
GEN = $(TARGET)/gen
GEN_SRC = $(GEN)/src
GEN_INCLUDE = $(GEN)/include

# files
EXE = $(TARGET)/lox
LIB = $(TARGET)/liblox.a
TEST_EXE = $(TARGET)/lox_test

# flex and bison
LEXER_OBJ = $(OBJ)/lexer.o
LEXER = $(GEN_SRC)/lexer.c
LEXER_H = $(GEN_INCLUDE)/lexer.h
LEXER_FLEX = $(SRC)/lexer.l
PARSER = $(GEN_SRC)/parser.c
TOKEN_H = $(GEN_INCLUDE)/token.h
PARSER_BISON = $(SRC)/parser.y
PARSER_OBJ = $(OBJ)/parser.o
BISON_FLAGS = --color=always -Wcounterexamples

# multiple files
SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
OBJS += $(LEXER_OBJ)
OBJS += $(PARSER_OBJ)
HEADERS += $(TOKEN_H)
HEADERS += $(LEXER_H)
TEST_SRCS := $(wildcard $(TEST_SRC)/*.c)
TEST_OBJS := $(patsubst $(TEST_SRC)/%.c, $(TEST_OBJ)/%.o, $(TEST_SRCS))
DEPS := $(patsubst $(SRC)/%.c, $(DEP)/%.d, $(SRCS))


.PHONY: default
default: $(EXE)

.PHONY: run
run: $(EXE)
	$(EXE)
	
.PHONY: test
test: $(TEST_EXE)
	$(TEST_EXE)

.PHONY: clean
clean:
	rm -rf $(TARGET)

.PHONY: cleandep
cleandep:
	rm -f $(dep)
	
.PHONY: cleanparser
cleanparser:
	rm $(LEXER)
	rm $(LEXER_H)
	rm $(PARSER)
	rm $(TOKEN_H)
	
.PHONY: check
check:
	cppcheck $(SRC)
	
.PHONY: bisonreport
bisonreport:
	bison $(PARSER_BISON) --html=target/parser_report.html
	rm parser.tab.c
	rm parser.xml
	xdg-open target/parser_report.html


# executable
$(EXE): $(OBJS) | $(TARGET)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)
	
# test executable
$(TEST_EXE): $(TEST_OBJS) $(LIB) | $(TARGET)
	$(CC) $(filter %main.o, $^) -o $@ $(CFLAGS) $(LIBS) -lcriterion -Ltarget -llox
	
# static library
$(LIB): $(OBJS) | $(TARGET)
	ar -rcs $(LIB) $^
	
# create object files
$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) | $(TARGET) $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -MMD -MF $(DEP)/$*.d -c -o $@ $<
	
# bison
$(PARSER_OBJ): $(PARSER) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PARSER) $(TOKEN_H): $(PARSER_BISON) | $(GEN_SRC) $(GEN_INCLUDE)
	bison $(BISON_FLAGS) -o $(PARSER) --header=$(TOKEN_H) $(PARSER_BISON)
	
# flex
$(LEXER_OBJ): $(LEXER) $(PARSER)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LEXER) $(LEXER_H): $(LEXER_FLEX) | $(GEN_SRC) $(GEN_INCLUDE)
	flex --header-file=$(LEXER_H) -o $(LEXER) $(LEXER_FLEX)

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.c | $(TARGET) $(TEST_OBJ) $(TEST_DEP)
	$(CC) $(CFLAGS) -MMD -MF $(TEST_DEP)/$*.d -c -o $@ $<
		
$(TARGET) $(OBJ) $(TEST_OBJ) $(DEP) $(TEST_DEP) $(GEN) $(GEN_INCLUDE) $(GEN_SRC):
	@mkdir -p $@

# dash means don't error if the dep files can't be found
-include $(DEPS)
