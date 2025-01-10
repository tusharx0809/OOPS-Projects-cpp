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
};