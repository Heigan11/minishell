# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/27 15:40:30 by chilton           #+#    #+#              #
#    Updated: 2021/05/19 18:06:07 by chilton          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= minishell
SRCS 		= minishell.c init_minishell.c \
			read_line.c read_line_utils.c \
			split_line_to_tokens.c get_token.c parse_token.c get_commands.c \
			builtins.c ft_exit.c ft_export.c ft_cd.c \
			utilites.c error_messages.c env_list.c table.c get_cmd_path.c \
			exec_simple_cmd.c exec_utils.c exec_pipe_cmds.c \
			pipe.c redirect.c
OBJS		= $(SRCS:.c=.o)
LIBFT		= ./libft/libft.a
CFLAGS		= -Wall -Werror -Wextra -I$(dir $(LIBFT)) -MMD
CC			= gcc

all:		$(NAME)

$(NAME):	$(OBJS) $(LIBFT)
			gcc  -o $(NAME) -L$(dir $(LIBFT)) -lft -ltermcap $(OBJS)
			@echo ""
			@echo "▂▃▅▇█▓▒░ DONE ░▒▓█▇▅▃▂"
			@echo ""

$(LIBFT):	FORCE
			make bonus -C $(dir $(LIBFT))

FORCE:

-include	$(SRCS:.c=.d)

clean:
			make clean -C $(dir $(LIBFT))
			$(RM) $(OBJS) $(SRCS:.c=.d)

fclean:		clean
			$(RM) $(LIBFT)
			$(RM) $(NAME)

re:			fclean all

run:		$(NAME)
			./$(NAME)

norm:
			norminette *.[ch]

.PHONY: 	all clean fclean re run norm
