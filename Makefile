# Project Name
NAME	= minishell

# Compiler and Flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
EXT_FLAGS = -lreadline

# Directories
SRC_DIR = src/
LIBFT_DIR = libft/
TOKEN = src/lexer/
PARSE = src/parsing/
SIGNAL = src/signals/
ENV = src/env/
BUILTIN = src/builtin/
EXEC = src/execution/
SIG = src/signals/
HISTORY = src/history/

# Subdirectories inside src/
SRC_SUBDIRS = lexer parser

# Source files (manually listed)
SRC		= $(TOKEN)free.c $(TOKEN)grammar.c $(TOKEN)tokenize_input.c \
		  $(TOKEN)quotes.c $(TOKEN)quotes2.c \
		  $(PARSE)parsing.c $(PARSE)ast_utils.c $(PARSE)expand_utils.c \
		  $(PARSE)str_expand.c $(PARSE)expand_utils2.c $(PARSE)redir_expand.c \
		  $(ENV)env.c $(ENV)setup.c $(ENV)freeing.c $(ENV)add.c \
		  $(ENV)delete.c $(ENV)print.c $(ENV)search.c $(ENV)update.c \
		  $(ENV)print_export.c \
		  $(SIG)signal.c \
		  $(SRC_DIR)main.c

# Object files
OBJ		= $(SRC:.c=.o)

# Includes
INCLUDES = $(INC_DIR)minishell.h $(LIBFT_DIR)/libft.h

# Library
LIBFT	= $(LIBFT_DIR)/libft.a

# Build Executable
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(EXT_FLAGS)

	# Compile .c files to .o files
%.o: %.c $(INCLUDES)
	@mkdir -p $(OBJ_DIR)
	for dir in $(SRC_SUBDIRS); do mkdir -p $(OBJ_DIR)/$$dir; done
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Compile Libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all

# Clean object files
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

# Remove binary and object directory
fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)
	rm -f .minishell_history
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Default Target
all: $(LIBFT) $(NAME)

.PHONY: clean fclean re all
