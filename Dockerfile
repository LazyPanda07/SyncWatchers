FROM ubuntu:24.04 AS build

RUN apt update && apt install -y cmake git unzip zip gcc g++ wget ninja-build

COPY ./ /opt/SyncWatchers

RUN cd /opt/SyncWatchers && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=bin -G "Ninja" .. && cmake --build . -j && cmake --install .

FROM ubuntu:24.04

ENV HOST=0.0.0.0

ENV LD_LIBRARY_PATH=/opt/SyncWatchers

COPY --from=build /opt/SyncWatchers/build/bin /opt/SyncWatchers

WORKDIR /opt/SyncWatchers
ENTRYPOINT [ "sh", "-c", "./SyncWatchers ${HOST}" ]
