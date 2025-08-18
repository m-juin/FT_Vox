FILES :=	image \

TARGET := Ext/stb/

URL := https://raw.githubusercontent.com/nothings/stb/master/

EXTOBJS = $(patsubst %, $(TARGET)stb_%.h, $(FILES))

ERASE_LINE = \033[2K\r

# Règle par défaut
.PHONY: all STB_clean STB_download
all: STB_download

# Création du répertoire cible
$(TARGET):
	@mkdir -p $@;

# Téléchargement des fichiers
STB_download: $(TARGET) $(EXTOBJS)

# Pattern pour télécharger chaque fichier
$(TARGET)stb_%.h:
	@printf '$(ERASE_LINE)\033[1;37mDownloading STB \033[1;35m$*.h\033[1;37m...\n'
	@curl -s -L -o $@ "$(URL)stb_$*.h" > /dev/null
	@if [ "$$(cat $@)" = "404: Not Found" ]; then \
	    rm -f $@ > /dev/null; \
	    printf '$(ERASE_LINE)\033[1;31mFailed to download STB \033[1;35m$*\033[1;37m\n'; \
	fi

# Nettoyage
STB_clean:
	@printf '$(ERASE_LINE)\033[1;37mCleaning STB Library...\n'
	@rm -rf $(TARGET)