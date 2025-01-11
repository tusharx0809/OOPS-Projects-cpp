#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
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
    https: // www.w3schools.com/cpp/cpp_function_param.asp
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

    // Method to save account details to a string (for file writing)
    string toFileString() const
    {
        stringstream ss;
        ss << accountNumber << " " << name << " " << balance << " " << accountType;
        return ss.str();
    }

    // Static method to create an Account object from a file line
    static Account fromFileString(const string &line)
    {
        stringstream ss(line);
        int accNum;
        string name, accType;
        double balance;
        ss >> accNum >> name >> balance >> accType;
        return Account(accNum, name, balance, accType);
    }
};

class RecurringDepositAccount: public Account{
    private:
        Account* parenetAcc;
        double monthlyDeposit;
        int duration;
        double interestRate;

    public:
        //Constructor
        RecurringDepositAccount(int accNum, string accName, double initialBalance, double deposit, int durationInMonths, Account* parent)
        : Account(accNum, accName, initialBalance, "Recurring Deposit"),
          parentAcc(parent), 
          monthlyDeposit(deposit), 
          duration(durationInMonths), 
          interestRate(6.5) 
        {}
};

class Bank
{
private:
    vector<Account> accounts;

    // Helper method to find an account by number
    Account *findAccount(int accountNumber)
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

    // Helper method to load accounts from the file
    void loadAccountsFromFile()
    {
        ifstream file("accounts.txt");
        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                accounts.push_back(Account::fromFileString(line));
            }
        }
        file.close();
    }

    // Helper method to save accounts to the file
    void saveAccountsToFile()
    {
        ofstream file("accounts.txt");
        for (const auto &account : accounts)
        {
            file << account.toFileString() << endl;
        }
        file.close();
    }

public:
    Bank()
    {
        loadAccountsFromFile(); // Load accounts from file when the bank is created
    }

    bool checkAccount(int accNum)
    {
        if (findAccount(accNum) != NULL)
        {
            return true;
        }
        return false;
    }

    

    void addAccount(int accNum, string name, double initialBalance, string accType)
    {
        accounts.emplace_back(accNum, name, initialBalance, accType);
        cout << "Account created successfully!" << endl;
        saveAccountsToFile();
    }

    void depositToAccount(int accNum, double amount)
    {
        Account *account = findAccount(accNum);
        if (account)
        {
            account->deposit(amount);
        }
        else
        {
            cout << "Account not found!" << endl;
        }
        saveAccountsToFile();
    }

    void withdrawFromAccount(int accNum, double amount)
    {
        Account *account = findAccount(accNum);
        if (account)
        {
            account->withdraw(amount);
        }
        else
        {
            cout << "Account not found!" << endl;
        }
        saveAccountsToFile();
    }

    void transferMoney(int fromAccNum, int toAccNum, double amount)
    {
        Account *fromAccount = findAccount(fromAccNum);
        Account *toAccount = findAccount(toAccNum);

        if (fromAccount && toAccount)
        {
            if (fromAccount->withdraw(amount))
            {
                toAccount->deposit(amount);
                cout << "Transfer Successful!" << endl;
            }
        }
        else
        {
            cout << "One or both accounts not found!" << endl;
        }
        saveAccountsToFile();
    }

    void displayAccountDetails(int accNum)
    {
        Account *account = findAccount(accNum);
        if (account)
        {
            account->displayDetails();
        }
        else
        {
            cout << "Account not found!" << endl;
        }
    }
    void initializeRandomSeed()
    {
        srand(time(0));
    }
    int generateAccountNumber()
    {
        int accNum = rand() % 9999999 + 1000000;
        while (checkAccount(accNum))
        {
            accNum = rand() % 9999999 + 1000000;
        }
        return accNum;
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