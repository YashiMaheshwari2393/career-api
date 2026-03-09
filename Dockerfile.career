FROM ubuntu:22.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y g++ cmake libboost-all-dev libssl-dev zlib1g-dev git ca-certificates libasio-dev && apt-get clean
RUN git clone --depth=1 https://github.com/CrowCpp/Crow.git /crow
RUN cd /crow && mkdir build && cd build && cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF && make install
RUN find /usr/local/include -name "*.h" | grep -i crow | head -20
WORKDIR /app
COPY career.cpp .
RUN g++ -std=c++17 -O2 -I/usr/local/include career.cpp -o career_server -lboost_system -lpthread -lssl -lcrypto -lz
FROM ubuntu:22.04 AS runtime
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y libboost-system1.74.0 libssl3 zlib1g && apt-get clean
WORKDIR /app
COPY --from=builder /app/career_server .
EXPOSE 8080
CMD ["./career_server"]
