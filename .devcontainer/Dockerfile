FROM ubuntu:noble

# Update and install base packages
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    clang \
    npm \
    pkg-config \
    libssl-dev \
    libcurl4-openssl-dev \
    && apt remove -y nodejs npm libnode-dev \
    && curl -fsSL https://deb.nodesource.com/setup_22.x | bash - \
    && apt install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

# Install Starship
RUN curl -sS https://starship.rs/install.sh | sh -s -- --yes

# Configure Starship as default prompt
RUN echo 'eval "$(starship init bash)"' >> /root/.bashrc

# Set default shell to bash
SHELL ["/bin/bash", "-c"]
