#include<iostream>
#include<vector>
#include<map>
#include<queue>
using namespace std;

class Splitwise{
    private:
        vector<vector<double>> split_matrix;
        vector<double> individual_totals;
        map<int,string> index_to_name;
        map<string,int> name_to_index;
        int group_size = 0;

        void print_ind_totals(){
            for(int i = 0; i < group_size; i++){
                if(individual_totals[i] >= 0){
                    cout<<index_to_name[i]<<" has to give "<<individual_totals[i]<<endl;
                }
                else{
                    cout<<index_to_name[i]<<" gets back "<<(-individual_totals[i])<<endl;
                }
            }
        }

        void print_splits(){
            for(int i = 0; i < group_size; i++){
                cout<<"----------------------"<<endl;
                cout<<index_to_name[i]<<"'s expense report"<<endl;
                for(int j = 0; j < group_size; j++){
                    if(i == j){continue;}
                    if(split_matrix[i][j] > 0){
                        cout<<"     owes "<<split_matrix[i][j]<<" Rupees to "<<index_to_name[j]<<" "<<endl;
                    }
                    else if(split_matrix[i][j] < 0){
                        cout<<"     is owed "<<-split_matrix[i][j]<<" Rupees by "<<index_to_name[j]<<endl;
                    }
                }
            }
            cout<<"----------------------"<<endl;
        }

        void add_direct_expense(){
            cout<<"Paid by: ";
            string paidby;
            cin>>paidby;

            cout<<"Paid to: ";
            string paidto;
            cin>>paidto;

            cout<<"Enter amount: ";
            double amount;
            cin>>amount;

            individual_totals[name_to_index[paidby]] -= amount;
            individual_totals[name_to_index[paidto]] += amount;
        }

        void add_group_expense(){
            cout<<"Paid by: ";
            string paidby;
            cin>>paidby;

            cout<<"Enter amount: ";
            double amount;
            cin>>amount;

            cout<<"Equal or Unequal: ";
            string mode;
            cin>>mode;

            if(mode == "Equal"){
                cout<<"Was this person involved?\n";
                vector<int> index_ind;

                for(int i = 0; i < group_size; i++){
                    cout<<index_to_name[i]<<": (Yes/No)";
                    string ans; cin>>ans;
                    if(ans == "Yes"){
                        index_ind.push_back(i);
                    }
                }
                add_group_expense_equal(paidby, amount, index_ind);
            }
            else{
                cout<<"Was this person involved?\n";
                vector<pair<int,double>> index_ind;
                double total_amt = 0;

                for(int i = 0; i < group_size; i++){
                    cout<<index_to_name[i]<<": (Yes/No)";
                    string ans; cin>>ans;
                    if(ans == "Yes"){
                        cout<<"How much do they owe: ";
                        double amt; cin>>amt;
                        index_ind.push_back({i,amt});
                        total_amt += amt;
                    }
                }

                if(total_amt != amount){
                    cout<<"Error: Amount does not add up. Expense not recorded. Please reenter";
                }
                else{
                    add_group_expense_unequal(paidby, amount, index_ind);
                }
            }
        }

        void add_group_expense_equal(string paidby, double amount, vector<int> index_ind){
            double amt_ind = amount/index_ind.size();
            individual_totals[name_to_index[paidby]] -= amount;
            for(auto it: index_ind){
                individual_totals[it] += amt_ind;
            }
        }

        void add_group_expense_unequal(string paidby, double amount, vector<pair<int,double>> index_ind){
            individual_totals[name_to_index[paidby]] -= amount;
            for(auto it: index_ind){
                individual_totals[it.first] += it.second;
            }
        }

        void calculate_splits(){
            vector<double> temp_totals = individual_totals;
            int i = 0, j = 0;
            while(i < group_size && j < group_size){
                while(i < group_size && temp_totals[i] <= 0){
                    i++;
                }
                while(j < group_size && temp_totals[j] >= 0){
                    j++;
                }
                if(i >= group_size || j >= group_size){
                    break;
                }
                if(temp_totals[i] > -temp_totals[j]){
                    split_matrix[i][j] = -temp_totals[j];
                    split_matrix[j][i] = temp_totals[j];
                    temp_totals[i] += temp_totals[j];
                    temp_totals[j] = 0;
                }
                else if(temp_totals[i] < -temp_totals[j]){
                    split_matrix[i][j] = temp_totals[i];
                    split_matrix[j][i] = -temp_totals[i];
                    temp_totals[j] += temp_totals[i];
                    temp_totals[i] = 0;
                }
                else if(temp_totals[i] == -temp_totals[j]){
                    split_matrix[i][j] = temp_totals[i];
                    split_matrix[j][i] = temp_totals[j];
                    temp_totals[i] = 0;
                    temp_totals[j] = 0;
                }
            }
        }

    public:
        Splitwise(){
            cout<<"--------------------------"<<endl;
            cout<<"Welcome to this Splitwise clone created by Rudra"<<endl;
            cout<<"You need to create a group first"<<endl;

            cout<<"Enter number of people: ";
            int n; cin>>n;

            cout<<"Enter Names: ";
            vector<string> names;
            for(int i = 0; i < n; i++){
                string name; cin>>name;
                names.push_back(name);
            }

            group_size = n;
            for(int i = 0; i < group_size; i++){
                name_to_index[names[i]] = i;
                index_to_name[i] = names[i];
                split_matrix.push_back(vector<double>(n,0));
                individual_totals.push_back(0);
            }
        }

        void interactive(){
            cout<<"----------------"<<endl;
            cout<<"Group created. You can add expenses now"<<endl;

            while(1){
                cout<<"----------------"<<endl;
                cout<<"You can do: "<<endl;
                cout<<"1. Check group splits"<<endl;
                cout<<"2. Check individual expenditure"<<endl;
                cout<<"3. Add a direct payment"<<endl;
                cout<<"4. Add a group expense"<<endl;
                cout<<"5. Quit"<<endl;
                cout<<"Enter a number from 1 to 5"<<endl;

                int choice; cin>>choice;
                if(choice > 5 || choice < 1){
                    cout<<"Choose a number between 1 and 5"<<endl;
                    continue;
                }
                if(choice == 5){
                    break;
                }

                if(choice == 1){
                    calculate_splits();
                    print_splits();
                }
                else if(choice == 2){
                    print_ind_totals();
                }
                else if(choice == 3){
                    add_direct_expense();
                }
                else if(choice == 4){
                    add_group_expense();
                }

                cout<<"Success"<<endl;
            }
        }
};

int main(){
    Splitwise S;
    S.interactive();
}