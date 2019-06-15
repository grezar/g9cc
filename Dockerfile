FROM ubuntu:18.04

RUN apt update && \
      apt install -y gcc make binutils libc6-dev && \
      apt clean && \
      mkdir /workspace

COPY . /workspace

WORKDIR /workspace

CMD ["make", "test"]
