FROM ubuntu:22.04

# Update and install base packages
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    clang \
    pkg-config \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Rust
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

# Install Starship
RUN curl -sS https://starship.rs/install.sh | sh -s -- --yes

# Configure Starship as default prompt
RUN echo 'eval "$(starship init bash)"' >> /root/.bashrc

# Set default shell to bash
SHELL ["/bin/bash", "-c"]
