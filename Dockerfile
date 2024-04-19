FROM ubuntu:20.04
WORKDIR /YU
COPY . .

RUN chmod +x start.sh

RUN apt-get update && apt-get install -y gcc\
    cmake
RUN ./start.sh

CMD [ "./build/debug/demo" ]
