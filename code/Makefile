#
# Makefile for viroli webserv project
#

NAME		:= webserv

CXX			:= c++
CFLAGS		:= -Wall -Werror -Wextra -pedantic -std=c++98

# # # # # # # # # # # # # # # # # # # #
#			  MAKE HELPERS			  #
# # # # # # # # # # # # # # # # # # # #

uniqlist 	= $(if $1,$(firstword $1) $(call uniqlist,$(filter-out $(firstword $1),$1)))

# # # # # # # # # # # # # # # # # # # #
#			  DIRECTORIES			  #
# # # # # # # # # # # # # # # # # # # #

OBJ_DIR		:= object_files/
SRC_DIR		:= source_files/
INC_DIR		:= includes/

# # # # # # # # # # # # # # # # # # # #
#			  SOURCE_FILES			  #
# # # # # # # # # # # # # # # # # # # #

MAIN		:= $(SRC_DIR)main.cpp

# a list of classes without the .cpp suffix!
CLASSES		:= config/Directive\
				config/DirectiveParam\
				config/ConfigParser

OBJECTS		:= $(CLASSES:%=$(OBJ_DIR)%.o)
HEADERS		:= $(CLASSES:%=$(INC_DIR)%.hpp)

UNIQ_DIRS	:= $(call uniqlist, $(foreach class, $(CLASSES), $(firstword $(subst /, , $(class)))))

CPPFLAGS	:= -I$(INC_DIR)
CPPFLAGS	+= $(foreach dir, $(UNIQ_DIRS), $(join -I$(INC_DIR),$(dir)))

# # # # # # # # # # # # # # # # # # # #
#				  RULES				  #
# # # # # # # # # # # # # # # # # # # #

all: $(NAME)

$(NAME): $(OBJECTS) $(MAIN)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(MAIN) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS)
	mkdir -p $(OBJ_DIR) $(dir $@)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean:
	@$(MAKE) clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
