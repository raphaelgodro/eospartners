# VERSION 1.0.0
# AUTHOR: Raphael Gaudreault
# DESCRIPTION: Docker image of eos.io commission contract

FROM eosio/eos-dev
MAINTAINER Raphael Gaudreault <valnnan@gmail.com>
LABEL authors="raphael.gaudreault@eva.coop"

COPY ./entrypoint.sh /
COPY ./contracts /opt/eosio/bin/data-dir/contracts

RUN chmod +x ./entrypoint.sh
