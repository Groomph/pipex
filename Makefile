# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsanchez <rsanchez@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 16:05:34 by rsanchez          #+#    #+#              #
#    Updated: 2021/11/02 15:41:41 by romain           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = clang

CFLAGS = -Wall -Wextra -Werror #--analyze

LIBFT	= libft

HEADER = includes

DIR_S = sources

DIR_O = temporary

SOURCES = main.c path_args.c pipex.c quote_handler.c \
	  $(LIBFT)/string_split.c $(LIBFT)/str_n_comp.c \
	  $(LIBFT)/substr_free.c $(LIBFT)/strjoin_free.c \
	  $(LIBFT)/string_len.c $(LIBFT)/string_duplicate.c \
	  $(LIBFT)/is_whitespace.c $(LIBFT)/init_zero.c \
	  $(LIBFT)/array_clear.c \
	  $(LIBFT)/get_next_line.c

SOURCESB = main_b.c path_args.c pipex.c redirect_infile.c quote_handler.c \
	  $(LIBFT)/string_split.c $(LIBFT)/str_n_comp.c \
	  $(LIBFT)/substr_free.c $(LIBFT)/strjoin_free.c \
	  $(LIBFT)/string_len.c $(LIBFT)/string_duplicate.c \
	  $(LIBFT)/is_whitespace.c $(LIBFT)/init_zero.c \
	  $(LIBFT)/array_clear.c \
	  $(LIBFT)/get_next_line.c

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))

OBJS = $(addprefix $(DIR_O)/,$(SOURCES:.c=.o))

SRCSB = $(addprefix $(DIR_S)/,$(SOURCESB))

OBJSB = $(addprefix $(DIR_O)/,$(SOURCESB:.c=.o))

all: $(NAME)

bonus: $(OBJSB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJSB)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(DIR_O)/%.o: $(DIR_S)/%.c
	@mkdir -p $(DIR_O)
	@mkdir -p $(DIR_O)/$(LIBFT)
	$(CC) $(CFLAGS) -I $(HEADER) -o $@ -c $<

norme:
	norminette $(HEADER)
	norminette $(DIR_S)

clean:
	rm -rf $(DIR_O)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY:		all clean fclean re bonus norme
