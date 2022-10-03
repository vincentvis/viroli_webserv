#
# Makefile for viroli webserv project
#

NAME		:= webserv

CXX			:= c++
CXXFLAGS	:= -Wall -Werror -Wextra -pedantic -g3 -std=c++98

ifdef EXTRA_CFLAGS
	CXXFLAGS += $(EXTRA_CFLAGS)
endif

ifdef CXX14
	CXX := /Volumes/Data_v/utils/homebrew/opt/llvm/bin/clang++
endif

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
CLASSES		:= config/ConfigParser\
				config/Config\
				server/Location\
				server/Server\
				utils/Utils\
				ipollable/IPollable\
				ipollable/ServerFD\
				ipollable/ClientFD\
				ipollable/FileFD\
				request/CGIRequest\
				request/HttpRequest\
				request/Request\
				request/RequestInterface\
				response/Response\

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
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJECTS) $(MAIN) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HEADERS)
	mkdir -p $(OBJ_DIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

tidy:
	/Volumes/Data_v/utils/homebrew/opt/llvm/bin/clang-tidy $(CLASSES:%=$(SRC_DIR)%.cpp) -- $(CPPFLAGS)

clean:
	$(RM) -r $(OBJ_DIR)

fclean:
	@$(MAKE) clean
	$(RM) $(NAME)

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re
