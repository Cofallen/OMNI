FROM ubuntu:20.04

WORKDIR /YU
COPY . .

RUN #apt-get update && apt-get install -y cmake
#    gcc

# 我考虑下，能否改成环境变量，实现多功能
RUN #apt update && apt install -y cmake

ARG author
RUN chmod +x start.sh
RUN echo "Welcome ${author}"
#CMD [ "./build/debug/demo" ]
# 在容器内编译一次，保障覆盖已修改内容 仔细想了想，太费时间 -- 其实可以先搞一个这样含有camke的镜像。就是内存大点
RUN #cd build/debug/
RUN #make clean -j9
RUN #cd -

#CMD [ "./build/debug/make -j9" ]