CFLAGS+=-std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -g
LDLIBS=

TARGET=hase_igel
OBJS=hase_igel.o game.o rand.o

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS) $(TEST_TARGETS) $(addsuffix .o,$(TEST_TARGETS))
