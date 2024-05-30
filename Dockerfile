FROM ubuntu:24.04
LABEL Description="Build environment"
MAINTAINER Wagner Keller"wagner.keller@gmail.com"
RUN echo "check-certificate = off" >> ~/.wgetrc
RUN DEBIAN_FRONTEND=noninteractive
RUN apt-get update 
RUN apt-get install -y build-essential wget cmake libasio-dev git python3 doxygen graphviz
RUN rm -rf /var/lib/apt/lists/* 
RUN wget https://letsencrypt.org/certs/isrg-root-x1-cross-signed.pem \
&& mkdir -p /etc/ssl/certs/ && cp -r isrg-root-x1-cross-signed.pem /etc/ssl/certs/ 
RUN apt update
RUN apt install python3 python3-pip -y
#RUN apt-get update && \
#    apt-get install -y python3-pip && \
#    pip3 install --upgrade pip 
# RUN mkdir /home/gtest \ 
# && cd /home/gtest \         
# && git clone https://github.com/google/googletest.git \
# && cd /home/gtest/googletest \
# && mkdir build \
# && cd build \
# && cmake .. \
# && make -j8\
# && make install
# RUN mkdir /home/crow \ 
# && cd /home/crow \         
# && git clone https://github.com/CrowCpp/Crow.git \
# && cd /home/crow/Crow \
# && mkdir build \
# && cd build \
# && cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF \
# && make -j8 \
# && make install
#RUN mkdir /home/gitroot \      
#&& cd /home/gitroot \        
#&& git clone https://github.com/KellerW/movietheathermanager.git \
#&& cd movietheathermanager \
#&& mkdir docs \
#&& rm -rf build \ 
#&& mkdir build \
#&& cd build \
#&& conan profile detect \
#&& conan install .. -s build_type=Release -s compiler.cppstd=17 --output-folder=. --build missing \
#&& cmake -DCMAKE_BUILD_TYPE=Release  .. \
#&& make -j8

#RUN cd /home/gitroot/movietheathermanager \
#&& doxygen 
#WORKDIR /home/
#VOLUME ["/home/gitroot"]
#RUN useradd -ms /bin/bash mvm
#USER mvm
#WORKDIR /home/mvm
#RUN mkdir /home/mvm/app/  
#RUN cp -r /home/gitroot/movietheathermanager/build/ /home/mvm/app/
#ENV APP_DIRECTORY /home/mvm/app/build/
#EXPOSE 8888/tcp
#WORKDIR ${APP_DIRECTORY}
#CMD ["/home/mvm/app/build/MovieManager"]