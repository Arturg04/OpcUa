# Compiler
CC = cc

# Additional include paths
INC_PATHS = -I "/usr/include/libxml2" -I "/usr/include/libxml2/libxml"

# Compiler flags
CFLAGS = -Wall -Wextra -Werror $(INC_PATHS)

# Source files
SRC = OpcUaService.c OpcServerComm.c read_xml.c

# Object files
OBJ = $(SRC:.c=.o)

# Libraries
LIBS = -lopen62541 -lxml2 -lsqlite3

# Executable name
EXECUTABLE = a.out

# Default target (the first target in the file)
all: $(EXECUTABLE)

# Linking rule
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compilation rule for source files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule to remove object files and executable
clean:
	rm -f $(OBJ) $(EXECUTABLE)
