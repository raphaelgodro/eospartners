# SETUP

First, build the docker volumes:
```
docker volume create --name=nodeos-data-volume
docker volume create --name=keosd-data-volume
```

Then, start nodeosd daemon testnet
```
docker-compose up -d
```

It may be useful to link a cleos alias within your keosd
```
alias cleos='docker exec commissioncontract_nodeosd_1 cleos -u http://nodeosd:8888'
```

# START AND DEV

To compile the contract and deploy it onchain.

```
docker exec -it commissioncontract_nodeos1 ./entrypoint.sh
```

If you make changes and want to test directly, you can run the restart.sh script
```
./restart.sh
``

