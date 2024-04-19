FROM ubuntu:20.04
COPY . .
WORKDIR /YU
RUN apt install -y gcc\
    gdb\
    cmake

CMD [ "./start/sh" ]