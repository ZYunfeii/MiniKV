FROM grpc/cxx
LABEL maintainer="yunfei_z@buaa.edu.cn"
ENV PROJECT_DIR=/server 

COPY ./ $PROJECT_DIR/

RUN apt-get update && apt-get install -y cmake && apt-get install -y build-essential && apt-get clean

WORKDIR $PROJECT_DIR/build

RUN cmake .. && make && cd ..