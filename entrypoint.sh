#!/bin/bash
################################################################################
#
# Scrip Created by Digital Scarcity
#
# Authors: Raphael Gaudreault, Max Gravitt
#
################################################################################

alias cleos='docker-compose exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://keosd:8900'
PUBKEY="EOS7ckzf4BMgxjgNSYV22rtTXga8R9Z4XWVhYp8TBgnBi2cErJ2hn"
PRIVKEY="5JhhMGNPsuU42XXjZ57FcDKvbb7KLrehN65tdTQFrH51uruZLHi"

# Create defauls Wallet
cleos wallet create > /opt/eosio/bin/data-dir/contracts/wallet_private_key.txt
cleos wallet import  --private-key "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"
cleos wallet import --private-key $PRIVKEY
cleos wallet import --private-key "5KA6PDD8KCn1xeetnLqT6taohzTJzh7hAuR6soHjwW6HvFypfnr"
cleos wallet import --private-key "5JkKPTWh7QoTfz5LFcB3EdcWSrUxbFWnD2VjjVUCHRdLYDwfRkK"
cleos wallet import --private-key "5JdJ3c1mSZCJbcuVuXycEqnHiLySzaHgNRSrXuj92XcM9yQtSd7";
cleos wallet import --private-key "5KHaatGPrZRnY2XKQz9whnJjvw3n1UvkQ4FNWWJi9w9zuWB2sro"
cleos wallet import --private-key "5JmQZV4bQj82Z5eL312aYT5yisdQTRT3Y2BQx1h6VBiqht62H6J"
cleos wallet import --private-key "5JpUijnnFMTVXMCxLBaLxuGVbgX8sWiKj9zm3juPyWpr7AHZchn"
cleos wallet import --private-key "5J4E2DcmdfQLsJB3fnLLQo4D6uYKw2UwVK6DGJvmNksKiD4CMHt"

# Create Accounts
cleos create account eosio eosio.token $PUBKEY $PUBKEY
cleos create account eosio eosnames EOS5FjxRzQrhcSnnAmzHb1sxcxzcpz49cENdgSE52rC52jJtj27JW EOS5FjxRzQrhcSnnAmzHb1sxcxzcpz49cENdgSE52rC52jJtj27JW
cleos create account eosio account1 EOS5VAYCfzvgm1zsReqKoBgMc2iqwnQ2nBH9ND6Bnrz7y1hKUV4kh EOS5VAYCfzvgm1zsReqKoBgMc2iqwnQ2nBH9ND6Bnrz7y1hKUV4kh
cleos create account eosio account2 EOS6wjxCRF5F7QYbmHjR8MEn8UqWxhHsypiGJThmd9fGf1QSdtfFt EOS6wjxCRF5F7QYbmHjR8MEn8UqWxhHsypiGJThmd9fGf1QSdtfFt

cleos create account eosio seller EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY
cleos create account eosio seller2 EOS7L8GkLrAjcefNAFptuBL4PFkVVUqyhU4zmeM8xQxffPMAinSYn EOS7L8GkLrAjcefNAFptuBL4PFkVVUqyhU4zmeM8xQxffPMAinSYn
cleos create account eosio seller3 EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY
cleos create account eosio buyer  EOS4x6aSHT3tXCxwwNE8Hx6wtpfdC7rHMPnKq61Bc4rYd9WzAfuQ5 EOS4x6aSHT3tXCxwwNE8Hx6wtpfdC7rHMPnKq61Bc4rYd9WzAfuQ5

eosiocpp -o /opt/eosio/bin/data-dir/contracts/eoscommission/eoscommission.wast /opt/eosio/bin/data-dir/contracts/eoscommission/eoscommission.cpp &&
eosiocpp -g /opt/eosio/bin/data-dir/contracts/eoscommission/eoscommission.abi /opt/eosio/bin/data-dir/contracts/eoscommission/eoscommission.cpp &&

cleos set contract eosnames /opt/eosio/bin/data-dir/contracts/eoscommission


#Add permission to eosnames
cleos push action eosio updateauth '{"account":"eosnames","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS5FjxRzQrhcSnnAmzHb1sxcxzcpz49cENdgSE52rC52jJtj27JW", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"eosnames","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p eosnames

## Set the token contract
cleos wallet import --private-key "5KaN6Tq1i39MmjtWgNCodjWgJ14M1QkavT4EAT5FSMCLStDt424"
cleos wallet import --private-key "5JsvQBxRmCmMHdrSBmq35U9PJ8okt1bLRWW6qs8Bt681VhmDH43"
cleos wallet import --private-key "5J3bExsPhD7cvqWSCHUGqzhQ9UMk97ke4KhK25A4nkDMNCuxVvM"
cleos wallet import --private-key "5KUrFjuadjEsztL3CNPr1YVp4kL1BgYs4i1NRzozQtmKbwBHUb9"


# Generate a temp token contract
cleos create account eosio namestoken EOS7oLEQ5yDXCycnXTzZXxMHbgsJriXJ73aU3m6qZU4SDX8B7udC5 EOS7oLEQ5yDXCycnXTzZXxMHbgsJriXJ73aU3m6qZU4SDX8B7udC5
cleos create account eosio sender EOS4zuMFedA1MvwqLXKS86uSAuQUfehodZ8vwejRcvtNe1LDwGchp EOS4zuMFedA1MvwqLXKS86uSAuQUfehodZ8vwejRcvtNe1LDwGchp
cleos create account eosio receiver EOS6unXBDkG8r65B1TdM6XtLJBCCS6u2N5GmePuVa5W8TrhEUD49d EOS6unXBDkG8r65B1TdM6XtLJBCCS6u2N5GmePuVa5W8TrhEUD49d
cleos create account eosio commission EOS7PXXgn2nUYR1GVVRtfKkFEdUptQ1TsemREPeywknDN6dtEAt7R EOS7PXXgn2nUYR1GVVRtfKkFEdUptQ1TsemREPeywknDN6dtEAt7R

cleos set contract namestoken /opt/eosio/bin/data-dir/contracts/eosio.token -p namestoken
cleos push action namestoken create '["namestoken", "1000000.00 TOK", "intial balance"]' -p namestoken
cleos push action namestoken issue '["namestoken", "1000000.00 TOK"]' -p namestoken
cleos push action namestoken transfer '["namestoken","buyer","1000.00 TOK","initial balance"]' -p namestoken
cleos push action namestoken transfer '["namestoken","seller","1000.00 TOK","initial balance"]' -p namestoken
cleos push action namestoken transfer '["namestoken","account2","1000.00 TOK","initial balance"]' -p namestoken
#Set contract config
cleos push action eosnames setconfig '[1000, "commission", "namestoken", "TOK", 2, "EOS5FjxRzQrhcSnnAmzHb1sxcxzcpz49cENdgSE52rC52jJtj27JW"]' -p eosnames


## GIVE KEY POWER TO CONTRACT AND THEM LIST IT!
#cleos push action eosio updateauth '{"account":"account1","permission":"active","parent":"owner","auth":{"keys":[{"key":"EOS5VAYCfzvgm1zsReqKoBgMc2iqwnQ2nBH9ND6Bnrz7y1hKUV4kh", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"eosnames","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p account1@active
cleos push action eosio updateauth '{"account":"account1","permission":"owner","parent":"","auth":{"keys":[{"key":"EOS5VAYCfzvgm1zsReqKoBgMc2iqwnQ2nBH9ND6Bnrz7y1hKUV4kh", "weight":1}],"threshold":1,"accounts":[{"permission":{"actor":"eosnames","permission":"eosio.code"},"weight":1}],"waits":[]}}' -p account1@owner
cleos push action eosnames addlisting '["seller", "account1", "EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY", "EOS6wkAebX9zaMBvpSFNotvGYdeybUdp9SijDv8BYLvsyzBjNWgWY", "10.00 EOS"]' -p seller

cleos push action eosnames purchase  '[0, "buyer", "EOS4x6aSHT3tXCxwwNE8Hx6wtpfdC7rHMPnKq61Bc4rYd9WzAfuQ5", "EOS4x6aSHT3tXCxwwNE8Hx6wtpfdC7rHMPnKq61Bc4rYd9WzAfuQ5", "12.00 TOK"]' -p buyer
cleos push action namestoken l '["buyer", "eosnames", "12.00 TOK", "0"]' -p buyer

