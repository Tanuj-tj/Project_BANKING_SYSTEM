#include<iostream>
#include<fstream>
#include<map>
using namespace std;

#define MIN_BALANCE 500;
class InsufficientFunds{};

/*
  Class -> Account
*/

class Account{
    // Private Data Members
    long account_no;  
    string first_name;
    string last_name;
    float balance;
    static long Next_account_no;

    public:
    // NonParameterized Constructor
    Account(){}  
    // Parameterized Constructor
    Account(string first_name,string last_name,float balance){
        Next_account_no++;
        account_no = Next_account_no;
        this->first_name = first_name;
        this->last_name = last_name;
        this->balance = balance;
    }

    // Accessor Functions
    long getAccNo(){return account_no;}
    string getFirstName(){return first_name;}
    string getLastName(){return last_name;}
    float getBalance(){return balance;}

    // Mutator Functions
    void Deposit(float amount){
        balance+=amount;
    }
    void Withdraw(float amount);

    static void setLastAccountNo(long accountNo){
        Next_account_no = accountNo;
    }
    static long getLastAccountNo(){
        return Next_account_no;
    }

    // Friend Function Definations
    friend ofstream & operator<<(ofstream &ofs,Account &A);
    friend ifstream & operator>>(ifstream &ifs,Account &A);
    friend ostream & operator<<(ostream &os,Account &A);
};

// Assigning the value in static data member outside the class
long Account::Next_account_no = 0;

/*
  Class -> Bank
*/

class Bank{

    map<long,Account> accounts;       //{ Key : Value }

    public:

    Bank(){
        Account account;   // Datatype -> Accounts , accounts -> {FName,LName,Balance}
        ifstream ifs;
     
        ifs.open("Record.data"); // Reading data from Record.data file
        
        // If the file does not exist generate a message
        if(!ifs){
            cout<<"Error in opening! File Not Found!!"<<endl;
            return;
        }
        // Insert values of Account until end of file is reaches
        while(!ifs.eof()){
            ifs>>account;
            accounts.insert(pair<long,Account>(account.getAccNo(),account));
        }
        Account::setLastAccountNo(account.getAccNo());

        ifs.close();
    }

    // Creating Account class objects 
    Account OpenAccount(string fname,string lname,float balance){
        ofstream ofs;
        Account account(fname,lname,balance);
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
        ofs.open("Record.data",ios::trunc);

        map<long,Account>::iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++){
            ofs<<itr->second;
        }
        ofs.close();
        return account;
    }
    Account BalanceEnquiry(long accountNo){
        map<long,Account>::iterator itr=accounts.find(accountNo);
        return itr->second;
    }
    Account Deposit(long accountNo,float amount){
        map<long,Account>::iterator itr=accounts.find(accountNo);
        itr->second.Deposit(amount);
        return itr->second;
    }
    Account Withdraw(long accountNo,float amount){
        map<long,Account>::iterator itr=accounts.find(accountNo);
        itr->second.Withdraw(amount);
        return itr->second;
    }

    // Functions

    void CloseAccount(long accountNo){
        map<long,Account>::iterator itr=accounts.find(accountNo);
        cout<<"Account Deleted"<<itr->second;
        accounts.erase(accountNo);
    }
    void ShowAllAccounts(){
        map<long,Account>::iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++){
            cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
        }
    }

    // Destroy the Bank Class object 
    ~Bank(){
        ofstream ofs;
        ofs.open("Record.data",ios::trunc);
        map<long,Account>::iterator itr;
        for(itr=accounts.begin();itr!=accounts.end();itr++){
            ofs<<itr->second;
        }
        ofs.close();
    }

};

int main(){

    Bank B;
    Account A;
    int choice;
    string fname,lname;
    long account_No;
    float balance;
    float amount;

    do{
        cout<<endl<<"---------------------------------------"<<endl;
        cout<<"************ BANKING SYSTEM ***********"<<endl;
        cout<<"---------------------------------------"<<endl<<endl;
        cout<<"SELECT ONE OPTION BELOW  :"<<endl;
        cout<<"1. Open an account "<<endl;
        cout<<"2. Balance Enquiry "<<endl;
        cout<<"3. Deposit "<<endl;
        cout<<"4. Withdrawal "<<endl;
        cout<<"5. Chose an account "<<endl;
        cout<<"6. Show all account "<<endl;
        cout<<"7. Quit "<<endl<<endl;
        
        cout<<"Enter your choice : ";
        cin>>choice;

        switch(choice){
            case 1:
                cout<<endl<<"Enter First Name: ";
                cin>>fname;
                cout<<"Enter Last Name: ";
                cin>>lname;
                cout<<"Enter Initial Balance: ";
                cin>>balance;
                A = B.OpenAccount(fname,lname,balance);
                cout<<endl<<"Congratulation Account is Created"<<endl;
                cout<<A;
                break;

            case 2:
                cout<<"Enter Account Number: ";
                cin>>account_No;
                A=B.BalanceEnquiry(account_No);
                cout<<endl<<"Your Account Details"<<endl;
                cout<<A;
                break;

            case 3:
                cout<<"Enter Account Number: ";
                cin>>account_No;
                cout<<"Enter Balance: ";
                cin>>amount;
                A = B.Deposit(account_No,amount);
                cout<<endl<<"Amount is Deposited"<<endl;
                cout<<A;
                break;

            case 4:
                cout<<"Enter Account Number: ";
                cin>>account_No;
                cout<<"Enter Balance: ";
                cin>>amount;
                A = B.Withdraw(account_No,amount);
                cout<<endl<<"Amount Withdrawn"<<endl;
                cout<<A;
                break;

            case 5:
                cout<<"Enter Account Number: ";
                cin>>account_No;
                B.CloseAccount(account_No);
                cout<<endl<<"Account is Closed "<<endl;
                cout<<A;

            case 6:
                B.ShowAllAccounts();
                break;

            case 7:
                break;
            
            default:
                cout<<endl<<"Enter Correct Choice";
                exit(0);
        }
    }while(choice!=7);

    return 0;
}

void Account::Withdraw(float amount){
     balance-=amount;
}

ofstream & operator<<(ofstream &ofs,Account &A){
    ofs<<A.account_no<<endl;
    ofs<<A.first_name<<endl;
    ofs<<A.last_name<<endl;
    ofs<<A.balance<<endl;
    return ofs;
}
ifstream & operator>>(ifstream &ifs,Account &A){
    ifs>>A.account_no;
    ifs>>A.first_name;
    ifs>>A.last_name;
    ifs>>A.balance;
    return ifs;
}
ostream & operator<<(ostream &os,Account &A){
    os<<"First Name : "<<A.getFirstName()<<endl;
    os<<"Last Name : "<<A.getLastName()<<endl;
    os<<"Account Number "<<A.getAccNo()<<endl;
    os<<"Balance. : "<<A.getBalance()<<endl;
    return os;
}