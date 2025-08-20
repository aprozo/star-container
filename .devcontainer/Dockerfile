# Stage 1: Extract from the STAR image
FROM ghcr.io/star-bnl/star-sw:main-root5-gcc485 as star-source

# Stage 2: Main development container
FROM mcr.microsoft.com/devcontainers/base:ubuntu

# Copy the star-sw directory from the source image
COPY --from=star-source /star-sw/StRoot /star-sw/StRoot
COPY --from=star-source /opt/software/linux-scientific7-x86_64/gcc-4.8.5/root-5.34.38-ffhsnriu5ktoo4tieuku5tbezanz6q5g /opt/software/root-5.34-38

# Modified star-shell script with mounted paths
RUN echo '#!/bin/bash\n\
    docker run -it --rm \
    -v "${HOST_PROJECT_PATH}:/workspaces/star-tutorial" \
    -v "${HOST_PROJECT_PATH}/.star-bashrc:/root/.bashrc" \
    -w /workspaces/star-tutorial \
    --hostname star-sw \
    ghcr.io/star-bnl/star-sw:main-root5-gcc485 \
    "$@"' > /usr/local/bin/star-shell && \
    chmod +x /usr/local/bin/star-shell
