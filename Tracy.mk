TRACY_VERSION := 0.13.0

TRACY_URL := https://github.com/wolfpld/tracy/archive/refs/tags/v$(TRACY_VERSION).tar.gz

DOWNLOADER := curl -L

TRACY_TMP := Temp/Tracy
TRACY_SOURCE := $(TRACY_TMP)/tracy-$(TRACY_VERSION)

TRACY_TARGET := Ext/Tracy
TRACY_TOOL_TARGET := Tools/Tracy

TRACY_CMAKE_CACHE := $(TRACY_SOURCE)/build/CMakeCache.txt
TRACY_LIB_TARGET := $(TRACY_TARGET)/lib/libTracyClient.a

.PHONY: tracy_dl tracy_profiler_bld tracy_lib_bld tracy_clean tracy_fclean

$(TRACY_SOURCE):
	mkdir -p $(TRACY_TMP)
	$(DOWNLOADER) $(TRACY_URL) -o $(TRACY_TMP)/$(TRACY_VERSION).tar.gz
	tar -xvf $(TRACY_TMP)/$(TRACY_VERSION).tar.gz -C $(TRACY_TMP)

tracy_dl: $(TRACY_SOURCE)

# window

# $(TRACY_CMAKE_CACHE): $(TRACY_SOURCE)
# 	mkdir -p $(TRACY_SOURCE)/build
# 	cd $(TRACY_SOURCE)/build && \
# 	cmake -DCMAKE_BUILD_TYPE=Release \
# 	      -DTRACY_NO_FRAME_IMAGE=ON \
# 	      -DTRACY_NO_CONTEXT_SWITCH=ON \
# 	      -DCMAKE_SYSTEM_NAME=Windows \
# 	      ..

#linux

$(TRACY_CMAKE_CACHE): $(TRACY_SOURCE)
	mkdir -p $(TRACY_SOURCE)/build
	cd $(TRACY_SOURCE)/build && \
	cmake -DCMAKE_BUILD_TYPE=Release \
		  -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
	      -DTRACY_NO_FRAME_IMAGE=ON \
	      -DTRACY_NO_CONTEXT_SWITCH=ON \
	      ..

$(TRACY_LIB_TARGET): $(TRACY_CMAKE_CACHE)
	cd $(TRACY_SOURCE)/build && \
	pwd && \
	make -j$(nproc)
	mkdir -p $(TRACY_TARGET)/lib
	mkdir -p $(TRACY_TARGET)/include
	cp $(TRACY_SOURCE)/build/libTracyClient.a $(TRACY_TARGET)/lib/
	cp -r $(TRACY_SOURCE)/public/* $(TRACY_TARGET)/include/

tracy_profiler_bld:
	export HOST_IP=$$(hostname -I | awk '{print $$1}') && \
	docker compose -f $(MAKEFILE_DIR)Tools/TracyDocker/compose.yml up --build

launch_profiler:
	export HOST_IP=$$(hostname -I | awk '{print $$1}') && \
	printf "\033[1;37mConnect to host at address:\033[1;35m $$HOST_IP\033[0m\n"
	docker exec -it tracy_docker /home/Tracy/tracy/profiler/build/tracy-profiler

tracy_lib_bld: $(TRACY_LIB_TARGET)

tracy: tracy_lib_bld

tracy_clean:
	rm -rf $(TRACY_TARGET)

tracy_fclean: tracy_clean
	rm -rf $(TRACY_EXECUTABLE)