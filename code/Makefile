#
# Makefile for viroli webserv project
#

NAME		:= webserv
LIB			:= libwebserv.a

CXX			:= c++
CFLAGS		:= -Wall -Werror -Wextra -pedantic -std=c++98

# # # # # # # # # # # # # # # # # # # #
#			  MAKE HELPERS			  #
# # # # # # # # # # # # # # # # # # # #

uniqlist = $(if $1,$(firstword $1) $(call uniqlist,$(filter-out $(firstword $1),$1)))

# # # # # # # # # # # # # # # # # # # #
#			  DIRECTORIES			  #
# # # # # # # # # # # # # # # # # # # #

OBJ_DIR		:= object_files/
SRC_DIR		:= source_files/
INC_DIR		:= includes/

# # # # # # # # # # # # # # # # # # # #
#			  SOURCE_FILES			  #
# # # # # # # # # # # # # # # # # # # #

MAIN			:= $(SRC_DIR)main.cpp

# a list of classes without the .cpp suffix!
CLASSES			:=	config/config

OBJECTS			:= $(CLASSES:%=$(OBJ_DIR)%.o)
HEADERS			:= $(CLASSES:%=$(INC_DIR)%.hpp)

UNIQ_DIRS		:= $(call uniqlist, $(foreach class, $(CLASSES), $(firstword $(subst /, , $(class)))))

# for "-I $(INCLUDE_DIR)/class_directory/"
CPPFLAGS			:=	-I$(INC_DIR)
CPPFLAGS			+= $(foreach dir, $(UNIQ_DIRS), $(join -I$(INC_DIR),$(dir)))


# # # # # # # # # # # # # # # # # # # #
#				  RULES				  #
# # # # # # # # # # # # # # # # # # # #

#all: $(NAME)
all:
	@echo "TODO - add \$$(NAME) rule as dependency"

#$(LIB): $(OBJECTS) $(HEADERS)
#	ar -rcs $(LIB) $(OBJECTS)

$(NAME): $(LIB) $(MAIN)
	@echo $(HEADERS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(MAIN) $(LIB) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS)
	mkdir -p $(OBJ_DIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean:
	@$(MAKE) clean

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re test
