FROM ubuntu:25.04 AS builder

# Install build-essential packages and dependencies for compiling Qt
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc-11 g++-11 \
    cmake \
    ninja-build \
    git \
    curl \
    wget \
    unzip \
    python3 \
    xz-utils \
    ca-certificates \
    # Qt Dependencies
    libxrender-dev \
    libxcb-render0-dev \
    libxcb-render-util0-dev \
    libxcb-shape0-dev \
    libxcb-randr0-dev \
    libxcb-xfixes0-dev \
    libxcb-xkb-dev \
    libxcb-sync-dev \
    libxcb-shm0-dev \
    libxcb-icccm4-dev \
    libxcb-keysyms1-dev \
    libxcb-image0-dev \
    libxcb-util-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libxext-dev \
    libx11-dev \
    libxcb1-dev \
    libx11-xcb-dev \
    libsm-dev \
    libice-dev \
    libglib2.0-dev \
    libpthread-stubs0-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 as default for the build
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

# Get Qt's source code
WORKDIR /opt
RUN wget -nv https://download.qt.io/archive/qt/6.2/6.2.4/single/qt-everywhere-src-6.2.4.tar.xz && \
    tar -xf qt-everywhere-src-6.2.4.tar.xz && \
    mkdir qt-build

# Configure, compile, and install Qt
WORKDIR /opt/qt-build
RUN cmake -G Ninja ../qt-everywhere-src-6.2.4/qtbase \
    -DCMAKE_INSTALL_PREFIX=/opt/qt_install \
    -DCMAKE_BUILD_TYPE=Release \
    -DFEATURE_developer_build=OFF \
    -DQT_BUILD_EXAMPLES=OFF \
    -DQT_BUILD_TESTS=OFF \
    -DFEATURE_optimize_full=ON \
    -DQT_FEATURE_network=ON \
    -DQT_FEATURE_gui=ON \
    -DQT_FEATURE_widgets=ON \
    -DQT_FEATURE_testlib=ON && \
    ninja -j$(nproc) && ninja install


FROM ubuntu:25.04

# Install only the RUNTIME dependencies needed by Qt and sourcevault.
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    gcc-11 g++-11 \
    cmake \
    ninja-build \
    git \
    curl \
    wget \
    unzip \
    lcov \
    gcovr \
    clang-tidy \
    clang-format \
    gdb \
    zsh \
    python3 python3-pip \
    xz-utils \
    libicu-dev \
    # Runtime Qt Dependencies
    libxrender1 \
    libxcb-render0 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-randr0 \
    libxcb-xfixes0 \
    libxcb-xkb1 \
    libxcb-sync1 \
    libxcb-shm0 \
    libxcb-icccm4 \
    libxcb-keysyms1 \
    libxcb-image0 \
    libxcb-util1 \
    libxkbcommon0 \
    libxkbcommon-x11-0 \
    libfontconfig1 \
    libfreetype6 \
    libxext6 \
    libx11-6 \
    libxcb1 \
    libx11-xcb1 \
    libsm6 \
    libice6 \
    libglib2.0-0 \
    libgl1 \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 as default in the final image
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100 \
 && update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-11 100

# Copy the compiled Qt libraries from the 'builder' stage
COPY --from=builder /opt/qt_install /opt/qt

# Set environment variables so the system can find Qt
ENV PATH="/opt/qt/bin:$PATH"
ENV LD_LIBRARY_PATH="/opt/qt/lib"
ENV CXX="g++"
