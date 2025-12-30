FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive
ARG TZ=America/Chicago


# Install developer tools: compilers, meta-build tools, code modeller, linter,
# auto-formatters, coverage analyzer, and memory analyzer.

RUN apt-get update && apt-get install -y \
    build-essential \
    clang-format clang-tidy clang-tools clangd \
    cmake cmake-format \
    curl \
    gcovr \
    git \
    ninja-build \
    sudo \
    valgrind \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


# Install the mutation testing engine.

RUN curl -1sLf 'https://dl.cloudsmith.io/public/mull-project/mull-stable/setup.deb.sh' | bash
RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y \
    mull-14 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*


# Create the developer user and set as the default user

ARG USER_NAME
RUN useradd $USER_NAME -s /bin/bash -m \
    && echo "$USER_NAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
    && echo "\n# enable git completion features\nsource /usr/share/bash-completion/completions/git" >> /home/$USER_NAME/.bashrc \
    && echo "\n# set LANG to one of the items listed in locale -a\nexport LANG=C.UTF-8" >> /home/$USER_NAME/.bashrc
USER $USER_NAME
