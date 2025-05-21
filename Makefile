# Project Name
NAME = minishell

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
EXT_FLAGS = -lreadline

# Directories
SRC_DIR = src/
LIBFT_DIR = libft/
LEXER = src/lexing/
PARSE = src/parser/
SIGNAL = src/signals/
ENV = src/env/
BUILTIN = src/builtin/
EXEC = src/execution/
SIG = src/signals/

# Source files
SRC = $(LEXER)lex_utils.c $(LEXER)lexer.c $(LEXER)grammar_check.c \
      $(LEXER)expand.c $(LEXER)lex_utils2.c $(LEXER)grammar_check2.c $(LEXER)lexer2.c \
      $(ENV)env.c $(ENV)setup.c $(ENV)freeing.c $(ENV)add.c \
      $(ENV)delete.c $(ENV)print.c $(ENV)search.c $(ENV)update.c \
      $(ENV)print_export.c $(ENV)print_export2.c \
      $(SIG)signal.c \
      $(SRC_DIR)main.c \
      $(EXEC)utils.c $(EXEC)exec.c $(EXEC)cmds.c $(EXEC)error.c \
      $(EXEC)pipe.c $(EXEC)redir.c \
      $(BUILTIN)b_cd.c $(BUILTIN)b_echo.c $(BUILTIN)b_env.c \
      $(BUILTIN)b_exit.c $(BUILTIN)b_export.c $(BUILTIN)b_pwd.c \
      $(BUILTIN)b_unset.c $(BUILTIN)b_export_utils.c \
      $(PARSE)parser.c $(PARSE)parse_utils.c $(PARSE)heredoc.c \
	  $(PARSE)heredoc_utils.c \

# Object files
OBJ = $(SRC:.c=.o)

# Includes
INCLUDES = $(INC_DIR)minishell.h $(LIBFT_DIR)/libft.h

# Library
LIBFT = $(LIBFT_DIR)/libft.a

# Silent compilation commands
COMPILE_MSG = @echo "Compiling..."
LINK_MSG = @echo "Successfully compiled!"
HIDE = @

# Build Executable
$(NAME): $(LIBFT) $(OBJ)
	$(COMPILE_MSG)
	$(HIDE)$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(EXT_FLAGS)
	$(LINK_MSG)

# Silent compilation rule
%.o: %.c $(INCLUDES)
	$(HIDE)$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Compile Libft
$(LIBFT):
	$(HIDE)$(MAKE) -C $(LIBFT_DIR) all --silent

# Clean object files
clean:
	$(HIDE)rm -f $(OBJ)
	$(HIDE)$(MAKE) -C $(LIBFT_DIR) clean --silent
	@echo "Cleaned object files."

# Remove binary and object directory
fclean: clean
	$(HIDE)rm -f $(NAME)
	$(HIDE)rm -rf $(OBJ_DIR)
	$(HIDE)rm -f .minishell_history
	$(HIDE)$(MAKE) -C $(LIBFT_DIR) fclean --silent
	@echo "Fully cleaned project."

# Rebuild everything
re: fclean all

# Default Target
all: $(LIBFT) $(NAME)

.PHONY: clean fclean re all