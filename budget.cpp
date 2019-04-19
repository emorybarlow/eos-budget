#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] budget : public contract {
  public:
      using contract::contract;

      [[eosio::action]]
      void addaccount(name account, float balance) {
        require_auth(_self);
        account_index accounts(get_self(), get_first_receiver().value);
        auto iterator = accounts.find(account.value);
        check(iterator == accounts.end(), "Account already exists");

        accounts.emplace(_self, [&]( auto& row ) {
          row.account = account;
          row.balance = (int) (balance * 100);
        });
      }

      [[eosio::action]]
      void delaccount(name account) {
        require_auth(_self);
        account_index accounts(get_self(), get_first_receiver().value);
        auto iterator = accounts.find(account.value);
        check(iterator != accounts.end(), "Account does not exist");
	accounts.erase(iterator);
      }

      [[eosio::action]]
      void updtbalance(name account, float balance) {
        require_auth(_self);
        account_index accounts(get_self(), get_first_receiver().value);
        auto iterator = accounts.find(account.value);
        check(iterator != accounts.end(), "Account does not exist");

        accounts.modify(iterator, _self, [&]( auto& row ) {
          row.account = account;
          row.balance = (int) (balance * 100);
        });
      }

      [[eosio::action]]
      void addtrx(name transaction, std::string display_name, float debit, std::string recurring, std::string start, int repeat) {
        require_auth(_self);
        transaction_index transactions(get_self(), get_first_receiver().value);
        auto iterator = transactions.find(transaction.value);
        check(iterator == transactions.end(), "Transaction already exists");

        transactions.emplace(_self, [&]( auto& row ) {
          row.transaction = transaction;
	  row.display_name = display_name;
          row.debit = (int) (debit * 100);
	  row.recurring = recurring;
	  row.start = start;
	  row.repeat = repeat;
        });
      }

      [[eosio::action]]
      void deltrx(name transaction) {
        require_auth(_self);
        transaction_index transactions(get_self(), get_first_receiver().value);
        auto iterator = transactions.find(transaction.value);
        check(iterator != transactions.end(), "Transaction does not exist");
	transactions.erase(iterator);
      }

      [[eosio::action]]
      void updttrx(name transaction, std::string display_name, float debit, std::string recurring, std::string start, int repeat) {
        require_auth(_self);
        transaction_index transactions(get_self(), get_first_receiver().value);
        auto iterator = transactions.find(transaction.value);
        check(iterator != transactions.end(), "Transaction does not exist");

        transactions.modify(iterator, _self, [&]( auto& row ) {
          row.transaction = transaction;
	  row.display_name = display_name;
          row.debit = (int) (debit * 100);
	  row.recurring = recurring;
	  row.start = start;
	  row.repeat = repeat;
        });
      }

  private:
    struct [[eosio::table]] account {
      name account;
      int balance;

      uint64_t primary_key() const { return account.value;}
    };
    typedef eosio::multi_index<"accounts"_n, account> account_index;

    struct [[eosio::table]] transaction {
      name transaction;
      std::string display_name;
      int debit;
      std::string recurring;
      std::string start;
      int repeat;

      uint64_t primary_key() const { return transaction.value;}
    };
    typedef eosio::multi_index<"transactions"_n, transaction> transaction_index;
};
