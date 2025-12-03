FROM debian:trixie

RUN apt upgrade && apt update
RUN apt-get install -y cmake curl git pkg-config libdbus-glib-1-dev libegl-dev libxi-dev libxrandr-dev libxinerama-dev libxcursor-dev libx11-dev libgl1-mesa-dev\
                 libxkbcommon-dev \
                 libgles2-mesa-dev libssl-dev libcapstone-dev clang-19 libwayland-dev

WORKDIR /home/Tracy

RUN git clone https://github.com/wolfpld/tracy.git

WORKDIR /home/Tracy/tracy

RUN git checkout v0.13.0

ENV CC=/usr/bin/clang-19
ENV CXX=/usr/bin/clang++-19
RUN cmake -B profiler/build -S profiler -DCMAKE_BUILD_TYPE=Release -DTRACY_WAYLAND=OFF -DLEGACY=ON
RUN make -C profiler/build -j$(nproc)

EXPOSE 8086

RUN echo '#!/bin/bash' > /showIp.sh
RUN echo 'printf "\033[1;37mConnect to host at address:\033[1;35m $HOST_IP\033[0m\n"' >> /showIp.sh
RUN chmod +x /showIp.sh

CMD ["/bin/bash", "-c", "/showIp.sh && tail -f /dev/null"]

# CMD ["/start-tracy.sh"]