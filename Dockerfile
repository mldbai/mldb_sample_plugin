FROM quay.io/datacratic/mldb_sdk:MLDB-756-plugin-sdk

COPY . /build
RUN cd /build && make -j2 -k
