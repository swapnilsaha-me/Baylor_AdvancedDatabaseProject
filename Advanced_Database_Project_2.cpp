#include<bits/stdc++.h>
using namespace std;

#define pf              printf
#define pb              push_back

#define MAX_DATA        32

#define INVALID_INPUT   0
#define START_INPUT     1
#define READ_INPUT      2
#define WRITE_INPUT     3
#define COMMIT_INPUT    4
#define EOF_INPUT       5

// Transaction Type
#define READ            0
#define WRITE           1

// Transaction Status
#define NOT_GRANTED     0
#define GRANTED         1
#define COMMITED        2
#define ROLLBACK        3

// Lock Type
#define NO_LOCK         0
#define SHARED_LOCK     1
#define EXCLUSIVE_LOCK  2

int originalData;

int next_transaction_id;
map<int, int>hash_transaction_id;
map<int, int>original_transaction_id;

int getTransactionId(int id, bool isStart, bool isOriginal)
{
    if(isOriginal)
    {
        return original_transaction_id[id];
    }
    else
    {
        if(isStart)
        {
            hash_transaction_id[id] = ++next_transaction_id;
            original_transaction_id[next_transaction_id] = id;
        }
        return hash_transaction_id[id];
    }
}

struct Transaction
{
    int transaction_id;
    int data_id;
    int transaction_type;
    int transaction_status;
};

struct DataTable
{
    int lock_type = NO_LOCK;
    vector<Transaction>concurrent_transaction;
    vector<Transaction>waiting_transaction;
};

DataTable data_table[MAX_DATA];

vector<vector<int>>graph;

//struct TransactionTable
//{
//    int transaction_status;
//    vector<Transaction>transactions;
//};
//
//vector<TransactionTable>transaction_table;

/*
 * This method is for initialization.
 */
void init()
{
    originalData = 0;
    scanf("%d", &originalData);
    vector<int>temp;
    graph.pb(temp);
}

int processInput(string str)
{
    for(int i = 0; i < str.size(); i++)
    {
        if('A' <= str[i] && str[i] <= 'Z')
        {
            str[i] = str[i] - 'A' + 'a';
        }
        if('a' <= str[i] && str[i] <= 'z')
        {
            continue;
        }
        return INVALID_INPUT;
    }
    if(str == "start")
    {
        return START_INPUT;
    }
    if(str == "read")
    {
        return READ_INPUT;
    }
    if(str == "write")
    {
        return WRITE_INPUT;
    }
    if(str == "commit")
    {
        return COMMIT_INPUT;
    }
    if(str == "eof")
    {
        return EOF_INPUT;
    }
    return INVALID_INPUT;
}

bool isLockShouldBeUpgraded(int data_id, int transaction_id, int transaction_type)
{
    if(data_table[data_id].lock_type == NO_LOCK)
    {
        return false;
    }
    if(data_table[data_id].lock_type == EXCLUSIVE_LOCK)
    {
        return false;
    }
    if(data_table[data_id].waiting_transaction.size())
    {
        return false;
    }
    for(int i = 0; i < data_table[data_id].concurrent_transaction.size(); i++)
    {
        if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
        {
            return false;
        }
    }
    if(transaction_type == WRITE)
    {
        return true;
    }
    return false;
}

bool isLockCanBeGranted(int data_id, int transaction_id, int transaction_type)
{
    if(isLockShouldBeUpgraded(data_id, transaction_id, transaction_type))
    {
        return true;
    }
    if(data_table[data_id].lock_type == NO_LOCK)
    {
        return true;
    }
    if(data_table[data_id].waiting_transaction.size())
    {
        return false;
    }
    if(data_table[data_id].lock_type == EXCLUSIVE_LOCK)
    {
        for(int i = 0; i < data_table[data_id].concurrent_transaction.size(); i++)
        {
            if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
            {
                return false;
            }
        }
        return true;
    }
    if(transaction_type == READ)
    {
        return true;
    }
    for(int i = 0; i < data_table[data_id].concurrent_transaction.size(); i++)
    {
        if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
        {
            return false;
        }
        return true;
    }
}

void updateLock(int data_id, int transaction_id, int transaction_type)
{
    if(data_table[data_id].lock_type == EXCLUSIVE_LOCK)
    {
        return;
    }
    if(transaction_type == READ)
    {
        data_table[data_id].lock_type = SHARED_LOCK;
        return;
    }
    data_table[data_id].lock_type = EXCLUSIVE_LOCK;
    return;
}

void addEdge(int data_id, int transaction_id, int transaction_type)
{
    vector<int>vv = graph[transaction_id];
    if(transaction_type == READ)
    {
        for(int i = data_table[data_id].waiting_transaction.size() - 2; i >= 0; i--)
        {
            if(data_table[data_id].waiting_transaction[i].transaction_type == WRITE)
            {
                if(data_table[data_id].waiting_transaction[i].transaction_id != transaction_id)
                {
                    vv.pb(data_table[data_id].waiting_transaction[i].transaction_id);
                    graph[transaction_id] = vv;
                    return;
                }
            }
        }
        for(int i = 0; i < data_table[data_id].concurrent_transaction.size(); i++)
        {
            if(data_table[data_id].concurrent_transaction[i].transaction_type == WRITE)
            {
                if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
                {
                    vv.pb(data_table[data_id].concurrent_transaction[i].transaction_id);
                    graph[transaction_id] = vv;
                    return;
                }
            }
        }
    }
    else
    {
        bool transaction_found = false;
        for(int i = data_table[data_id].waiting_transaction.size() - 1; i >= 0; i--)
        {
            if(data_table[data_id].waiting_transaction[i].transaction_type == WRITE)
            {
                if(transaction_found)
                {
                    graph[transaction_id] = vv;
                    return;
                }
                if(data_table[data_id].waiting_transaction[i].transaction_id != transaction_id)
                {
                    vv.pb(data_table[data_id].waiting_transaction[i].transaction_id);
                    graph[transaction_id] = vv;
                    return;
                }
            }
            if(data_table[data_id].waiting_transaction[i].transaction_id != transaction_id)
            {
                transaction_found = true;
                vv.pb(data_table[data_id].waiting_transaction[i].transaction_id);
            }
        }
        for(int i = 0; i < data_table[data_id].concurrent_transaction.size(); i++)
        {
            if(data_table[data_id].concurrent_transaction[i].transaction_type == WRITE)
            {
                if(transaction_found)
                {
                    graph[transaction_id] = vv;
                    return;
                }
                if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
                {
                    vv.pb(data_table[data_id].concurrent_transaction[i].transaction_id);
                    graph[transaction_id] = vv;
                    return;
                }
            }
            if(data_table[data_id].concurrent_transaction[i].transaction_id != transaction_id)
            {
                transaction_found = true;
                vv.pb(data_table[data_id].concurrent_transaction[i].transaction_id);
            }
        }
        graph[transaction_id] = vv;
        return;
    }
}

void printGraph()
{
    for(int i = 1; i < graph.size(); i++)
    {
        pf("Transaction: %2d >> ", i);
        for(int j = 0; j < graph[i].size(); j++)
        {
            pf("%2d", graph[i][j]);
        }
        pf("\n");
    }
}

void addTransaction(int data_id, int transaction_id, int transaction_type)
{
    Transaction transaction;
    transaction.data_id = data_id;
    transaction.transaction_id = transaction_id;
    transaction.transaction_type = transaction_type;

    if(isLockCanBeGranted(data_id, transaction_id, transaction_type))
    {
        transaction.transaction_status = GRANTED;
        data_table[data_id].concurrent_transaction.push_back(transaction);
        updateLock(data_id, transaction_id, transaction_type);
    }
    else
    {
        transaction.transaction_status = NOT_GRANTED;
        data_table[data_id].waiting_transaction.push_back(transaction);
        addEdge(data_id, transaction_id, transaction_type);
    }
}

void processStart()
{
    int transaction_id;
    scanf("%d", &transaction_id);
    transaction_id = getTransactionId(transaction_id, true, false);
    vector<int>temp;
    graph.pb(temp);
}

void processRead()
{
    int data_id, transaction_id;
    scanf("%d %d", &data_id, &transaction_id);
    transaction_id = getTransactionId(transaction_id, false, false);
    printf("Read  >> Data_id = %d \t Tr_id = %d, Upgrade = %d, Granted = %d\n", data_id, transaction_id, isLockShouldBeUpgraded(data_id, transaction_id, READ), isLockCanBeGranted(data_id, transaction_id, READ));
    addTransaction(data_id, transaction_id, READ);
}

void processWrite()
{
    int data_id, transaction_id;
    scanf("%d %d", &data_id, &transaction_id);
    transaction_id = getTransactionId(transaction_id, false, false);
    printf("Write >> Data_id = %d \t Tr_id = %d, Upgrade = %d, Granted = %d\n", data_id, transaction_id, isLockShouldBeUpgraded(data_id, transaction_id, WRITE), isLockCanBeGranted(data_id, transaction_id, WRITE));
    addTransaction(data_id, transaction_id, WRITE);
}

void processCommit()
{
    for(int i = 0; i < 32; i++)
    {

    }
}

void processEof()
{

}

int main()
{
    init();
    string str;
    while(cin >> str)
    {
        int inputType = processInput(str);
        cout << "Input Type : " << inputType << endl;
        if(inputType == INVALID_INPUT)
        {

        }
        else if(inputType == START_INPUT)
        {
            processStart();
        }
        else if(inputType == READ_INPUT)
        {
            processRead();
        }
        else if(inputType == WRITE_INPUT)
        {
            processWrite();
        }
        else if(inputType == COMMIT_INPUT)
        {
            processCommit();
        }
        else if(inputType == EOF_INPUT)
        {
            processEof();
        }
        printGraph();
    }
    return 0;
}

/*


0
Start 1
Start 2
Start 3
Read 0 1
Read 1 2
Write 1 1
Write 0 1
Write 0 2
Read 0 3


0
Start 1
Start 2
Start 3
Read 0 1
Read 0 2
Write 0 3
Start 4
Start 5
Start 6
Read 0 4
Read 0 5
Write 0 6






                data    tran
        0
        Start 0
        Start 1
        Read    0       0
        Read    0       1
        Write   0       0
        commit 0
        Write   0       1
        commit 1

*/
