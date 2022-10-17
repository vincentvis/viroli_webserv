#
# Makefile for viroli webserv project
#

NAME		:= webserv

CXX			:= c++
CXXFLAGS	:= -Wall -Werror -Wextra -pedantic -g3 -std=c++98

ifdef EXTRA_CFLAGS
	CXXFLAGS += $(EXTRA_CFLAGS)
endif

# # # # # # # # # # # # # # # # # # # #
#			  MAKE HELPERS			  #
# # # # # # # # # # # # # # # # # # # #

uniqlist 	= $(if $1,$(firstword $1) $(call uniqlist,$(filter-out $(firstword $1),$1)))

# # # # # # # # # # # # # # # # # # # #
#			  DIRECTORIES			  #
# # # # # # # # # # # # # # # # # # # #

OBJ_DIR		:= object_files
SRC_DIR		:= source_files
INC_DIR		:= includes

# # # # # # # # # # # # # # # # # # # #
#			  SOURCE_FILES			  #
# # # # # # # # # # # # # # # # # # # #

MAIN		:= $(SRC_DIR)/main.cpp

# a list of classes without the .cpp suffix!
CLASSES		:= config/ConfigParser\
				config/Config\
				server/Location\
				server/Server\
				utils/Utils\
				ipollable/IPollable\
				ipollable/ClientFD\
				ipollable/ServerFD\
				ipollable/FileFD\
				request/CGIRequest\
                request/HttpRequest\
                request/Request\
                request/RequestInterface\
                response/Response\
				autoindex/Autoindex\
                cgi/Exec\
                cgi/Pipes

OBJECTS		:= $(CLASSES:%=$(OBJ_DIR)/%.o)
OBJECTS     += $(OBJ_DIR)/main.o

# These .d files will contain dependencies
DEP			:= $(OBJECTS:%.o=%.d)

HEADERS		:= $(CLASSES:%=$(INC_DIR)/%.hpp)

UNIQ_DIRS	:= $(call uniqlist, $(foreach class, $(CLASSES), $(firstword $(subst /, , $(class)))))

CPPFLAGS	:= -I$(INC_DIR)/
CPPFLAGS	+= $(foreach dir, $(UNIQ_DIRS), $(join -I$(INC_DIR)/,$(dir)))


# # # # # # # # # # # # # # # # # # # #
#				  RULES				  #
# # # # # # # # # # # # # # # # # # # #

$(NAME): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJECTS) -o $(NAME)

# include all .d files
-include $(DEP)

all: $(NAME)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/ $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean:
	@$(MAKE) clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
