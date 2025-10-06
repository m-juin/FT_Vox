NAME := FT_Vox

CPP := clang++

HDRS_ROOT := Headers
SRCS_ROOT := Sources
OBJS_ROOT := .Objects

CPPFLAGS := -Werror -Wextra -Wall -std=c++17 -g -IHeaders/ -IExt/ -IExt/freeType/include/freetype2

Libs =

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

HDRS := $(call rwildcard, $(HDRS_ROOT), *.hpp)
SRCS := $(call rwildcard, $(SRCS_ROOT), *.cpp)

SRCS := $(filter-out $(SRCS_ROOT)/Checkers/%, $(SRCS))

Includes :=

OBJS := $(patsubst $(SRCS_ROOT)/%.cpp,$(OBJS_ROOT)/%.o,$(SRCS))

ERASE_LINE = \033[2K\r

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    Libs += -lglfw -lvulkan -LExt/freeType/lib -lfreetype
else
    Libs += -lglfw3 -lvulkan-1 -LExt/freeType/lib -lfreetype
endif

include stb.mk
include FreeType.mk

$(OBJS_ROOT)/%.o: $(SRCS_ROOT)/%.cpp $(HDRS)
	@printf '$(ERASE_LINE)\033[1;37mCompiling \033[1;35m"$<"\033[1;37m into \033[1;35m"$@"\033[1;37m.'
	@$(CPP) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): external createFold shaders $(OBJS)
	@printf '$(ERASE_LINE)\033[1;37mCompiling the executable \033[1;35m${NAME}\033[1;37m...'
	@$(CPP) $(CPPFLAGS) $(OBJS) -o $@ $(Libs)
	@printf '$(ERASE_LINE)\033[1;32mCompilation ended\033[1;30m\n'

external: STB_download DL_FreeType

createFold:
	@printf '$(ERASE_LINE)\033[1;31mCreating object folder...\033[1;30m'
	@mkdir -p $(sort $(dir $(OBJS)))
	@printf '$(ERASE_LINE)\033[1;37mObject folder created at "$(OBJS_ROOT)".\n'

clean:
	@printf '$(ERASE_LINE)\033[1;36mCleaning project sources...\033[1;30m'
	@rm -rf $(OBJS_ROOT)
	@printf '$(ERASE_LINE)\033[1;32mProject sources cleaned.\033[1;30m\n'

include Shaders.mk

checkers:
	@printf '\033[1;33mRunning all Checkers tests...\033[0m\n'
	@for file in $(call rwildcard,$(SRCS_ROOT)/Checkers,*.cpp); do \
		name=$$(basename $$file .cpp); \
		$(CPP) $(CPPFLAGS) $$file -o $(OBJS_ROOT)/$$name $(Libs) || exit 1; \
		$(OBJS_ROOT)/$$name || exit 1; \
		rm -f $(OBJS_ROOT)/$$name; \
	done
	@printf '\033[1;32mAll Checkers finished successfully!\033[0m\n'

fclean: clean cleanShaders STB_clean FTP_Clean
	@printf '$(ERASE_LINE)\033[1;36mCleaning project executable...\033[1;30m'
	@rm -rf $(NAME)
	@printf '$(ERASE_LINE)\033[1;32mProject cleaned.\033[1;30m\n'


.PHONY: all $(NAME) createFold clean fclean external checkers