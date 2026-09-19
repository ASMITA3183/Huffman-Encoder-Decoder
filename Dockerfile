FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y g++ && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app/backend

COPY backend /app/backend
COPY frontend /app/frontend

RUN g++ -std=c++17 \
    server.cpp \
    encoder/HuffmanEncoder.cpp \
    decoder/HuffmanDecoder.cpp \
    tree/HuffmanNode.cpp \
    tree/HuffmanTree.cpp \
    file/FileReader.cpp \
    file/FileWriter.cpp \
    format/HuffmanFileFormat.cpp \
    -o server

CMD ["./server"]