FILES :=	image \

TARGET := Ext/stb/

STB_URL := https://raw.githubusercontent.com/nothings/stb/master/

STB_OBJS = $(patsubst %, $(TARGET)stb_%.h, $(FILES))

ERASE_LINE = \033[2K\r

.PHONY: STB_clean STB_download

$(TARGET):
	@mkdir -p $@;

$(STB_OBJS): | $(TARGET)

$(TARGET)stb_%.h:
	@printf '$(ERASE_LINE)\033[1;37mDownloading STB \033[1;35m$*.h\033[1;37m...\n'
	@curl -s -L -o $@ "$(STB_URL)stb_$*.h" > /dev/null
	@if [ "$$(cat $@)" = "404: Not Found" ]; then \
	    rm -f $@ > /dev/null; \
	    printf '$(ERASE_LINE)\033[1;31mFailed to download STB \033[1;35m$*\033[1;37m\n'; \
	fi

STB_clean:
	@printf '$(ERASE_LINE)\033[1;37mCleaning STB Library...\n'
	@rm -rf $(TARGET)