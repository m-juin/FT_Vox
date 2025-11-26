LOGGER_URL := https://github.com/m-juin/LoggerLib/archive/refs/tags/
LOGGER_VERSION := 1.1.0
LOGGER_TMP_FOLDER := Temp/LoggerLib
LOGGER_TARGET_FOLDER := Ext/LoggerLib

NPROC := $(shell nproc 2>/dev/null || echo 4)

DOWNLOADER:= curl -L

.PHONY: logger_dl logger_clean

logger_dl: $(LOGGER_TARGET_FOLDER)

$(LOGGER_TARGET_FOLDER):
	@printf "\033[1;37mDownloading Logger lib\033[0m\n"
	@mkdir -p $(LOGGER_TMP_FOLDER)
	@$(DOWNLOADER) -o $(LOGGER_TMP_FOLDER)/$(LOGGER_VERSION).tar.gz $(LOGGER_URL)$(LOGGER_VERSION).tar.gz
	@tar -xvzf $(LOGGER_TMP_FOLDER)/$(LOGGER_VERSION).tar.gz -C $(LOGGER_TMP_FOLDER)
	@mkdir -p $(LOGGER_TARGET_FOLDER)
	@cp $(LOGGER_TMP_FOLDER)/LoggerLib-$(LOGGER_VERSION)/*.hpp $(LOGGER_TARGET_FOLDER)
	@rm -rf $(LOGGER_TMP_FOLDER)

logger_clean:
	@rm -rf $(LOGGER_TARGET_FOLDER)