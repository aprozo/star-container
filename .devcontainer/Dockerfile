# Stage 1: Extract from the STAR image
FROM ghcr.io/star-bnl/star-sw:main-root5-gcc485 as star-source

# Stage 2: Main development container
FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Install additional tools
RUN apt-get update && apt-get install -y \
    curl \
    wget \
    git \
    docker.io \
    && rm -rf /var/lib/apt/lists/*

# Copy the star-sw directory from the source image
COPY --from=star-source /star-sw /star-sw

# Modified star-shell script with mounted paths
RUN echo '#!/bin/bash\n\
    docker run -it --rm \
    -v "${HOST_PROJECT_PATH}:/workspaces/star-container" \
    -w /workspaces/star-container \
    --hostname star-sw \
    ghcr.io/star-bnl/star-sw:main-root5-gcc485 \
    "$@"' > /usr/local/bin/star-shell && \
    chmod +x /usr/local/bin/star-shell