FROM ubuntu:20.04
LABEL email="2504082957@qq.com"
LABEL version="1.0.0"
RUN  sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list
RUN  apt clean
RUN  apt update
ENV DEBIAN_FRONTEND=noninteractive
ENV LC_ALL=zh_CN.UTF-8
RUN apt install locales && locale-gen zh_CN.UTF-8

RUN \
  apt install -y curl wget git build-essential gcc g++ vim nano cmake python3.8 && \
  apt install -y flex bison && \
  apt install -y clang-format clang-tidy clang-tools clang clangd
#  rm -rf /var/lib/apt/lists/* \


RUN \
    cd / && \
    mkdir llvm && cd llvm && \
    wget https://github.com/llvm/llvm-project/archive/llvmorg-10.0.1.tar.gz  && \
    #    https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-10.0.1.tar.gz
    tar -zxvf llvmorg-10.0.1.tar.gz && \
    cd llvm-project-llvmorg-10.0.1 && \
    mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release --enable-optimized --enable-targets=host-only -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" -DLLVM_TARGETS_TO_BUILD="ARM;X86;AArch64" -DCMAKE_INSTALL_PREFIX=/opt/llvm-10 ../llvm  && \
    make -j4 && \
    make install

RUN \
    cd / && \
    rm -rf /llvm

RUN export PATH="/opt/llvm-10/bin:$PATH"
RUN export LDFLAGS="-L/opt/llvm-10/lib"
RUN export CPPFLAGS="-I/opt/llvm-10/include"
