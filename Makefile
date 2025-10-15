# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mtravez <mtravez@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/25 11:47:30 by mtravez           #+#    #+#              #
#    Updated: 2025/10/15 19:28:24 by mtravez          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls

SRC = ft_ls.c utils.c sort.c flags.c

CFLAGS      = -Wall -Wextra -Werror

SAN_LDFLAGS = -L../LeakSanitizer -llsan -lc -Wno-gnu-include-next -I ../LeakSanitize
FSAN_FLAGS = -fsanitize=undefined -fsanitize-undefined-trap-on-error
OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

LIBFT = libft/libft.a

PURPLE = \033[1;35m
RED = \033[1;31m
NC = \033[0m

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(BONUS_DIR)
	@mkdir -p $(OBJ_DIR)$(ENV_DIR)
	@mkdir -p $(OBJ_DIR)$(PARSE_DIR)
	@mkdir -p $(OBJ_DIR)$(LEXER_DIR)
	@mkdir -p $(OBJ_DIR)$(BUILTIN_DIR)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@printf "$(PURPLE)[ft_ls] Compiled successfuly!!! :D $(NC)\n"

$(LIBFT):
	@make -C libft

$(OBJ_DIR)/%.o: %.c 
	@cc $(CFLAGS) -c $< -o $@

clean:
	@printf "$(RED)Cleaning ft_ls$(NC)\n"
	@/bin/rm -rf $(OBJ_DIR)
	@make clean -C libft

fclean: clean
	@/bin/rm -f $(NAME) $(BONUS)
	@make fclean -C libft

re: fclean all 

.PHONY: all clean fclean re