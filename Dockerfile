# ─────────────────────────────────────────────────────────────────────────────
# Stage 1 – Builder
# ─────────────────────────────────────────────────────────────────────────────
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update --fix-missing && apt-get install -y \
    g++ \
    cmake \
    libboost-all-dev \
    libssl-dev \
    zlib1g-dev \
    git \
    ca-certificates \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Download Crow (header-only library)
RUN git clone --depth=1 https://github.com/CrowCpp/Crow.git /crow

WORKDIR /app

COPY kundali.cpp .

RUN g++ -std=c++17 -O2 \
    -I/crow/include \
    kundali.cpp \
    -o kundali_server \
    -lboost_system \
    -lpthread \
    -lssl \
    -lcrypto \
    -lz

# ─────────────────────────────────────────────────────────────────────────────
# Stage 2 – Runtime
# ─────────────────────────────────────────────────────────────────────────────
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update --fix-missing && apt-get install -y \
    libboost-system1.74.0 \
    libssl3 \
    zlib1g \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/kundali_server .

EXPOSE 8080

CMD ["./kundali_server"]
