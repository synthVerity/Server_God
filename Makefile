CC = clang
CFLAGS = -Wall -pedantic -Werror -g -c
LDFLAGS =
GOD_SOURCES = god.c
SERVANT_SOURCES = servant.c
GOD_OBJECTS = $(GOD_SOURCES:.c=.o)
SERVANT_OBJECTS = $(SERVANT_SOURCES:.c=.o)
GOD_EXECUTABLE = god.out
SERVANT_EXECUTABLE = servant.out

all: god servant

.PHONY: all clean god servant

god: $(GOD_EXECUTABLE)

servant: $(SERVANT_EXECUTABLE)

$(GOD_EXECUTABLE): $(GOD_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(SERVANT_EXECUTABLE): $(SERVANT_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

$(GOD_OBJECTS): $(GOD_SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

$(SERVANT_OBJECTS): $(SERVANT_SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(GOD_OBJECTS) $(SERVANT_OBJECTS)
