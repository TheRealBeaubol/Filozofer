# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/19 23:18:04 by lboiteux          #+#    #+#              #
#    Updated: 2024/03/19 23:18:04 by lboiteux         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 								NAME PROJECT								   #
# **************************************************************************** #

NAME	=	philo

# **************************************************************************** #
# 								  COMMANDS									   #
# **************************************************************************** #

CC					=	@cc
CFLAGS				=	-Wall -Wextra -Werror -g
IFLAGS				=	-I ./includes
MK					=	@mkdir -p
CPT_PHILO		=	$(shell ls -lR srcs | grep -F .c | wc -l)
FILE_PHILO		=	$(shell echo "$(CPT_PHILO)" | bc)
PROJ_CMP_PHILO	=	1
RM					= 	@rm -rf


# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #

# **************************************************************************** #
#                                   PRINTS                                     #
# **************************************************************************** #

# **************************************************************************** #
# 								   SOURCES									   #
# **************************************************************************** #

#SOURCES
SRCS	= 	srcs/main.c \
				srcs/utils.c \
				srcs/init.c \
				srcs/parsing.c \
# OBJECTS

OBJS_DIR	:=	.objs
OBJS		:=	$(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))

all: $(NAME)
$(OBJS_DIR)/%.o: %.c
	$(MK) $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@if [ "$(PROJ_CMP_PHILO)" -ne "$(FILE_PHILO)" ]; then \
		printf " \033[1;38;5;28m [\033[0m$(PROJ_CMP_PHILO)\033[1;38;5;28m/\033[0m$(FILE_PHILO)\033[1;38;5;28m]\t\033[0m$(GREEN)$<$(BLUE)$(RESET)                        \r"; \
	else \
		printf " \033[1;38;5;28m [\033[0m$(PROJ_CMP_PHILO)\033[1;38;5;28m/\033[0m$(FILE_PHILO)\033[1;38;5;28m]\t\033[0m$(GREEN)$<$(BLUE)$(RESET)\n\n"; \
	fi
	@$(eval PROJ_CMP_PHILO=$(shell echo $$(($(PROJ_CMP_PHILO)+1))))

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(IFLAGS) $(OBJS) -o $(NAME)
		@printf "  ✅ \033[1;38;5;120mphilosopher successfully compiled\033[0m\n"
		@printf "  🔄 \033[1;38;5;240mphilosopher is ready to run\033[0m \n"

clean:
		@printf "  👾 \033[1;4;38;5;240m$(NAME)\033[0m   \n  \033[1;38;5;240m└──> 🗑️    \033[1;38;5;255m.o \033[1;38;5;248mhave been deleted$(RESET)\n"
		$(RM) $(OBJS_DIR)

fclean: clean
		$(RM) $(NAME)
		@printf "  \033[1;38;5;240m└──> 🗑️    \033[1;38;5;255mphilo binary \033[1;38;5;248mhas been deleted$(RESET)\n\n"
clear:
	@clear

re:	clear fclean all

.PHONY:	re fclean all clean

.SILENT:
