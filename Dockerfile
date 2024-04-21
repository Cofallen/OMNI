FROM ubuntu:20.04
WORKDIR /YU
COPY . .

RUN #apt-get update && apt-get install -y cmake
#    gcc
ARG author
RUN chmod +x start.sh
RUN echo "Welcome ${author}"
#CMD [ "./build/debug/demo" ]
