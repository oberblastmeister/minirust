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
LIBS := -lm -lfl
LEXER_OBJ := $(OBJ)/lexer.o
OBJS += $(LEXER_OBJ)
LEXER := src/lexer.c
LEXER_FLEX := src/lexer.l

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
clean:
	rm -rf $(TARGET)
	rm $(LEXER)

.PHONY: cleandep
cleandep:
	rm -f $(dep)
	
$(TARGET) $(OBJ) $(TEST_OBJ) $(DEP) $(TEST_DEP):
	mkdir -p $@

$(LEXER_OBJ): $(LEXER)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/%.o: $(SRC)/%.c | $(TARGET) $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -MMD -MF $(DEP)/$*.d -c -o $@ $<
	
$(LEXER): $(LEXER_FLEX)
	flex $<
	mv ./lex.yy.c $@

$(TEST_OBJ)/%.o: $(TEST_SRC)/%.c | $(TARGET) $(TEST_OBJ) $(TEST_DEP)
	$(CC) $(CFLAGS) -MMD -MF $(TEST_DEP)/$*.d -c -o $@ $<
		
# dash means don't error if the dep files can't be found
-include $(DEPS)
