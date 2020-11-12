CC				=	gcc
CFLAGS			=	-W -Wall -Wextra -Werror

SRC				=	$(wildcard *.c)
OBJ				=	$(SRC:.c=.o)

NAME			=	my
TARGET			=	lib$(NAME).a

all				:	$(TARGET)
				mkdir -p ../../include
				cp $(NAME).h ../../include/$(NAME).h
				cp $(TARGET) ../../$(TARGET)

$(TARGET)		:	$(OBJ)
				$(AR) rsc $@ $^

clean			:
				$(RM) $(OBJ)
				$(RM) $(TARGET)

fclean			:	clean
				$(RM) ../../include/$(NAME).h
				$(RM) ../../$(TARGET)

re				:	fclean all