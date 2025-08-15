FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Install additional tools if needed
RUN apt-get update && apt-get install -y \
    curl \
    wget \
    git \
    && rm -rf /var/lib/apt/lists/*

# Create directories for extracted container content
RUN mkdir -p /star-container-fs && \
    chown -R vscode:vscode /star-container-fs && \
    chmod -R 755 /star-container-fs

# Create an extraction script
RUN echo '#!/bin/bash\n\
    echo "Extracting STAR container filesystem..."\n\
    CONTAINER_ID=$(docker create ghcr.io/star-bnl/star-sw:main-root5-gcc485)\n\
    docker cp $CONTAINER_ID:/star-sw /star-container-fs 2>/dev/null || true\n\
    docker rm $CONTAINER_ID\n\
    echo "Extraction complete"' > /usr/local/bin/extract-star-fs && \
    chmod +x /usr/local/bin/extract-star-fs

# Execute extraction during build
RUN /usr/local/bin/extract-star-fs

# Create a symlink to the extracted directory
RUN ln -s /star-container-fs/star-sw /star-sw

# Modified star-shell script with mounted paths
RUN echo '#!/bin/bash\n\
    docker run -it --rm \
    -v "${HOST_PROJECT_PATH}:/workspaces/star-container" \
    -w /workspaces/star-container \
    --hostname star-sw \
    ghcr.io/star-bnl/star-sw:main-root5-gcc485 \
    "$@"' > /usr/local/bin/star-shell && \
    chmod +x /usr/local/bin/star-shell