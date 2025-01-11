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

    string getName() const
    {
        return name;
    }
    string getAccountType() const
    {
        return accountType;
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

class RecurringDepositAccount : public Account
{
private:
    Account *parentAcc;
    double monthlyDeposit;
    int duration;
    double interestRate;

public:
    // Constructor
    RecurringDepositAccount(int accNum, string accName, double initialBalance, double deposit, int durationInMonths, Account *parent)
        : Account(accNum, accName, initialBalance, "Recurring Deposit"),
          parentAcc(parent),
          monthlyDeposit(deposit),
          duration(durationInMonths),
          interestRate(6.5)
    {};


    // method to deposity money in recurring from parent
    void depositToRecurring()
    {
        if (parentAcc->withdraw(monthlyDeposit))
        {
            parentAcc->withdraw(monthlyDeposit);
            deposit(monthlyDeposit);
            cout << "Monthly deposit of " << monthlyDeposit << " made to recurring account from parent account." << endl;
        }
        else
        {
            cout << "Insufficient balance!" << endl;
        }
    }

    // Calculate maturity amount
    double calculateMaturityAmount() const
    {
        double totalDeposit = monthlyDeposit * duration;
        double interest = (totalDeposit * interestRate * duration / 12) / 100;
        return getBalance() + interest;
    }

    // Display details
    void displayRecurringDetails() const
    {
        displayDetails();
        cout << "Monthly deposit: " << monthlyDeposit << endl;
        cout << "Duration(months): " << duration << endl;
        cout << "Interest Rate: " << interestRate << endl;
        cout << "Maturity Amount: " << calculateMaturityAmount() << endl;
        if (parentAcc)
        {
            cout << "Linked parent account: " << parentAcc->getAccountNumber() << endl;
        }
    }
    int getRecAccountNumber(){
        return getAccountNumber();
    }

    string toRecFileString() const
    {
        stringstream ss;
        ss << getAccountNumber() << " " << getName() << " " << getBalance() << " " << monthlyDeposit <<" " << duration <<" "<< getAccountType() <<" "<<parentAcc->getAccountNumber();
        return ss.str();
    }

    static RecurringDepositAccount fromRecFileString(const string &line, const vector<Account *> &allAccounts)
    {
        stringstream ss(line);
        int accNum, dur, parentAccNum;
        string name, accType;
        double balance, monthlydep;

        ss >> accNum >> name >> balance >> monthlydep >> dur >> accType >> parentAccNum;

        // Find the parent account from allAccounts
        Account *parentAcc = nullptr;
        for (auto *acc : allAccounts)
        {
            if (acc->getAccountNumber() == parentAccNum)
            {
                parentAcc = acc;
                break;
            }
        }

        // Ensure parentAcc is found, otherwise handle the error (return or throw an exception)
        if (!parentAcc)
        {
            cout << "Parent account not found!" << endl;
            // Handle the error as needed
        }

        return RecurringDepositAccount(accNum, name, balance, monthlydep, dur, parentAcc);
    }
};

class Bank
{
private:
    vector<Account> accounts;
    vector<RecurringDepositAccount> recAccounts;

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
    void loadRecAccountsFile()
    {
        ifstream file("rec_accounts.txt");
        string line;
        vector<Account *> accountPointers;

        // Create a vector of Account pointers
        for (auto &account : accounts)
        {
            accountPointers.push_back(&account);
        }

        while (getline(file, line))
        {
            if (!line.empty())
            {
                recAccounts.push_back(RecurringDepositAccount::fromRecFileString(line, accountPointers));
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

    void saveRecAccountsfile()
    {
        ofstream file("rec_accounts.txt");
        for (const auto &recAcc : recAccounts)
        {
            file << recAcc.toRecFileString() << endl;
        }
        file.close();
    }

public:
    Bank()
    {
        loadAccountsFromFile(); // Load accounts from file when the bank is created
        loadRecAccountsFile();
    }

    

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

    RecurringDepositAccount *findRecAccount(int accNum)
    {
        for (auto &recAcc : recAccounts)
        {
            if (recAcc.getAccountNumber() == accNum)
            {
                return &recAcc;
            }
        }
        return NULL;
    }

    void addAccount(int accNum, string name, double initialBalance, string accType)
    {
        accounts.emplace_back(accNum, name, initialBalance, accType);
        cout << "Account created successfully!" << endl;
        saveAccountsToFile();
    }

    void addRecAccount(Account *parent, double monthlyDeposit, int duration, double interestRate)
    {
        int accNum = generateRecAccountNumber();
        cout<<"Your Recurring account number: "<<accNum<<endl;

        RecurringDepositAccount newRecAccount(accNum, parent->getName(), monthlyDeposit, monthlyDeposit, duration, parent);

        if (parent->withdraw(monthlyDeposit))
        {
            
            recAccounts.push_back(newRecAccount);

            saveRecAccountsfile();

            cout << "Recurring Deposit account created successfully!" << endl;
        }
        else
        {
            cout << "Insufficient balance!" << endl;
        }
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
        while (findAccount(accNum) != NULL)
        {
            accNum = rand() % 9999999 + 1000000;
        }
        return accNum;
    }

    
    int generateRecAccountNumber(){
        int recAccNum = rand() % 9999999 + 1000000;
        while((findAccount(recAccNum) != NULL) && (findRecAccount(recAccNum) != NULL)){
            recAccNum = rand() % 9999999 + 1000000;
        }
        return recAccNum;
    }
};

int main()
{
    Bank bank;
    int choice;

    do
    {
        cout << "\n--- Banking System ---\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Display Account Details\n";
        cout << "6. Create Recurring Deposit Account\n";     // Option to create recurring deposit account
        cout << "7. Deposit Monthly to Recurring Deposit\n"; // Option to deposit to recurring account
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int accNum;
            string name, accType;
            double initialBalance;
            accNum = bank.generateAccountNumber();
            cout << "Your Account number is: " << accNum << endl;
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
        case 2:
        {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            bank.depositToAccount(accNum, amount);
            break;
        }
        case 3:
        {
            int accNum;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNum;
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            bank.withdrawFromAccount(accNum, amount);
            break;
        }
        case 4:
        {
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
        case 5:
        {
            int accNum;
            cout << "Enter Account Number: ";
            cin >> accNum;
            bank.displayAccountDetails(accNum);
            break;
        }
        case 6: // Create Recurring Deposit Account
        {
            int parentAccNum;
            double monthlyDeposit;
            int duration;
            double interestRate = 6.5; // Assume fixed interest rate
            cout << "Enter Parent Account Number: ";
            cin >> parentAccNum;

            Account *parentAccount = bank.findAccount(parentAccNum);
            if (!parentAccount)
            {
                cout << "Parent account not found!" << endl;
                break;
            }

            cout << "Enter Monthly Deposit: ";
            cin >> monthlyDeposit;
            cout << "Enter Duration (in months): ";
            cin >> duration;

            bank.addRecAccount(parentAccount, monthlyDeposit, duration, interestRate);
            break;
        }
        case 7: // Deposit Monthly to Recurring Deposit
        {
            int recAccNum;
            cout << "Enter Recurring Deposit Account Number: ";
            cin >> recAccNum;
            RecurringDepositAccount *recAcc = bank.findRecAccount(recAccNum);
            if (!recAcc)
            {
                cout << "Recurring Deposit account not found!" << endl;
                break;
            }

            recAcc->depositToRecurring();
            break;
        }
        case 8:
            cout << "Exiting... Thank you!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
