FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y cmake g++ git libboost-all-dev libasio-dev libssl-dev zlib1g-dev ca-certificates && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY career.cpp .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel

EXPOSE 8080

CMD ["./build/career"]
