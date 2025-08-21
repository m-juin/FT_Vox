FT_URL = https://download.savannah.gnu.org/releases/freetype/
VERSION = 2.13.3
FOLD_NAME = freetype-$(VERSION)
ARCHIVE_NAME = $(FOLD_NAME).tar.xz
INSTALL_DIR = $(PWD)/Ext/freeType
ERASE_LINE = \033[2K\r

# Utiliser le dossier d'installation comme cible principale

all: DL_FreeType

DL_FreeType: $(INSTALL_DIR)
	@printf '$(ERASE_LINE)\033[1;32mFreeType compilation end.\n\033[1;30m'

$(INSTALL_DIR):
	@printf '$(ERASE_LINE)\033[1;32mInstalling FreeType...\n\033[1;30m'
	@mkdir -p Temp/freeType
	@curl -L $(FT_URL)$(ARCHIVE_NAME) -o Temp/freeType/$(ARCHIVE_NAME)
	@tar -xvf Temp/freeType/$(ARCHIVE_NAME) -C Temp/freeType
	@cd Temp/freeType/$(FOLD_NAME); \
	./autogen.sh; \
	./configure --prefix="$(INSTALL_DIR)" --without-harfbuzz --without-brotli; \
	mkdir -p Libs; \
	make -j; \
	make install;
	@rm -rf Temp/

FTP_Clean:
	@printf '$(ERASE_LINE)\033[1;32mDeleting FreeType installation...\n\033[1;30m'
	@rm -rf $(INSTALL_DIR)

.PHONY: all FTP_Clean DL_FreeType