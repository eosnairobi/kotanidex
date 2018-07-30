/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>

namespace eosiosystem
{
class system_contract;
}

namespace eosio
{

class escrow : public contract
{
      public:
        escrow(account_name self) : contract(self) {}

        void eshello(account_name user);

        void escreate(account_name issuer,
                      asset maximum_supply);

        void esissue(account_name to, asset quantity);

        void estransfer(account_name from,
                        account_name to,
                        account_name agent,
                        asset quantity);

        void esrelease(account_name agent,
                       account_name to,
                       asset quantity);

        inline asset get_supply(symbol_name sym) const;

        inline asset get_balance(account_name owner, symbol_name sym) const;

      private:
        struct account
        {
                asset balance;

                uint64_t primary_key() const { return balance.symbol.name(); }
        };

        struct currency_stats
        {
                asset supply;
                asset max_supply;
                account_name issuer;

                uint64_t primary_key() const { return supply.symbol.name(); }
        };

        typedef eosio::multi_index<N(accounts), account> accounts;

        typedef eosio::multi_index<N(stat), currency_stats> stats;

        void sub_balance(account_name owner, asset value);
        void add_balance(account_name owner, asset value, account_name ram_payer);

      public:
        struct transfer_args
        {
                account_name from;
                account_name to;
                asset quantity;
        };
};

asset escrow::get_supply(symbol_name sym) const
{
        stats statstable(_self, sym);
        const auto &st = statstable.get(sym);
        return st.supply;
}

asset escrow::get_balance(account_name owner, symbol_name sym) const
{
        accounts accountstable(_self, owner);
        const auto &ac = accountstable.get(sym);
        return ac.balance;
}

} // namespace eosio
