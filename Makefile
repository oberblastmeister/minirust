CC := gcc
TARGET := ./target
DEP := $(TARGET)/dep
TEST_DEP := $(TARGET)/test_dep
SRC := ./src
TEST_SRC := ./test
OBJ := $(TARGET)/obj
TEST_OBJ = $(TARGET)/test_obj
SRCS := $(wildcard $(SRC)/*.c)
TEST_SRCS := $(wildcard $(TEST_SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
TEST_OBJS := $(patsubst $(TEST_SRC)/%.c, $(TEST_OBJ)/%.o, $(TEST_SRCS))
DEPS := $(patsubst $(SRC)/%.c, $(DEP)/%.d, $(SRCS))
EXE := $(TARGET)/lox
LIB := $(TARGET)/liblox.a
TEST_EXE := $(TARGET)/lox_test
CFLAGS := -Wextra -Wall -std=gnu17
LIBS := -lm
LEXER_OBJ := $(OBJ)/lexer.o
OBJS += $(LEXER_OBJ)
LEXER := $(SRC)/lexer.c
LEXER_H := $(SRC)/lexer.h
LEXER_FLEX := $(SRC)/lexer.l
PARSER := $(SRC)/parser.c
TOKEN_H := $(SRC)/token.h
PARSER_BISON := $(SRC)/parser.y
PARSER_OBJ = $(OBJ)/parser.o
OBJS += $(PARSER_OBJ)
BISON_FLAGS := --color=always -Wcounterexamples

.PHONY: default
default: $(EXE)

.PHONY: run
run: $(EXE)
	$(EXE)
	
.PHONY: test
test: $(TEST_EXE)
	$(TEST_EXE)

$(EXE): $(OBJS) | $(TARGET)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)
	
$(TEST_EXE): $(TEST_OBJS) $(LIB) | $(TARGET)
	$(CC) $(filter %main.o, $^) -o $@ $(CFLAGS) $(LIBS) -lcriterion -Ltarget -llox
	
$(LIB): $(OBJS) | $(TARGET)
	ar -rcs $(LIB) $^

.PHONY: clean
clean: cleanparser
	rm -rf $(TARGET)

.PHONY: cleandep
cleandep:
	rm -f $(dep)
	
cleanparser:
	rm $(LEXER)
	rm $(LEXER_H)
	rm $(PARSER)
	rm $(TOKEN_H)
	
$(TARGET) $(OBJ) $(TEST_OBJ) $(DEP) $(TEST_DEP):
	mkdir -p $@

$(LEXER_OBJ): $(LEXER) $(PARSER)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC)/%.c | $(TARGET) $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -MMD -MF $(DEP)/$*.d -c -o $@ $<
	
$(PARSER_OBJ): $(PARSER) $(TOKEN_H)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PARSER) $(TOKEN_H): $(PARSER_BISON)
	bison $(BISON_FLAGS) -o $(PARSER) --header=$(TOKEN_H) $(PARSER_BISON)
	
$(LEXER) $(LEXER_H): $(LEXER_FLEX)
	flex --header-file=$(LEXER_H) -o $(LEXER) $(LEXER_FLEX)

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.c | $(TARGET) $(TEST_OBJ) $(TEST_DEP)
	$(CC) $(CFLAGS) -MMD -MF $(TEST_DEP)/$*.d -c -o $@ $<
		
# dash means don't error if the dep files can't be found
-include $(DEPS)
