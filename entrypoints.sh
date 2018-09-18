#!/bin/bash
################################################################################
#
#
# Authors: Raphael Gaudreault
#
################################################################################

alias cleos='docker-compose exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://keosd:8900'

PUBKEY="EOS7ckzf4BMgxjgNSYV22rtTXga8R9Z4XWVhYp8TBgnBi2cErJ2hn"
PRIVKEY="5JhhMGNPsuU42XXjZ57FcDKvbb7KLrehN65tdTQFrH51uruZLHi"

EOSIO_PRIVATE_KEY="5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"

# Create defauls Wallet
cleos wallet create > /opt/eosio/bin/data-dir/contracts/wallet_private_key.txt
cleos wallet import --private-key $EOSIO_PRIVATE_KEY
cleos wallet import --private-key $PRIVKEY
cleos wallet import --private-key "5Jq3vpw8JPp2wbTkLCnsUwhVoHM4UYFmwGscRNCj8tb9mmV9taf" # max
cleos wallet import --private-key "5HxYzVNWDXh5EERW5bWiUmGXPPEw5DqXVSPL7gjn4kKyMgsJQTx" #raph

# Create Accounts
cleos create account eosio eoscomm $PUBKEY $PUBKEY
cleos create account eosio cooltoken $PUBKEY $PUBKEY

cleos create account eosio max EOS7fvh63TtMiyPzttFuNcSpgvFj72wmC9cA446PJEVWQ6k3rxFto EOS7fvh63TtMiyPzttFuNcSpgvFj72wmC9cA446PJEVWQ6k3rxFto
cleos create account eosio raph  EOS6bt6cUTRQGSvtEv3tBqdGQnfMZL5YiG1b6k6KVUULgG484v1PG EOS6bt6cUTRQGSvtEv3tBqdGQnfMZL5YiG1b6k6KVUULgG484v1PG
cleos create account eosio bob  EOS6bt6cUTRQGSvtEv3tBqdGQnfMZL5YiG1b6k6KVUULgG484v1PG EOS6bt6cUTRQGSvtEv3tBqdGQnfMZL5YiG1b6k6KVUULgG484v1PG


eosiocpp -o /opt/eosio/bin/data-dir/contracts/eoscomm/eoscomm.wast /opt/eosio/bin/data-dir/contracts/eoscomm/eoscomm.cpp &&
eosiocpp -g /opt/eosio/bin/data-dir/contracts/eoscomm/eoscomm.abi /opt/eosio/bin/data-dir/contracts/eoscomm/eoscomm.cpp &&

cleos set contract eoscomm /opt/eosio/bin/data-dir/contracts/eoscomm


#Add permission to eoscomm
cleos push action eosio updateauth '{"account":"eoscomm","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS7ckzf4BMgxjgNSYV22rtTXga8R9Z4XWVhYp8TBgnBi2cErJ2hn", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"eoscomm","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p eoscomm

cleos set contract cooltoken /opt/eosio/bin/data-dir/contracts/eosio.token -p cooltoken
cleos push action cooltoken create '["cooltoken", "100000.00 TOK", "intial balance"]' -p cooltoken
cleos push action cooltoken issue '["cooltoken", "100000.00 TOK"]' -p cooltoken
cleos push action cooltoken transfer '["cooltoken","max","100.00 TOK","initial balance"]' -p cooltoken
cleos push action cooltoken transfer '["cooltoken","raph","100.00 TOK","initial balance"]' -p cooltoken

cleos push action eoscomm setconfig '["cooltoken", "TOK", 2]' -p eoscomm

cleos push action eoscomm addpartner '["raph", 40]' -p eoscomm
cleos push action eoscomm addpartner '["max", 60]' -p eoscomm

cleos push action cooltoken transfer '["cooltoken","eoscomm","100.00 TOK","transfer shit"]' -p cooltoken
cleos push action eoscomm close '[]' -p eoscomm

cleos push action eoscomm addpartner '["bob", 10]' -p eoscomm

