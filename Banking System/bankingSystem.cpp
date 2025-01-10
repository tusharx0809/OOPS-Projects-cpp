#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;

class Account{

    private:
        int accountNumber;
        string name;
        double balance;
        string accountType;

    public:

        //Constructor
        Account(int accNum, string accName, double initialBalance, string accType)
        : accountNumber(accNum), name(accName), balance(initialBalance), accountType(accType) {}

        //Methods
        void deposit(double amount){
            balance += amount;
            cout << "Deposited: "<< amount << ". New Balance: " << balance << endl;
        }

        bool withdraw(double amount){
            if(amount > balance){
                cout<<"Insufficient Balance!"<<endl;
                return false;
            }
            balance -= amount;
            cout<<"Withdrawn: "<<amount<<". Remaining Balance: "<<balance;<<endl;
            return true;
        }

        void displayDetails() const {
            cout<<"Account number: "<< accountNumber << endl;
            cout<<"Name: "<< name << endl;
            cout<<"Balance: "<<balance<<endl;
            cout<<"Account Type: "<<accountType << endl;
        }

        int getAccountNumber() const{
            return accountNumber;
        }

        double getBalance() const{
            return balance;
        }

        void setBalance(double newBalance){
            balance = newBalance;
        }
};