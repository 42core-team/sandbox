# Build the server
FROM rust:alpine AS build-server
RUN apk add build-base
WORKDIR /core
COPY ./core .
RUN cargo build --release --bin game

# Build the alpine gcc image
FROM alpine:latest AS alpine-gcc
RUN apk --no-cache add \
	gcc musl-dev \
	make

# Build the library
FROM alpine-gcc AS build-library
WORKDIR /connection
COPY ./bot/connection .
RUN make release

# Create the release image
FROM alpine-gcc AS release
RUN apk --no-cache add \
	git

COPY --from=build-server /core/target/release/game /core/game
COPY --from=build-library /connection/inc/con_lib.h core/con_lib.h
COPY --from=build-library /connection/con_lib.a /core/con_lib.a
