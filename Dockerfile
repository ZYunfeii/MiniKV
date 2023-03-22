FROM ubuntu:18.04
LABEL maintainer="yunfei_z@buaa.edu.cn"
ENV PROJECT_DIR=/server
COPY ./build/kvserver $PROJECT_DIR
CMD ./kvserver