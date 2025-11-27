TRACY_VERSION := 0.13.0

TRACY_URL := https://github.com/wolfpld/tracy/archive/refs/tags/v$(TRACY_VERSION).tar.gz

DOWNLOADER := curl -L

TRACY_TMP := Temp/Tracy
TRACY_SOURCE := $(TRACY_TMP)/tracy-$(TRACY_VERSION)

TRACY_TARGET := Ext/Tracy
TRACY_TOOL_TARGET := Tools/Tracy

# Fichiers pour vérifier si la build est nécessaire
TRACY_CMAKE_CACHE := $(TRACY_SOURCE)/build/CMakeCache.txt
TRACY_LIB_TARGET := $(TRACY_TARGET)/lib/libTracyClient.a

.PHONY: tracy_dl tracy_profiler_bld tracy_lib_bld tracy_clean tracy_fclean

TRACY_EXECUTABLE := $(EXECUTABLE)_Tracy.exe

$(TRACY_SOURCE):
	mkdir -p $(TRACY_TMP)
	$(DOWNLOADER) $(TRACY_URL) -o $(TRACY_TMP)/$(TRACY_VERSION).tar.gz
	tar -xvf $(TRACY_TMP)/$(TRACY_VERSION).tar.gz -C $(TRACY_TMP)

tracy_dl: $(TRACY_SOURCE)

$(TRACY_CMAKE_CACHE): $(TRACY_SOURCE)
	mkdir -p $(TRACY_SOURCE)/build
	cd $(TRACY_SOURCE)/build && \
	cmake -DCMAKE_BUILD_TYPE=Release \
	      -DTRACY_NO_FRAME_IMAGE=ON \
	      -DTRACY_NO_CONTEXT_SWITCH=ON \
	      -DCMAKE_SYSTEM_NAME=Windows \
	      ..

$(TRACY_LIB_TARGET): $(TRACY_CMAKE_CACHE)
	cd $(TRACY_SOURCE)/build && \
	make -j$(nproc) TracyClient
	mkdir -p $(TRACY_TARGET)/lib
	mkdir -p $(TRACY_TARGET)/include
	cp $(TRACY_SOURCE)/build/libTracyClient.a $(TRACY_TARGET)/lib/
	cp -r $(TRACY_SOURCE)/public/* $(TRACY_TARGET)/include/

tracy_lib_bld: $(TRACY_LIB_TARGET)

tracy: tracy_lib_bld

tracy_clean:
	rm -rf $(TRACY_TARGET)

tracy_fclean: tracy_clean
	rm -rf $(TRACY_EXECUTABLE)