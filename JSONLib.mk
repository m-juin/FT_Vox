URL = https://github.com/m-juin/JSONLib/archive/refs/tags/1.0.0.tar.gz

JSONLib = Ext/JsonLib/Json.hpp

ARCHIVE_PATH = Temp/JsonLib.tar.gz

.PHONY: JsonLib_Clean

all: $(JSONLib)

$(JSONLib):
	@printf '$(ERASE_LINE)\033[1;32mRetrieving the JsonLib...\n\033[1;30m'
	@mkdir -p Temp/JsonLib
	@curl -L $(URL) -o $(ARCHIVE_PATH) > /dev/null
	@tar -xvf $(ARCHIVE_PATH) -C Temp/JsonLib
	@mv Temp/JsonLib/* Ext/JsonLib
	@rm -f Ext/JsonLib/.gitignore
	@rm -f Ext/JsonLib/README.md
	@rm -rf Temp/JsonLib
	@rm -rf $(ARCHIVE_PATH)

JsonLib_Clean:
	@printf '$(ERASE_LINE)\033[1;32mDeleting JsonLib...\n\033[1;30m'
	@rm -rf Ext/JsonLib