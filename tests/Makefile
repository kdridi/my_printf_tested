CC				=	gcc --coverage -g -g3 -ggdb -fsanitize=address
CFLAGS			=	-W -Wall -Wextra -Werror
CPPFLAGS		=	-I../lib/my -I/usr/local/include

SRC				=	$(wildcard *.c) $(wildcard ../lib/my/*.c)
OBJ				=	$(SRC:.c=.o)

TARGET			=	unit_tests

all				:	$(TARGET)
				./$(TARGET)

$(TARGET)		:	$(OBJ)
				$(CC) -o $@ $^ -lcriterion -luuid

clean			:
				$(RM) $(OBJ)

fclean			:	clean
				$(RM) $(TARGET)
				$(RM) $(OBJ:.o=.gcda)
				$(RM) $(OBJ:.o=.gcno)

re				:	fclean all