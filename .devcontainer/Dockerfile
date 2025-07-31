FROM ubuntu:22.04 AS builder

# Get tools, QT dependencies and OpenGL.
ENV DEBIAN_FRONTEND=noninteractive
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
    # QT
    libfontconfig1-dev \
    libfreetype-dev \
    libgtk-3-dev \
    libx11-dev \
    libx11-xcb-dev \
    libxcb-cursor-dev \
    libxcb-glx0-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-shape0-dev \
    libxcb-shm0-dev \
    libxcb-sync-dev \
    libxcb-util-dev \
    libxcb-xfixes0-dev \
    libxcb-xkb-dev \
    libxcb1-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxrender-dev \
    # Additional OpenGL support
    mesa-common-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 as default for the build
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

# Get Qt's source code
# see https://doc.qt.io/qt-6/build-sources.html
# see https://doc.qt.io/qt-6/configure-options.html
WORKDIR /opt
RUN wget -nv https://download.qt.io/archive/qt/6.2/6.2.4/single/qt-everywhere-src-6.2.4.tar.xz \
    && tar -xf qt-everywhere-src-6.2.4.tar.xz \
    && mkdir qt-build

# Configure the build and build
WORKDIR /opt/qt-build
RUN /opt/qt-everywhere-src-6.2.4/configure \
        -prefix /opt/qt-install \
        -release \
        -shared \
        -submodules qtbase \
    && cmake --build . --parallel \
    && cmake --install .

# Final image
FROM ubuntu:22.04

# Install the dependencies needed by Qt and sourcevault.
ENV DEBIAN_FRONTEND=noninteractive
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
    # QT
    libfontconfig1-dev \
    libfreetype-dev \
    libgtk-3-dev \
    libx11-dev \
    libx11-xcb-dev \
    libxcb-cursor-dev \
    libxcb-glx0-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev \
    libxcb-randr0-dev \
    libxcb-render-util0-dev \
    libxcb-shape0-dev \
    libxcb-shm0-dev \
    libxcb-sync-dev \
    libxcb-util-dev \
    libxcb-xfixes0-dev \
    libxcb-xkb-dev \
    libxcb1-dev \
    libxext-dev \
    libxfixes-dev \
    libxi-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxrender-dev \
    # Additional OpenGL support
    mesa-common-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 11 and gcov-11 as default in the final image
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 100 \
 && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100 \
 && update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-11 100

# Copy the compiled Qt libraries from the 'builder' stage
COPY --from=builder /opt/qt-install /opt/qt

# Set environment variables so the system can find Qt
ENV PATH="/opt/qt/bin:$PATH"
ENV LD_LIBRARY_PATH="/opt/qt/lib"
ENV CXX="g++"