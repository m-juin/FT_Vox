TRACY_VERSION := 0.13.0

TRACY_URL := https://github.com/wolfpld/tracy/archive/refs/tags/v$(TRACY_VERSION).tar.gz

DOWNLOADER := curl -L

TRACY_TMP := Temp/Tracy

TRACY_TARGET := Ext/Tracy
TRACY_TOOL_TARGET := Tools/Tracy

.PHONY: tracy_dl tracy_profiler_bld tracy_lib_bld

TRACY_EXECUTABLE := $(EXECUTABLE)_Tracy.exe

$(TRACY_TMP)/tracy-$(TRACY_VERSION):
	mkdir -p $(TRACY_TMP)
	$(DOWNLOADER) $(TRACY_URL) -o $(TRACY_TMP)/$(TRACY_VERSION).tar.gz
	tar -xvf $(TRACY_TMP)/$(TRACY_VERSION).tar.gz -C $(TRACY_TMP)

tracy_dl: $(TRACY_TMP)/tracy-$(TRACY_VERSION)

$(TRACY_TARGET)/lib/libTracyClient.a: tracy_dl
	mkdir -p $(TRACY_TMP)/tracy-$(TRACY_VERSION)/build
	cd $(TRACY_TMP)/tracy-$(TRACY_VERSION)/build && \
	cmake -DCMAKE_BUILD_TYPE=Release \
	      -DTRACY_NO_FRAME_IMAGE=ON \
	      -DTRACY_NO_CONTEXT_SWITCH=ON \
	      -DCMAKE_SYSTEM_NAME=Windows \
	      .. && \
	make -j$(nproc) TracyClient
	mkdir -p $(TRACY_TARGET)/lib
	mkdir -p $(TRACY_TARGET)/include
	cp $(TRACY_TMP)/tracy-$(TRACY_VERSION)/build/libTracyClient.a $(TRACY_TARGET)/lib/
	cp -r $(TRACY_TMP)/tracy-$(TRACY_VERSION)/public/* $(TRACY_TARGET)/include/

tracy_lib_bld: $(TRACY_TARGET)/lib/libTracyClient.a

tracy: tracy_lib_bld

tracy_clean:
	rm -rf $(TRACY_TARGET)

tracy_fclean: tracy_clean
	rm -rf $(TRACY_EXECUTABLE)