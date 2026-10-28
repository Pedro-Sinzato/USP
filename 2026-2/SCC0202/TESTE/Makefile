CC     = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g -I. -MMD -MP
TARGET = trabalho
BUILD  = build

# Todos os .c da raiz e de até dois níveis de subpastas
SRCS = $(wildcard *.c */*.c */*/*.c)
OBJS = $(addprefix $(BUILD)/,$(notdir $(SRCS:.c=.o)))
DEPS = $(OBJS:.o=.d)

# Onde o make deve procurar os .c (já que os .o não ficam ao lado deles)
vpath %.c $(sort $(dir $(SRCS)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir $(BUILD)

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(TARGET).exe
	-rmdir $(BUILD)

-include $(DEPS)

.PHONY: all clean
