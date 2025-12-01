.DEFAULT_GOAL := all

EXECUTABLE := FT_Vox

CXX := clang++
HDRS_ROOT := Headers
SRCS_ROOT := Sources
OBJS_ROOT := .Objects

CXXFLAGS := -Werror -Wextra -Wall -std=c++17 -g -IHeaders/ -IExt/ -IExt/freeType/include/freetype2

ERASE_LINE := \033[2K\r

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

HDRS := $(call rwildcard, $(HDRS_ROOT), *.hpp)
SRCS := $(call rwildcard, $(SRCS_ROOT), *.cpp)
SRCS := $(filter-out $(SRCS_ROOT)/Checkers/%, $(SRCS))
SRCS := $(filter-out $(SRCS_ROOT)/Tools/%, $(SRCS))
OBJS := $(patsubst $(SRCS_ROOT)/%.cpp,$(OBJS_ROOT)/%.o,$(SRCS))
OBJS_DIRS := $(sort $(dir $(OBJS)))

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
    Libs := -lglfw -lvulkan -LExt/freeType/lib -lfreetype
	TRACY_EXECUTABLE := $(EXECUTABLE)_Tracy
else
    Libs := -lglfw3 -lvulkan-1 -LExt/freeType/lib -lfreetype
	TRACY_EXECUTABLE := $(EXECUTABLE)_Tracy.exe
endif

MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include Shaders.mk
include stb.mk
include JSONLib.mk
include FreeType.mk
include Logger.mk
include Tracy.mk

Deps := $(JSONLib) $(STB_OBJS) cpl_shader FTP_install logger_dl

.PHONY: $(EXECUTABLE) clean fclean checkers

all: $(EXECUTABLE)

sanitize: CXXFLAGS += -fsanitize=address
sanitize: clean all

debug: CXXFLAGS += -DDEBUG_WORLD
debug: $(EXECUTABLE)
	./$(EXECUTABLE)

$(OBJS_DIRS):
	@mkdir -p $@
	@printf '$(ERASE_LINE)\033[1;37mObject folder created at "$(OBJS_ROOT)"\033[0m\n'

$(OBJS_ROOT)/%.o: $(SRCS_ROOT)/%.cpp $(HDRS) | $(OBJS_DIRS) $(Deps)
	@printf '$(ERASE_LINE)\033[1;37mCompiling \033[1;35m$<\033[1;37m into \033[1;35m$@\033[0m\n'
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJS)
	@printf '$(ERASE_LINE)\033[1;37mLinking executable \033[1;35m$(EXECUTABLE)\033[0m\n'
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(Libs)
	@printf '$(ERASE_LINE)\033[1;32mCompilation ended\033[0m\n'

checkers: $(OBJS)
	@printf '\033[1;33mRunning all Checkers tests...\033[0m\n'
	@mkdir -p $(OBJS_ROOT)
	@for file in $(call rwildcard,$(SRCS_ROOT)/Checkers,*.cpp); do \
		name=$$(basename "$$file" .cpp); \
		echo "Compiling test $$name..."; \
		CHECKER_OBJS=""; \
		for obj in $(OBJS); do \
			if [ "$$obj" != "$(OBJS_ROOT)/Vox.o" ]; then \
				CHECKER_OBJS="$$CHECKER_OBJS $$obj"; \
			fi; \
		done; \
		$(CXX) $(CXXFLAGS) $$CHECKER_OBJS "$$file" -o "$(OBJS_ROOT)/$$name" $(Libs) || { echo "Compilation failed for $$name"; exit 1; }; \
		echo "Running test $$name..."; \
		"$(OBJS_ROOT)/$$name" || { echo "Test failed for $$name"; exit 1; }; \
		rm -f "$(OBJS_ROOT)/$$name"; \
	done
	@printf '\033[1;32mAll Checkers finished successfully!\033[0m\n'

clean:
	@printf '$(ERASE_LINE)\033[1;36mCleaning project sources...\033[0m\n'
	@rm -rf $(OBJS_ROOT)
	@printf '$(ERASE_LINE)\033[1;32mProject sources cleaned.\033[0m\n'

fclean: clean cleanShaders STB_clean FTP_Clean JsonLib_Clean
	@printf '$(ERASE_LINE)\033[1;36mCleaning project executable...\033[0m\n'
	@rm -rf $(EXECUTABLE)
	@printf '$(ERASE_LINE)\033[1;32mProject cleaned.\033[0m\n'


$(TRACY_EXECUTABLE): CXXFLAGS += -DTRACY_ENABLE
# $(TRACY_EXECUTABLE): CXXFLAGS += -DDEBUG_WORLD
$(TRACY_EXECUTABLE): CXXFLAGS += -fsanitize=address
$(TRACY_EXECUTABLE): tracy_lib_bld
$(TRACY_EXECUTABLE): Libs += -LExt/Tracy/lib -lTracyClient
$(TRACY_EXECUTABLE): $(OBJS)
	@printf '$(ERASE_LINE)\033[1;37mLinking Tracy executable \033[1;35m$(TRACY_EXECUTABLE)\033[0m\n'
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(Libs)
	@printf '$(ERASE_LINE)\033[1;32mTracy compilation ended\033[0m\n'

tracy_enabled: $(TRACY_EXECUTABLE)
	./$(TRACY_EXECUTABLE)