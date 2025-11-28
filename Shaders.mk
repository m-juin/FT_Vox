SHADERS :=	GUIStaticV1 \
			Voxel \
			TransparentVoxel \
			SkyBox

GLSLANG_VALIDATOR := glslc

SHADER_SRCS_ROOT := Sources
SHADER_ROOT := Shaders

STAMP_FILE=.ShaderStamp

FRAG_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.frag.spv, $(SHADERS))
VERT_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.vert.spv, $(SHADERS))
Shaders_Target := $(VERT_TARGETS) $(FRAG_TARGETS)

.PHONY: cleanShaders cpl_shader

cpl_shader: $(Shaders_Target)
	@if test -e $(STAMP_FILE); then \
		printf '$(ERASE_LINE)\033[1;32mShaders compiled.\033[0m\n'; \
		rm -f $(STAMP_FILE); \
	fi

$(SHADER_ROOT)/%/Shader.vert.spv: $(SHADER_SRCS_ROOT)/Shaders/%/Shader.vert
	@mkdir -p $(dir $@)
	@touch $(STAMP_FILE)
	@printf '$(ERASE_LINE)\033[1;37mCompiling shader \033[1;35m$<\033[1;37m into \033[1;35m$@\033[0m\n'
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

$(SHADER_ROOT)/%/Shader.frag.spv: $(SHADER_SRCS_ROOT)/Shaders/%/Shader.frag
	@mkdir -p $(dir $@)
	@touch $(STAMP_FILE)
	@printf '$(ERASE_LINE)\033[1;37mCompiling shader \033[1;35m$<\033[1;37m into \033[1;35m$@\033[0m\n'
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

ERASE_LINE = \033[2K\r

debug_deps:
	@echo "Deps = $(Deps)"
	@echo "JSONLib = $(JSONLib)"
	@echo "STB_OBJS = $(STB_OBJS)"
	@echo "Shaders_Target = $(Shaders_Target)"
	@echo "FTP_install = $(FTP_install)"
	@echo "logger_dl = $(logger_dl)"

cleanShaders:
	@printf '$(ERASE_LINE)\033[1;31mCleaning shaders.\033[1;30m'
	@rm -rf $(SHADER_ROOT) $(STAMP_FILE)
	@printf '$(ERASE_LINE)\033[1;32mShaders cleaned.\n\033[1;30m'