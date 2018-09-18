#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/currency.hpp>

#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/crypto.h>
#include <cstring>
#include <functional>

using namespace eosio;
using namespace std;
using std::string;

class eoscomm : public contract
{

  public:
    eoscomm(account_name self) : contract(self) {}

    void apply(const account_name contract, const account_name act);
    void transferReceived(const currency::transfer &transfer, const account_name code);
    // @abi action
    void addpartner(const account_name partner,
                    const uint32_t weightx100);
    // @abi action
    void setconfig(const account_name _token_contract,
                   const string _symbol,
                   const uint8_t _symbol_precision);
    // @abi action
    void close();
    /*
    // @abi action
    void addlisting(const account_name _seller,
                    const account_name _account_for_sale,
                    //const uint64_t      _listing_type,
                    const asset _price);

    // @abi action
    void dellisting(const uint64_t _listing_id);

    // @abi action
    void purchase(const uint64_t _listing_id,
                  const account_name _buyer,
                  const public_key _new_owner_key,
                  const public_key _new_active_key,
                  const asset _pay_amount);

    void apply(const account_name contract, const account_name act);

    void transferReceived(const currency::transfer &transfer, const account_name code);

    // @abi action
    void setconfig(const uint32_t _comm_x100,
                   const account_name _comm_account,
                   const account_name _token_contract,
                   const string _symbol,
                   const uint8_t _symbol_precision,
                   const public_key _holding_pk);
    */

  private:
    // @abi table partners i64
    struct partner
    {
        uint32_t weightx100;
        account_name partner_acct;
        uint32_t issued_time;

        uint64_t primary_key() const { return partner_acct; }
        EOSLIB_SERIALIZE(partner, (weightx100)(partner_acct)(issued_time));
    };

    typedef eosio::multi_index<N(partners), partner> partner_table;

    // @abi table configs i64
    struct config
    {
        uint64_t config_id;
        account_name token_contract;
        symbol_type payment_symbol;
        uint8_t settled;

        uint64_t primary_key() const { return config_id; }
        EOSLIB_SERIALIZE(config, (config_id)(token_contract)(payment_symbol)(settled));
    };typedef eosio::multi_index<N(configs), config> config_table;
    
    void paytoken(const account_name from,
                  const account_name to,
                  const asset token_amount,
                  const string memo)
    {

        config_table config(_self, _self);
        auto itr = config.begin();
        eosio_assert(itr != config.end(), "token contract is not set");

        print("---------- Transfer -----------\n");
        print("Token Contract   : ", name{itr->token_contract}, "\n");
        print("From             : ", name{from}, "\n");
        print("To               : ", name{to}, "\n");
        print("Amount           : ", token_amount, "\n");
        print("Memo             : ", memo, "\n");

        action(
            permission_level{from, N(active)},
            itr->token_contract, N(transfer),
            std::make_tuple(from, to, token_amount, memo))
            .send();

        print("---------- End Transfer -------\n");
    }
    /*
    // @abi table configs i64
    struct config
    {
        uint64_t config_id;
        uint32_t comm_x100;
        account_name comm_account;
        account_name token_contract;
        symbol_type payment_symbol;
        public_key holding_pk;

        uint64_t primary_key() const { return config_id; }
        EOSLIB_SERIALIZE(config, (config_id)(comm_x100)(comm_account)(token_contract)(payment_symbol)(holding_pk));
    };


    typedef eosio::multi_index<N(configs), config> config_table;

    // @abi table listings i64
    struct listing
    {
        uint64_t listing_id;
        account_name seller;
        account_name account_for_sale;
        asset price;
        uint32_t created_date;

        uint64_t primary_key() const { return listing_id; }
        account_name byforsale() const { return account_for_sale; }
        EOSLIB_SERIALIZE(listing, (listing_id)(seller)(account_for_sale)(price)(created_date));
    };

    typedef eosio::multi_index<N(listings), listing,
                               indexed_by<N(forsale),
                                          const_mem_fun<listing, account_name, &listing::byforsale>>>
        listing_table;

    // // @abi table rentals i64
    // struct rental
    // {
    //     uint64_t        rental_id;
    //     account_name    landlord;
    //     public_key      landlord_key;
    //     account_name    account_for_rent;
    //     asset           rent_price;
    //     uint32_t        rental_period;
    //     uint32_t        last_paid_date;
    //     uint32_t        created_date;

    //     uint64_t primary_key() const { return listing_id; }
    //     EOSLIB_SERIALIZE (listing, (rental_id)(landlord)(landlord_key)
    //                         (account_for_rent)(rent_price)(rental_period)(created_date));
    // };

    // typedef eosio::multi_index<N(rentals), rental> rental_table;

    // // @abi table rentrecords i64
    // struct rentrecord
    // {
    //     uint64_t        rentrecord_id;
    //     account_name    renter;
    // }

    // @abi table buyrecords i64
    struct buyrecord
    {
        uint64_t buyrecord_id;
        account_name buyer;
        uint64_t listing_id;
        public_key new_owner_key;
        public_key new_active_key;
        asset pay_amount;

        uint64_t primary_key() const { return buyrecord_id; }
        EOSLIB_SERIALIZE(buyrecord, (buyrecord_id)(buyer)(listing_id)(new_owner_key)(new_active_key)(pay_amount));
    };

    typedef eosio::multi_index<N(buyrecords), buyrecord> buyrecord_table;

    const uint8_t AUCTION = 1;
    const uint8_t OUTRIGHT_SALE = 2;

    struct signup_public_key
    {
        uint8_t type;
        array<unsigned char, 33> data;
    };

    struct permission_level_weight
    {
        permission_level permission;
        weight_type weight;
    };

    struct key_weight
    {
        public_key key;
        weight_type weight;
    };
    struct wait_weight
    {
        uint32_t wait_sec;
        weight_type weight;
    };
    struct permission_o
    {
        account_name actor;
        string permission;
    };
    struct auth_accounts
    {
        vector<permission_o> perms;
        vector<weight_type> weights;
    };

    struct authority
    {
        uint32_t threshold = 0;
        vector<key_weight> keys;
        vector<permission_level_weight> accounts;
        vector<wait_weight> waits;
    };

    struct newaccount
    {
        account_name creator;
        account_name name;
        authority owner;
        authority active;
    };

    struct changekey
    {
        account_name acct;
        string perm;
        string parent;
        authority auth;
    };

    void paytoken(const account_name from,
                  const account_name to,
                  const asset token_amount,
                  const string memo)
    {

        config_table config(_self, _self);
        auto itr = config.begin();
        eosio_assert(itr != config.end(), "token contract is not set");

        print("---------- Transfer -----------\n");
        print("Token Contract   : ", name{itr->token_contract}, "\n");
        print("From             : ", name{from}, "\n");
        print("To               : ", name{to}, "\n");
        print("Amount           : ", token_amount, "\n");
        print("Memo             : ", memo, "\n");

        action(
            permission_level{from, N(active)},
            itr->token_contract, N(transfer),
            std::make_tuple(from, to, token_amount, memo))
            .send();

        print("---------- End Transfer -------\n");
    }

    uint64_t hashkey(const string s)
    {
        std::hash<std::string> hash_fn;
        size_t hash = hash_fn(s);
        return hash;
    }

    void chgownerkey(account_name _acct, public_key _newkey, bool _addcode)
    {
        vector<key_weight> keys = {key_weight{_newkey, 1}};
        authority auth;
        if (_addcode)
        {
            auth = authority{1, keys, {permission_level_weight{permission_level{_self, N(eosio.code)}, 1}}, {}};
        }
        else
        {
            auth = authority{1, keys, {}, {}};
        }

        auto update_auth_payload_2 = make_tuple(_acct, N(owner), N(), auth);

        action(
            permission_level{_acct, N(owner)},
            N(eosio),
            N(updateauth),
            update_auth_payload_2)
            .send();
    }

    void chgactivekey(account_name _acct, public_key _newkey, bool _addcode)
    {
        vector<key_weight> keys = {key_weight{_newkey, 1}};
        authority auth;
        if (_addcode)
        {
            auth = authority{1, keys, {permission_level_weight{permission_level{_self, N(eosio.code)}, 1}}, {}};
        }
        else
        {
            auth = authority{1, keys, {}, {}};
        }

        auto update_auth_payload_1 = make_tuple(_acct, N(active), N(owner), auth);

        action(
            permission_level{_acct, N(owner)},
            N(eosio),
            N(updateauth),
            update_auth_payload_1)
            .send();
    }
    */
};
