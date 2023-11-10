FROM ubuntu:18.04

RUN apt-get update && apt install opencl-headers pocl-opencl-icd ocl-icd-opencl-dev gcc g++ -y

WORKDIR /app