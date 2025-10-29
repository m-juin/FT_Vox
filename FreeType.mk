FT_URL = https://download.savannah.gnu.org/releases/freetype/
VERSION = 2.13.3
FOLD_NAME = freetype-$(VERSION)
ARCHIVE_NAME = $(FOLD_NAME).tar.xz
FTP_INSTALL_DIR = $(PWD)/Ext/freeType
ERASE_LINE = \033[2K\r

.PHONY: all FTP_Clean DL_FreeType

all: $(FTP_INSTALL_DIR)

$(FTP_INSTALL_DIR):
	@printf '$(ERASE_LINE)\033[1;32mInstalling FreeType...\n\033[1;30m'
	@mkdir -p Temp/freeType
	@curl -L $(FT_URL)$(ARCHIVE_NAME) -o Temp/freeType/$(ARCHIVE_NAME)
	@tar -xvf Temp/freeType/$(ARCHIVE_NAME) -C Temp/freeType
	@cd Temp/freeType/$(FOLD_NAME); \
	./autogen.sh; \
	./configure --prefix="$(FTP_INSTALL_DIR)" --without-harfbuzz --without-brotli; \
	mkdir -p Libs; \
	make -j; \
	make install;
	@rm -rf Temp/
	@printf '$(ERASE_LINE)\033[1;32mFreeType compilation end.\n\033[1;30m'

FTP_Clean:
	@printf '$(ERASE_LINE)\033[1;32mDeleting FreeType installation...\n\033[1;30m'
	@rm -rf $(FTP_INSTALL_DIR)