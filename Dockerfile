FROM ubuntu:24.04
LABEL Description="Build environment"
MAINTAINER Wagner Keller"wagner.keller@gmail.com"
RUN echo "check-certificate = off" >> ~/.wgetrc
RUN DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential wget cmake git python3 python3-pip doxygen graphviz
RUN rm -rf /var/lib/apt/lists/* 
RUN wget https://letsencrypt.org/certs/isrg-root-x1-cross-signed.pem \
&& mkdir -p /etc/ssl/certs/ && cp -r isrg-root-x1-cross-signed.pem /etc/ssl/certs/ 
RUN mkdir /home/gitroot \ 
&& cd /home/gitroot \         
&& git clone https://github.com/KellerW/cfscommstcp.git \
&& cd /home/gitroot/cfscommstcp \
&& rm -rf build && mkdir build \
&& cd build \
&& cmake -DCMAKE_BUILD_TYPE=Release  .. \                
&& make -j8

RUN cd /home/gitroot/cfscommstcp 
WORKDIR /home/
VOLUME ["/home/gitroot"]
RUN useradd -ms /bin/bash tcpcomm
USER tcpcomm
WORKDIR /home/tcpcomm
RUN mkdir /home/tcpcomm/app/  
RUN cp -r /home/gitroot/cfscommstcp/build/ /home/tcpcomm/app/
ENV APP_DIRECTORY /home/tcpcomm/app/build/
EXPOSE 8888/tcp
WORKDIR ${APP_DIRECTORY}
CMD ["/home/tcpcomm/app/build/CFSCOMMSTCP"]