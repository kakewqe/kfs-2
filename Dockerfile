FROM ubuntu:22.04

WORKDIR /workdir

RUN dpkg --add-architecture i386
RUN apt update && apt -y install wget unzip nasm make grub-pc-bin xorriso libc6:i386 libstdc++6:i386

RUN mkdir -p /tmp/cross && cd /tmp/cross && wget https://github.com/lordmilko/i686-elf-tools/releases/download/7.1.0/i686-elf-tools-linux.zip \
	&& unzip i686-elf-tools-linux.zip 

ENV PATH="/tmp/cross/bin:$PATH"

CMD ["./run.sh"]