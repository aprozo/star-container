FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Install additional tools if needed
RUN apt-get update && apt-get install -y \
    curl \
    wget \
    git \
    && rm -rf /var/lib/apt/lists/*

# Create a script to easily run the STAR container
RUN echo '#!/bin/bash\n\
    docker run -it --rm \
    -v "${HOST_PROJECT_PATH}:/workspaces/star-container" \
    -w /workspaces/star-container \
    --user $(id -u):$(id -g) \
    --hostname star-sw \
    ghcr.io/star-bnl/star-sw:main-root5-gcc485 \
    "$@"' > /usr/local/bin/star-shell && \
    chmod +x /usr/local/bin/star-shell