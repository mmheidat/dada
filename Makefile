# NAME = minishell

# SRC = main.c utils.c parsing.c parsing_utils.c split_without_quotes.c 

# OBJ = $(SRC:.c=.o)

# CC = gcc
# CFLAGS = -Wall -Wextra -Werror -I./includes -I./libft -I/Users/mbamatra/homebrew/opt/readline/include

# LIBFT_DIR = libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all: $(NAME)

# $(NAME): $(OBJ) $(LIBFT)
# 	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L$(LIBFT_DIR) -lft -L/Users/mbamatra/homebrew/opt/readline/lib  -lreadline

# $(LIBFT):
# 	make -C $(LIBFT_DIR) bonus

# clean:
# 	rm -f $(OBJ)
# 	make -C $(LIBFT_DIR) clean

# fclean: clean
# 	rm -f $(NAME)
# 	make -C $(LIBFT_DIR) fclean

# re: fclean all

# .PHONY: all clean fclean re

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asid-ahm <asid-ahm@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/08 17:14:53 by asid-ahm          #+#    #+#              #
#    Updated: 2024/07/22 15:58:13 by asid-ahm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN_B = "\033[1;32m"
RED_BI = "\033[1;3;31m"
BLUE_I = "\033[3;34m"
YELLOW = "\033[0;33m"
RESET = "\033[0m"

CC = cc
CFLAGS =  -g3 -fsanitize=address
# CFLAGS =  -Wall -Wextra -Werror -lreadline
# CFLAGS = -I.

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCDIR = SRC
OBJDIR = BOBJ
NAME = minishell
SOURCES = $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)
	@echo $(GREEN_B)"\n$(NAME) is Compiled.🐢 \n"$(RESET)

$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) -L$(LIBFT_DIR) -lft  -lreadline

$(LIBFT):
	@make -C $(LIBFT_DIR)
	@echo $(YELLOW)".\c"$(RESET)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFT_DIR) clean
	@echo $(YELLOW)"Cleaned"$(RESET)

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo $(YELLOW)"Fully cleaned"$(RESET)
	
valgrind: $(NAME)
	bash check_leaks.sh

re: fclean all

.PHONY: all bonus clean fclean re
