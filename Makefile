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
LEXER = src/lexing/
PARSE = src/parsing/
# PARSE = src/parser/
SIGNAL = src/signals/
ENV = src/env/
BUILTIN = src/builtin/
EXEC = src/execution/
SIG = src/signals/
HISTORY = src/history/

# Source files (manually listed)
SRC		= $(LEXER)lex_utils.c $(LEXER)lexer.c $(LEXER)grammar_check.c $(LEXER)expand.c \
		  $(ENV)env.c $(ENV)setup.c $(ENV)freeing.c $(ENV)add.c \
		  $(ENV)delete.c $(ENV)print.c $(ENV)search.c $(ENV)update.c \
		  $(ENV)print_export.c \
		  $(SIG)signal.c \
		  $(SRC_DIR)main.c \
		  $(EXEC)utils.c $(EXEC)exec.c $(EXEC)cmds.c $(EXEC)error.c \
		  $(EXEC)pipe.c $(EXEC)redir.c \
		  $(BUILTIN)b_cd.c $(BUILTIN)b_echo.c $(BUILTIN)b_env.c \
		  $(BUILTIN)b_exit.c $(BUILTIN)b_export.c $(BUILTIN)b_pwd.c \
		  $(BUILTIN)b_unset.c \
		#   $(PARSE)parsing.c $(PARSE)ast_utils.c $(PARSE)expand_utils.c \
		#   $(PARSE)str_expand.c $(PARSE)expand_utils2.c $(PARSE)redir_expand.c \
		#   $(PARSE)hd_parsing.c \
		#   $(PARSE)parser.c $(PARSE)parse_utils.c \

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
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Compile Libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) all --silent

# Clean object files
clean:
	@rm -f $(OBJ)
	@$(MAKE) -C $(LIBFT_DIR) clean --silent
	@echo "Cleaned object files."

# Remove binary and object directory
fclean: clean
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@rm -f .minishell_history
	@$(MAKE) -C $(LIBFT_DIR) fclean --silent
	@echo "Fully cleaned project."

# Rebuild everything
re: fclean all

# Default Target
all: $(LIBFT) $(NAME)

.PHONY: clean fclean re all
