#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include<vector>
using namespace std;

class Account
{

private:
    int accountNumber;
    string name;
    double balance;
    string accountType;

public:
    // Constructor
    Account(int accNum, string accName, double initialBalance, string accType)
        : accountNumber(accNum), name(accName), balance(initialBalance), accountType(accType) {}

    // Methods
    void deposit(double amount)
    {
        balance += amount;
        cout << "Deposited: " << amount << ". New Balance: " << balance << endl;
    }

    bool withdraw(double amount)
    {
        if (amount > balance)
        {
            cout << "Insufficient Balance!" << endl;
            return false;
        }
        balance -= amount;
        cout << "Withdrawn: " << amount << ". Remaining Balance: " << balance << endl;
        return true;
    }

    void displayDetails() const
    {
        cout << "Account number: " << accountNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: " << balance << endl;
        cout << "Account Type: " << accountType << endl;
    }

    int getAccountNumber() const
    {
        return accountNumber;
    }

    double getBalance() const
    {
        return balance;
    }

    void setBalance(double newBalance)
    {
        balance = newBalance;
    }
};

class Bank
{
private:
    vector<Account> accounts;
    Account* findAccount(int accountNumber)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accountNumber)
            {
                return &account;
            }
        }
        return NULL;
    }

public:
    void addAccount(int accNum, string name, double initialBalance, string accType){
        accounts.emplace_back(accNum, name, initialBalance, accType); //constucts and Constructs and inserts an element at the end of accounts
        cout<<"Account created successfully!"<<endl;
    }

    void depositToAccount(int accNum, double amount){
        Account* account = findAccount(accNum);
        if(account){
            account->deposit(amount);
        }else{
            cout<<"Account not found!"<<endl;
        }
    }

    void withdrawFromAccount(int accNum, double amount){
        Account* account = findAccount(accNum);
        if(account){
            account->withdraw(amount);
        }else{
            cout<<"Account not found!"<<endl;
        }
    }
    void transferMoney(int fromAccNum, int toAccNum, double amount){
        Account* fromAccount = findAccount(fromAccNum);
        Account* toAccount = findAccount(toAccNum);

        if(fromAccount && toAccount){
            if(fromAccount->withdraw(amount)){
                toAccount->deposit(amount);
                cout<<"Transfer Successfull!"<<endl;
            }
        }else{
            cout<<"One or both accounts not found!"<<endl;
        }
    }
    void displayAccountDetails(int accNum){
        Account* account = findAccount(accNum);
        if(account){
            account->displayDetails();
        }else{
            cout<<"Account not found!"<<endl;
        }
    }
};


int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n--- Banking System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Display Account Details\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int accNum;
                string name, accType;
                double initialBalance;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Initial Balance: ";
                cin >> initialBalance;
                cout << "Enter Account Type (Savings/Current): ";
                cin >> accType;
                bank.addAccount(accNum, name, initialBalance, accType);
                break;
            }
            case 2: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                bank.depositToAccount(accNum, amount);
                break;
            }
            case 3: {
                int accNum;
                double amount;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                bank.withdrawFromAccount(accNum, amount);
                break;
            }
            case 4: {
                int fromAccNum, toAccNum;
                double amount;
                cout << "Enter Your Account Number: ";
                cin >> fromAccNum;
                cout << "Enter Receiver's Account Number: ";
                cin >> toAccNum;
                cout << "Enter Amount to Transfer: ";
                cin >> amount;
                bank.transferMoney(fromAccNum, toAccNum, amount);
                break;
            }
            case 5: {
                int accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                bank.displayAccountDetails(accNum);
                break;
            }
            case 6:
                cout << "Exiting... Thank you!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}