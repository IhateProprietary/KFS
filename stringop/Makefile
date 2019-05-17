SRC		=	_memchr.c _memcmp.c _memcpy.c _memmove.c _memset.c			\
			_strchr.c _strcmp.c _strcpy.c _strlen.c

OBJ		=	$(addsuffix .o, $(basename $(SRC)))

NAME	=	libsops.a

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
