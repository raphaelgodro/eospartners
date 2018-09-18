#include <eoscomm.hpp>

#include <eosiolib/action.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace std;
using namespace eosio;

void eoscomm::apply(const account_name contract, const account_name act)
{

    if (act == N(transfer))
    {
        transferReceived(unpack_action_data<currency::transfer>(), contract);
        return;
    }

    auto &thiscontract = *this;

    switch (act)
    {
        EOSIO_API(eoscomm, (addpartner)(setconfig)(close))
    };
}

void eoscomm::transferReceived(const currency::transfer &transfer, const account_name code)
{
    if (transfer.to != _self)
    {
        return;
    }

    print("Account Name     :   ", name{code}, "\n");
    print("From             :   ", name{transfer.from}, "\n");
    print("To               :   ", name{transfer.to}, "\n");
    print("Asset            :   ", transfer.quantity, "\n");
    print("Received Amount  :   ", transfer.quantity.amount, "\n");
    print("Received Symbol  :   ", transfer.quantity.symbol, "\n");
    print("Memo             :   ", transfer.memo, "\n");

    auto total_amount = transfer.quantity.amount;

    partner_table p_t(_self, _self);
    for (auto itr = p_t.cbegin(); itr != p_t.cend(); ++itr) {
        print(name{itr->partner_acct});
        auto partner_acct = itr->partner_acct;
        asset amount_dispatched  = transfer.quantity * itr->weightx100 / 100;
        //asset comm = transfer.quantity * c_itr->comm_x100 / 10000;
        paytoken(_self, partner_acct, amount_dispatched, transfer.memo);
    }

}

void eoscomm::addpartner( const account_name partner,
                          const uint32_t weightx100)
{
    require_auth(_self);
    config_table c_t (_self, _self);
    eosio_assert(c_t.begin() != c_t.end(), "Config is not set.");
    auto itr = c_t.find(0);
    eosio_assert(itr->settled != 1, "Contract is settled");

    partner_table p_t(_self, _self);


    p_t.emplace(_self, [&](auto &l) {
        l.partner_acct = partner;
        l.weightx100 = weightx100;
        l.issued_time = now();
    });
}

void eoscomm::close()
{
    require_auth(_self);
    config_table c_t (_self, _self);

    eosio_assert(c_t.begin() != c_t.end(), "Config is not set.");
    auto itr = c_t.find(0);

    c_t.modify (itr, _self, [&](auto& c) {
      c.settled      = 1;
    });
}

void eoscomm::setconfig(const account_name _token_contract,
                        const string _symbol,
                        const uint8_t _symbol_precision)
{
    require_auth(_self);

    config_table c_t(_self, _self);
    auto itr = c_t.begin();
    if (itr != c_t.end())
    {
        c_t.erase(itr);
    }

    c_t.emplace(_self, [&](auto &c) {
        c.config_id = c_t.available_primary_key();
        c.token_contract = _token_contract;
        c.payment_symbol = string_to_symbol(_symbol_precision, _symbol.c_str());
        c.settled = 0;
    });

}

/*
void eoscomm::addlisting(const account_name    _seller,
                          const account_name    _account_for_sale,
                          const asset           _price)
{

    require_auth(_seller);

    eosio_assert(_seller != _account_for_sale, "Seller can be the same account as one that is for sale.");

    listing_table l_t(_self, _self);

    auto forsale_index = l_t.get_index<N(forsale)>();
    auto forsale_itr = forsale_index.find(_account_for_sale);
    eosio_assert(forsale_itr == forsale_index.end(), "Account is already for sale.");

    l_t.emplace(_seller, [&](auto &l) {
        l.listing_id = l_t.available_primary_key();
        l.seller = _seller;
        l.account_for_sale = _account_for_sale;
        l.price = _price;
        l.created_date = now();
    });

    config_table c_t(_self, _self);
    auto c_itr = c_t.find(0);
    //changekey(_account_for_sale, c_itr->holding_pk, true);
    chgownerkey (_account_for_sale, c_itr->holding_pk, true);
    chgactivekey (_account_for_sale, c_itr->holding_pk, true);
}

void eoscomm::dellisting(const uint64_t _listing_id)
{
    listing_table l_t(_self, _self);
    auto itr = l_t.find(_listing_id);
    eosio_assert(itr != l_t.end(), "Listing ID is not found.");
    require_auth(itr->seller);
    l_t.erase(itr);
}

void eoscomm::setconfig(const uint32_t     _comm_x100,
                         const account_name _comm_account,
                         const account_name _token_contract,
                         const string       _symbol,
                         const uint8_t      _symbol_precision,
                         const public_key   _holding_pk)
{
    require_auth(_self);

    config_table c_t(_self, _self);
    auto itr = c_t.begin();
    if (itr != c_t.end())
    {
        c_t.erase(itr);
    }

    c_t.emplace(_self, [&](auto &c) {
        c.config_id = c_t.available_primary_key();
        c.comm_x100 = _comm_x100;
        c.comm_account = _comm_account;
        c.token_contract = _token_contract;
        c.payment_symbol = string_to_symbol(_symbol_precision, _symbol.c_str());
        c.holding_pk = _holding_pk;
    });
}

void eoscomm::purchase(const uint64_t      _listing_id,
                        const account_name  _buyer,
                        const public_key    _new_owner_key,
                        const public_key    _new_active_key,
                        const asset         _pay_amount)
{

    require_auth(_buyer);

    listing_table l_t(_self, _self);
    auto itr = l_t.find(_listing_id);
    eosio_assert(itr != l_t.end(), "Listing ID is not found.");

    string key_string = to_string(now());
    key_string = key_string + to_string(_listing_id) + to_string(_buyer);
    print ("key string: ", key_string, "\n");
        
    if (itr->price <= _pay_amount)
    {
        buyrecord_table br_t(_self, _self);
        br_t.emplace(_buyer, [&](auto &p) {
            p.buyrecord_id = hashkey(key_string);
            p.buyer = _buyer;
            p.listing_id = _listing_id;
            p.new_owner_key = _new_owner_key;
            p.new_active_key = _new_active_key;
            p.pay_amount = _pay_amount;
        });
    }
}

void eoscomm::apply(const account_name contract, const account_name act)
{

    if (act == N(transfer))
    {
        transferReceived(unpack_action_data<currency::transfer>(), contract);
        return;
    }

    auto &thiscontract = *this;

    switch (act)
    {
        EOSIO_API(eoscomm, (setconfig)(addlisting)(dellisting)(purchase))
    };
}

void eoscomm::transferReceived(const currency::transfer &transfer, const account_name code)
{
    if (transfer.to != _self)
    {
        return;
    }

    print("Account Name     :   ", name{code}, "\n");
    print("From             :   ", name{transfer.from}, "\n");
    print("To               :   ", name{transfer.to}, "\n");
    print("Asset            :   ", transfer.quantity, "\n");
    print("Received Amount  :   ", transfer.quantity.amount, "\n");
    print("Received Symbol  :   ", transfer.quantity.symbol, "\n");
    print("Memo             :   ", transfer.memo, "\n");

    config_table c_t(_self, _self);
    auto c_itr = c_t.find(0);
    print("Expected payment symbol  : ", c_itr->payment_symbol, "\n");

    eosio_assert(c_itr->payment_symbol == transfer.quantity.symbol, "Payment is not the right symbol.");

    std::string::size_type sz;

    // need to confirm transfer is of eosio.token
    buyrecord_table br_t(_self, _self);
    auto b_itr = br_t.find(std::stoll(transfer.memo, &sz));
    
    eosio_assert(b_itr != br_t.end(), "Buy record ID is not found.");
    eosio_assert(transfer.from == b_itr->buyer, "Transfer is not from the buyer.");

    listing_table l_t(_self, _self);
    auto l_itr = l_t.find(b_itr->listing_id);
    eosio_assert(l_itr != l_t.end(), "Listing ID is not found.");

    asset comm = transfer.quantity * c_itr->comm_x100 / 10000;
    asset pay_to_seller = transfer.quantity - comm;
    paytoken(_self, c_itr->comm_account, comm, transfer.memo);
    paytoken(_self, l_itr->seller, pay_to_seller, to_string(name{l_itr->account_for_sale}));

    //changekey(l_itr->account_for_sale, b_itr->new_key, false);
    chgownerkey (l_itr->account_for_sale, b_itr->new_owner_key, false);
    chgactivekey (l_itr->account_for_sale, b_itr->new_active_key, false);
    l_t.erase(l_itr);
}
*/


extern "C"
{
    //using namespace bay;
    using namespace eosio;

    void apply(uint64_t receiver, uint64_t code, uint64_t action)
    {
        auto self = receiver;
        eoscomm contract(self);
        contract.apply(code, action);
        eosio_exit(0);
    }
}