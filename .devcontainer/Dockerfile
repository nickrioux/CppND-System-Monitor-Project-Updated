# To fully customize the contents of this image, use the following Dockerfile as a base and add the RUN statement from this file:
# https://github.com/microsoft/vscode-dev-containers/blob/master/containers/debian/.devcontainer/base.Dockerfile
FROM mcr.microsoft.com/vscode/devcontainers/base:debian-9

# This Dockerfile adds a non-root user with sudo access. Use the "remoteUser" property in
# devcontainer.json to use it. More info: https://aka.ms/vscode-remote/containers/non-root-user.
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Install needed packages and tweak non-root user. Use a separate RUN statement to add your own dependencies.
RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && if [ "$USER_GID" != "1000" ] || [ "$USER_UID" != "1000" ]; then \
        groupmod --gid $USER_GID $USERNAME \
        && usermod --uid $USER_UID --gid $USER_GID $USERNAME \
        && chown -R $USER_UID:$USER_GID /home/$USERNAME; \
    fi \
    # Install C++ tools
    && apt-get -y install lldb build-essential cmake cppcheck valgrind clang clang-format libncurses5-dev libncursesw5-dev \
    
    # Install Cmake 3.17
    && cd $HOME \
    && wget https://cmake.org/files/v3.17/cmake-3.17.0-Linux-x86_64.sh \
    && sudo sh cmake-3.17.0-Linux-x86_64.sh --prefix=/usr/local --exclude-subdir \
    && cp /usr/local/bin/* /usr/bin \
    
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# [Optional] Uncomment this section to install additional OS packages.
# RUN apt-get update \
#     && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>
