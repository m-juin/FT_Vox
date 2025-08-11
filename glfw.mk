GLFW_VERSION = 3.4
GLFW_ARCHIVE_NAME = $(GLFW_VERSION).tar.gz
GLFW_URL = https://github.com/glfw/glfw/archive/refs/tags/$(GLFW_ARCHIVE_NAME)
GLFW_EXTRACT_DIR = Temp/glfw-$(GLFW_VERSION)

BUILD_DIR = Temp/glfw_build
LIB_DIR = Lib

GLFW_LIB = $(LIB_DIR)/libglfw3.a

ERASE_LINE = \033[2K\r

.PHONY: all deps glfw clean download_glfw

glfw: $(GLFW_LIB)
	@printf '$(ERASE_LINE)\033[37;1mGLFW compilation finished successfully.'

$(GLFW_LIB): $(GLFW_EXTRACT_DIR)
	@cmake -S $(GLFW_EXTRACT_DIR) -B Temp/glfw/build -D GLFW_BUILD_WAYLAND=0 -D GLFW_BUILD_X11=0
	mingw32-make -C Temp/glfw/build

$(GLFW_EXTRACT_DIR):
	@mkdir -p Temp
	@printf '$(ERASE_LINE)\033[37;1mDownloading GLFW $(GLFW_VERSION)...\n'
	@printf '$(GLFW_URL)\n'
	@curl -L $(GLFW_URL) -o Temp/$(GLFW_ARCHIVE_NAME)
	@tar -xzvf Temp/$(GLFW_ARCHIVE_NAME) -C Temp/
	@printf '$(ERASE_LINE)\033[32;1mGLFW sources downloaded and extracted to $@.'

clean:
	@printf '$(ERASE_LINE)\033[31;1mCleaning build artifacts...'
	@rm -rf $(BUILD_DIR) $(LIB_DIR) $(GLFW_EXTRACT_DIR) Temp/$(GLFW_ARCHIVE_NAME) Temp