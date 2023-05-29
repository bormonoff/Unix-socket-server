FROM gcc:11.3 

RUN apt update && \
    apt install -y \
      python3-pip \
      cmake 

COPY ./src /app/src
COPY CMakeLists.txt /app/

RUN mkdir /app/build && cd /app/build && cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build .

ENTRYPOINT ["/app/build/server"] 