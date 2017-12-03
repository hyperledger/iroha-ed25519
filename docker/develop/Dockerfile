FROM ubuntu:16.04

# Release or Debug
ARG BUILD_TYPE=Release
ARG PARALLELISM=8

RUN apt-get update; \
    apt-get -y upgrade; \
    apt-get -y install --no-install-recommends \
        apt-utils \
        automake libtool \
        libssl-dev \
        lcov gcovr \
        wget curl \
        build-essential g++-5 gcc-5 \
        ccache \
        ca-certificates \
        tar \
        git \
        ssh \
        cppcheck \
        file \
        gzip


# install cmake 3.5.1
RUN git clone https://gitlab.kitware.com/cmake/cmake.git /tmp/cmake; \
    (cd /tmp/cmake ; git checkout 64130a7e793483e24c1d68bdd234f81d5edb2d51); \
    (cd /tmp/cmake ; /tmp/cmake/bootstrap --parallel=${PARALLELISM} --enable-ccache); \
    make -j${PARALLELISM} -C /tmp/cmake; \
    make -C /tmp/cmake install; \
    rm -rf /tmp/cmake

# install gtest
ENV GTEST_VERSION=release-1.8.0
RUN git clone https://github.com/google/googletest /tmp/googletest; \
    (cd /tmp/googletest ; git checkout ${GTEST_VERSION}); \
    cmake -H/tmp/googletest -B/tmp/googletest/build -DCMAKE_BUILD_TYPE=${BUILD_TYPE}; \
    cmake --build /tmp/googletest/build --target install -- -j${PARALLELISM}; \
    rm -rf /tmp/googletest

# install gbenchmark
ENV GBENCHMARK_VERSION=v1.3.0
RUN git clone https://github.com/google/benchmark /tmp/benchmark; \
    (cd /tmp/benchmark ; git checkout ${GBENCHMARK_VERSION}); \
    cmake -H/tmp/benchmark -B/tmp/benchmark/build -DCMAKE_BUILD_TYPE=${BUILD_TYPE}; \
    cmake --build /tmp/benchmark/build --target install -- -j${PARALLELISM}; \
    rm -rf /tmp/benchmark

# install sonar cli
ENV SONAR_CLI_VERSION=3.0.3.778
RUN mkdir -p /opt/sonar; \
    curl -L -o /tmp/sonar.zip https://sonarsource.bintray.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-${SONAR_CLI_VERSION}-linux.zip; \
    unzip -o -d /tmp/sonar-scanner /tmp/sonar.zip; \
    mv /tmp/sonar-scanner/sonar-scanner-${SONAR_CLI_VERSION}-linux /opt/sonar/scanner; \
    ln -s -f /opt/sonar/scanner/bin/sonar-scanner /usr/local/bin/sonar-scanner; \
    rm -rf /tmp/sonar*

# install sonar build wrapper
RUN curl -L -o /tmp/sonar-build-wrapper.zip https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip; \
    unzip -o -d /tmp/sonar-build-wrapper /tmp/sonar-build-wrapper.zip; \
    mv /tmp/sonar-build-wrapper/build-wrapper-linux-x86 /opt/sonar/build-wrapper; \
    ln -s -f /opt/sonar/build-wrapper/build-wrapper-linux-x86-64 /usr/local/bin/sonar-build-wrapper; \
    rm -rf /tmp/sonar*

# install latest valgrind
RUN git clone git://sourceware.org/git/valgrind.git /tmp/valgrind; \
    (cd /tmp/valgrind ; /tmp/valgrind/autogen.sh); \
    (cd /tmp/valgrind ; /tmp/valgrind/configure); \
    make -j${PARALLELISM} -C /tmp/valgrind; \
    make -C /tmp/valgrind install; \
    rm -rf /tmp/valgrind
