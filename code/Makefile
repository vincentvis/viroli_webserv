#
# Makefile for viroli webserv project
#

NAME := webserv
LIB  := libwebserv.a

# # # # # # # # # # # # # # # # # # # #
#				DEFINES				  #
# # # # # # # # # # # # # # # # # # # #

CFLAGS := -Wall -Werror -Wextra -pedantic -std=c++98

CXX := c++

# # # # # # # # # # # # # # # # # # # #
#			  SOURCE_FILES			  #
# # # # # # # # # # # # # # # # # # # #


SOURCE_DIR			:= source_files/

MAIN				:= $(SOURCE_DIR)main.cpp

# a list of classes without the .cpp suffix!
CLASSES				:= config/config
CLASS_FILES			:= $(addprefix $(SOURCE_DIR), $(CLASSES))

OBJECT_DIR			:= object_files/
OBJ					:= $(CLASS_FILES:%=%.o)

INCLUDE_DIR			:= include/
# since each class/.cpp file should be accompanied by a .hpp file we can use
# the same list for that
HEADER_FILES		:= $(addprefix $(INCLUDE_DIR), $(CLASS_FILES:%=%.hpp))

# for "-I $(INCLUDE_DIR)/class_directory/"
CPPFLAGS			:=	-I $(INCLUDE_DIR)\
						-I $(INCLUDE_DIR)config

# # # # # # # # # # # # # # # # # # # #
#				  RULES				  #
# # # # # # # # # # # # # # # # # # # #

all: $(NAME)

$(LIB): $(OBJ) $(HEADER_FILES)
	@echo "LIB"
	@echo $(OBJ)
	@echo $(HEADER_FILES)
	ar -r $(LIB) $(OBJ)

$(NAME): $(LIB) $(MAIN)
	@echo $(HEADER_FILES)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(MAIN) $(LIB) -o $(NAME)

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.cpp $(HEADER_FILES)
	mkdir -p $(OBJ_DIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJECT_DIR)

fclean:
	@$(MAKE) clean

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
