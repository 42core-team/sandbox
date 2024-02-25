FROM alpine:latest

RUN apk --no-cache add \
    bash \
    make \
    gcc

SHELL ["/bin/bash", "-c"]

# Customize the Bash prompt to remove the container ID
RUN echo 'PS1="root@core$ "' > /root/.bashrc

RUN mkdir /core

COPY game /core

COPY *.h /core

COPY con_lib.a /core

CMD ["bash"]
