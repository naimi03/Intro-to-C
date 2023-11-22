EXECBIN  = life

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)
LIBFLAGS = -lncurses

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -Ofast

.PHONY: all clean

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(OBJECTS)

format:
	clang-format -i -style=file *.[ch]
