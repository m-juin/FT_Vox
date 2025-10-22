SHADERS :=	GUIStaticV1 \
			Voxel \
			TransparentVoxel

GLSLANG_VALIDATOR := glslc

SRCS_ROOT := Sources
SHADER_ROOT := Shaders

STAMP_FILE=.ShaderStamp

FRAG_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.frag.spv, $(SHADERS))
VERT_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.vert.spv, $(SHADERS))
Shaders_Target := $(VERT_TARGETS) $(FRAG_TARGETS)

all: $(Shaders_Target)
	@if test -e $(STAMP_FILE); then \
		printf '$(ERASE_LINE)\033[1;32mShaders compiled.\033[0m\n'; \
		rm -f $(STAMP_FILE); \
	fi

$(SHADER_ROOT)/%/Shader.vert.spv: $(SRCS_ROOT)/Shaders/%/Shader.vert
	@mkdir -p $(dir $@)
	@touch $(STAMP_FILE)
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

$(SHADER_ROOT)/%/Shader.frag.spv: $(SRCS_ROOT)/Shaders/%/Shader.frag
	@mkdir -p $(dir $@)
	@touch $(STAMP_FILE)
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

ERASE_LINE = \033[2K\r

cleanShaders:
	@printf '$(ERASE_LINE)\033[1;31mCleaning shaders.\033[1;30m'
	@rm -rf $(SHADER_ROOT)
	@printf '$(ERASE_LINE)\033[1;32mShaders cleaned.\n\033[1;30m'

.PHONY: cleanShaders