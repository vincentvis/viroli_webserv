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
CLASSES		:= \
				autoindex/Autoindex\
				config/Config\
				config/ConfigParser\
				ipollable/ClientFD\
				ipollable/FileFD\
				ipollable/CgiFD\
				ipollable/IPollable\
				ipollable/PollableFactory\
				ipollable/ServerFD\
				ipollable/Buffer\
				request/CGIRequest\
				server/Location\
				server/Server\
				utils/FileStat\
				utils/Utils\
        		response/HttpStatus\
                cgi/Cgi\
                request/HttpRequest\
                request/Request\
                request/RequestInterface\
                response/Response

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

run_test_config: $(NAME)
	@echo "\033[31;1;4mPINDASERV -- Everything everywhere all the pinda\033[0m"
	./webserv ../website/config/test.config


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
