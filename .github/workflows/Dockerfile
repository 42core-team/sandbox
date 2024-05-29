# Build the server
FROM rust:latest AS build-server
WORKDIR /core
COPY ./core .
RUN cargo build --release --bin game

# Build the library
FROM gcc:latest AS build-library
WORKDIR /connection
COPY ./bot/connection .
RUN make

# Create the release image
FROM alpine:latest AS release
RUN apk --no-cache add \
bash \
make \
gcc \
musl-dev \
git

SHELL ["/bin/bash", "-c"]

# Customize the Bash prompt to remove the container ID
RUN echo 'PS1="root@core$ "' > /root/.bashrc

COPY --from=build-server /core/target/release/game /core/game
COPY --from=build-library /connection/inc/con_lib.h core/con_lib.h
COPY --from=build-library /connection/con_lib.a /core/con_lib.a

CMD ["bash"]
