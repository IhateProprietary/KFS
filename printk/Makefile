SRC		=	printk.c

OBJ		=	$(addsuffix .o, $(basename $(SRC)))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) -rcs $@ $(OBJ)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I../include

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
