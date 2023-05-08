#-------- philosophers --------#
NAME	=	philo
SRC		=	finish.c init.c main.c print.c routines.c utils.c
OBJ		=	$(SRC:.c=.o)

#---------- other ----------#
CFLAGS	=	-Wall -Wextra -Werror
RM		=	rm -f

#---------- rules ----------#

all: $(NAME) 

%.o: %.c
	@gcc -g $(CFLAGS) -c $(SRC)
	@echo "objetos creados :D"

$(NAME): $(OBJ)
	@gcc -g $(CFLAGS) $(OBJ) -o $(NAME) 
	@echo "philosophers creado :D"

clean:
	@$(RM) $(OBJ)
	@echo "objetos eliminados :D"

fclean: clean
	@$(RM) $(NAME)
	@echo "philosophers eliminado :D"

re: fclean all

.PHONY: all clean fclean re