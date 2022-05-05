FROM itsjustin/cs225-container-nextgen:legacy
ENV TERM xterm-256color
RUN apt-get update
RUN apt-get install -y python3
RUN apt-get install -y python3-pip
RUN pip3 install matplotlib
ENTRYPOINT bash