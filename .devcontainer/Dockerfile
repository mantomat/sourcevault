# for the Dockerfile of this image, see the backup on ssd
FROM ubuntu:25.04

# Install required packages
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
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100 \
 && update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-11 100

# Build and install Qt 6.2.4 (qtbase only)
ENV PATH="/opt/qt/bin:$PATH"
ENV LD_LIBRARY_PATH="/opt/qt/lib"
ENV CXX="g++"

# Install Qt deps
RUN apt-get update && apt-get install -y --no-install-recommends \
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
    libxcb-cursor0 \
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
    libglu1-mesa-dev

# Get Qt's source code
WORKDIR /opt
RUN wget -nv https://download.qt.io/archive/qt/6.2/6.2.4/single/qt-everywhere-src-6.2.4.tar.xz && \
    tar -xf qt-everywhere-src-6.2.4.tar.xz && \
    mkdir qt-build

# Setup CMake
WORKDIR /opt/qt-build
RUN cmake -G Ninja ../qt-everywhere-src-6.2.4/qtbase \
    -DCMAKE_INSTALL_PREFIX=/opt/qt \
    -DCMAKE_BUILD_TYPE=Release \
    -DFEATURE_developer_build=OFF \
    -DQT_BUILD_EXAMPLES=OFF \
    -DQT_BUILD_TESTS=OFF \
    -DFEATURE_optimize_full=ON \
    -DQT_FEATURE_network=ON \
    -DQT_FEATURE_gui=ON \
    -DQT_FEATURE_widgets=ON \
    -DQT_FEATURE_testlib=ON

# Compile the code
RUN ninja -j$(nproc) && ninja install && \
    rm -rf /opt/qt-everywhere-src-6.2.4 /opt/qt-build /opt/qt-everywhere-src-6.2.4.tar.xz

# Needed for JetBrains, otherwise it'll complain \
RUN apt-get update && apt-get install -y libicu-dev
