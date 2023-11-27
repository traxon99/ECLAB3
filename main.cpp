#include <iostream>
#include <iomanip>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    void displayDetails() const {
        std::cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        displayAdditionalDetails();
        std::cout << std::endl;
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposit of $" << amount << " successful." << std::endl;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    virtual void withdraw(double amount) const {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance." << std::endl;
        }
    }

    virtual std::string getType() const {
        return "Account";
    }

    virtual void displayAdditionalDetails() const {}

    double getBalance() const {
        return balance;
    }

    // Overload the << operator to display account details
    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "Account Details for " << account.getType() << " (ID: " << account.accountNumber << "):" << std::endl;
        os << "   Holder: " << account.accountHolder << std::endl;
        os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance << std::endl;
        account.displayAdditionalDetails();
        os << std::endl;
        return os;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void withdraw(double amount) const override {
        if (amount > 0 && amount <= balance && balance - amount >= 100) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance (minimum balance: $100)." << std::endl;
        }
    }

    std::string getType() const override {
        return "Savings Account";
    }

    void displayAdditionalDetails() const override {
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void withdraw(double amount) const override {
        if (amount > 0 && amount <= balance + overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrawal of $" << amount << " successful." << std::endl;
        } else {
            std::cout << "Invalid withdrawal amount or exceeded overdraft limit." << std::endl;
        }
    }

    std::string getType() const override {
        return "Current Account";
    }

    void displayAdditionalDetails() const override {
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    // Overload the + operator to allow transferring money between accounts
    CurrentAccount& operator+(const SavingsAccount& other) {
        double transferAmount = std::min(other.getBalance(), overdraftLimit + balance);
        other.withdraw(transferAmount);
        deposit(transferAmount);
        std::cout << "Transfer of $" << transferAmount << " from Savings Account to Current Account successful." << std::endl;
        return *this;
    }
};

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    std::cout << savings;
    std::cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings;
    std::cout << current;

    return 0;
}
