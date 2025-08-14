SHADERS :=	GUIStaticV1

GLSLANG_VALIDATOR := glslc

SRCS_ROOT := Sources
SHADER_ROOT := Shaders

FRAG_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.frag.spv, $(SHADERS))
VERT_TARGETS := $(patsubst %, $(SHADER_ROOT)/%/Shader.vert.spv, $(SHADERS))
ALL_TARGETS := $(VERT_TARGETS) $(FRAG_TARGETS)

$(SHADER_ROOT)/%/Shader.vert.spv: $(SRCS_ROOT)/Shaders/%/Shader.vert
	@mkdir -p $(dir $@)
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

$(SHADER_ROOT)/%/Shader.frag.spv: $(SRCS_ROOT)/Shaders/%/Shader.frag
	@mkdir -p $(dir $@)
	@$(GLSLANG_VALIDATOR) $(GLSLANG_OPTIONS) -o $@ $<

ERASE_LINE = \033[2K\r

shaders: $(ALL_TARGETS)
	@printf '$(ERASE_LINE)\033[1;32mShaders compiled.\n\033[1;30m'

cleanShaders:
	@printf '$(ERASE_LINE)\033[1;31mCleaning shaders.\033[1;30m'
	@rm -rf $(SHADER_ROOT)
	@printf '$(ERASE_LINE)\033[1;32mShaders cleaned.\n\033[1;30m'

.PHONY: shaders cleanShaders