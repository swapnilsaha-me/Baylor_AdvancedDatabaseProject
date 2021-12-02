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

// Transaction Alive
#define NOT_PROCESSED   0
#define PROCESSED       1

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

vector<int>transaction_status;
vector<vector<int>>graph;
vector<bool>mark;
bool cycle;

void DFS(int src)
{
    mark[src] = 1;
    if(cycle)
    {
        return;
    }
    for(int i = 0; i < graph[src].size(); i++)
    {
        int dest = graph[src][i];
        if(src != dest)
        {
            if(mark[dest])
            {
                cycle = true;
                return;
            }
            DFS(dest);
        }
    }
    mark[src] = 0;
}

bool hasCycle()
{
    cycle = false;
    for(int i = 0; i < transaction_status.size(); i++)
    {
        mark[i] = 0;
    }
    for(int i = 1; i < transaction_status.size(); i++)
    {
        if(transaction_status[i] == NOT_PROCESSED)
        {
            DFS(i);
        }
    }
    return cycle;
}

void processTransaction(int data_id, int transaction_type)
{
    if(transaction_type == READ)
    {
        return;
    }
    originalData = originalData ^ (1 << data_id);
    return;
}

void processRollbackOrCommit(int transaction_id, bool isRollback)
{
    cout << "processRollbackOrCommit ========= " << transaction_id << ", " << isRollback << endl;
    transaction_status[transaction_id] = PROCESSED;
    for(int i = 0; i < 32; i++)
    {
        vector<Transaction>vv;
        for(int j = 0; j < data_table[i].concurrent_transaction.size(); j++)
        {
            if(data_table[i].concurrent_transaction[j].transaction_id == transaction_id)
            {
                if(isRollback)
                {
                    processTransaction(i, data_table[i].concurrent_transaction[j].transaction_type);
                }
            }
            else
            {
                vv.pb(data_table[i].concurrent_transaction[j]);
            }
        }
        for(int j = 0; j < data_table[i].waiting_transaction.size(); j++)
        {
            if(data_table[i].waiting_transaction[j].transaction_id != transaction_id)
            {
                vv.pb(data_table[i].waiting_transaction[j]);
            }
        }

        vector<Transaction>concurrent, wait;
        if(vv.size() != 0)
        {
            int j = 0;
            if(vv[0].transaction_type == READ)
            {
                bool differentTransaction = false;
                for( ; j < vv.size(); j++)
                {
                    if(vv[j].transaction_type == WRITE)
                    {
                        break;
                    }
                }

                for(int k = 0; k < j; k++)
                {
                    if(vv[0].transaction_id != vv[k].transaction_id)
                    {
                        differentTransaction = true;
                        break;
                    }
                }

                if(!differentTransaction)
                {
                    for( ; j < vv.size(); j++)
                    {
                        if(vv[0].transaction_id != vv[j].transaction_id)
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                for( ; j < vv.size(); j++)
                {
                    if(vv[0].transaction_id != vv[j].transaction_id)
                    {
                        break;
                    }
                }
            }

            for(int k = 0; k < j; k++)
            {
                if(vv[k].transaction_status == NOT_GRANTED)
                {
                    vv[k].transaction_status = GRANTED;
                    processTransaction(vv[k].data_id, vv[k].transaction_type);
                }
                concurrent.pb(vv[k]);
            }
            for(int k = j; k < vv.size(); k++)
            {
                wait.pb(vv[k]);
            }
        }
        data_table[i].concurrent_transaction = concurrent;
        data_table[i].waiting_transaction = wait;
        data_table[i].lock_type = NO_LOCK;
        if(i == 6)
        {
            for(int j = 0; j < data_table[i].concurrent_transaction.size(); j++)
            {
                pf("-- %d, %d -- ", data_table[i].concurrent_transaction[j].transaction_id, getTransactionId(data_table[i].concurrent_transaction[j].transaction_id, false, true));
            }
        }
        pf("\n");
        for(int j = 0; j < data_table[i].concurrent_transaction.size(); j++)
        {
            if(data_table[i].concurrent_transaction[j].transaction_type == READ)
            {
                data_table[i].lock_type = SHARED_LOCK;
            }
            if(data_table[i].concurrent_transaction[j].transaction_type == WRITE)
            {
                data_table[i].lock_type = EXCLUSIVE_LOCK;
                break;
            }
        }
    }

    for(int i = 1; i < transaction_status.size(); i++)
    {
        vector<int>temp;
        if(i != transaction_id)
        {
            for(int j = 0; j < graph[i].size(); j++)
            {
                if(graph[i][j] != transaction_id)
                {
                    temp.pb(graph[i][j]);
                }
            }
        }
        graph[i] = temp;
    }
}

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
    transaction_status.pb(NOT_PROCESSED);
    mark.pb(0);
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
        if(transaction_status[i] == NOT_PROCESSED)
        {
            pf("Transaction: %2d >> ", i);
            for(int j = 0; j < graph[i].size(); j++)
            {
                pf("%2d", graph[i][j]);
            }
            pf("\n");
        }
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
        processTransaction(data_id, transaction_type);
    }
    else
    {
        transaction.transaction_status = NOT_GRANTED;
        data_table[data_id].waiting_transaction.push_back(transaction);
        addEdge(data_id, transaction_id, transaction_type);
        printGraph();
        if(hasCycle())
        {
            processRollbackOrCommit(transaction_id, true);
        }
    }
}

void processStart()
{
    int transaction_id;
    scanf("%d", &transaction_id);
    transaction_id = getTransactionId(transaction_id, true, false);
    vector<int>temp;
    graph.pb(temp);
    transaction_status.pb(NOT_PROCESSED);
    mark.pb(0);
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
    int transaction_id;
    scanf("%d", &transaction_id);
    transaction_id = getTransactionId(transaction_id, false, false);
    processRollbackOrCommit(transaction_id, false);
}

string getLockType(int lock_type)
{
    if(lock_type == NO_LOCK)
    {
        return "No Lock";
    }
    if(lock_type == SHARED_LOCK)
    {
        return "Shared Lock";
    }
    if(lock_type == EXCLUSIVE_LOCK)
    {
        return "Exclusive Lock";
    }
}

void processEof()
{
    pf("Result = %d\n\n", originalData);
    pf("Lock Table\n");
    pf("=================\n");
    for(int i = 0; i < 31; i++)
    {
        if(data_table[i].lock_type == NO_LOCK)
        {
            continue;
        }
        pf("Data ID -> %2d\n", i);
        pf("        -> Lock Type                = %s\n", getLockType(data_table[i].lock_type).c_str());
        pf("        -> Lock Granted Transaction = ");
        unordered_set<int>s;
        for(int j = 0; j < data_table[i].concurrent_transaction.size(); j++)
        {
            s.insert(data_table[i].concurrent_transaction[j].transaction_id);
        }
        vector<int>temp;
        temp.assign(s.begin(), s.end());
        for(int j = 0; j < temp.size(); j++)
        {
            if(j)
            {
                pf(", ");
            }
            pf("%-4d", getTransactionId(temp[j], false, true));
        }
        if(!data_table[i].concurrent_transaction.size())
        {
            pf("None");
        }
        pf("\n");

        s.clear();
        temp.clear();
        for(int j = 0; j < data_table[i].waiting_transaction.size(); j++)
        {
            s.insert(data_table[i].waiting_transaction[j].transaction_id);
        }
        temp.assign(s.begin(), s.end());
        pf("        -> Waiting Transaction      = ");
        for(int j = 0; j < temp.size(); j++)
        {
            if(j)
            {
                pf(", ");
            }
            pf("%-4d", getTransactionId(temp[j], false, true));
        }
        if(!data_table[i].waiting_transaction.size())
        {
            pf("None");
        }
        pf("\n");
    }
    pf("\n\n\n");
    pf("Waiting Table\n");
    pf("=================\n");
    pf("Transaction - Waiting For\n");
    for(int i = 1; i < graph.size(); i++)
    {
        if(transaction_status[i] == NOT_PROCESSED)
        {
            unordered_set<int>s(graph[i].begin(), graph[i].end());
            graph[i].assign(s.begin(), s.end());
            pf("%6d      - ", getTransactionId(i, false, true));
            for(int j = 0; j < graph[i].size(); j++)
            {
                if(j)
                {
                    pf(", ");
                }
                pf("%-4d", getTransactionId(graph[i][j], false, true));
            }
            if(!graph[i].size())
            {
                pf("None");
            }
            pf("\n");
        }
    }
    pf("\n\n");
    exit(0);
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
        //printGraph();
        printf("Has Cycle = %d\n", hasCycle());
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



0
Start 1
Start 2
Start 3
Start 4
Read  0 1
Write 0 2
Read  1 2
Write 1 3
Read  2 3
Read  2 4
Write 2 1






132
start 0
start 1
start 3
start 4
start 5
read 2 0
read 2 1
write 7 0
read 7 1
commit 0
read 6 4
read 6 5
commit 1
EOF



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
