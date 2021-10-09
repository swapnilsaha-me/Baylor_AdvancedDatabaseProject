#include<bits/stdc++.h>
using namespace std;

#define sf      scanf
#define pf      printf
#define pb      push_back

/*
    Projection:

    1. Will be there any null values for any column / input column?
    2. Will be there any empty line at the beginning or in between rows?
    3. Will be there any header which is null?
    4. If there is an input column which is not present in the table, then what to do?
    5. If the file doesn't exist, then what to do?
    6. Will be there any space in header name?
*/

string getLineCommaReplaceWithSpace(string str)
{
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] == ',')
        {
            str[i] = ' ';
        }
    }
    return str;
}

class File
{
private:
    string fileName;

public:
    fstream fin;

    void setFileName(string fileName)
    {
        this->fileName = fileName;
    }
    string getFileName()
    {
        return this->fileName;
    }
    void openFile()
    {
        this->fin.open(this->fileName, ios::in);
    }
};

class OutputTable
{
private:
    vector<int>columnWidth;
    vector<string>outputHeader;
    vector<vector<int>>outputRows;

public:
    void addColumnWidth(int width)
    {
        this->columnWidth.pb(width);
    }
    void setColumnWidth(int index, int width)
    {
        if(index < this->columnWidth.size())
        {
            this->columnWidth[index] = max(this->columnWidth[index], width);
        }
    }
    vector<int> getColumnWidth()
    {
        return this->columnWidth;
    }
    void addOutputHeader(string header)
    {
        this->outputHeader.pb(header);
    }
    void addOutputRows(vector<int> rows)
    {
        this->outputRows.pb(rows);
    }
    vector<string> getOutputHeader()
    {
        return this->outputHeader;
    }
    vector<vector<int>> getOutputRows()
    {
        return this->outputRows;
    }
    void printTable()
    {
        vector<int>colwidth = this->columnWidth;
        vector<string>header = this->outputHeader;
        vector<vector<int>>rows = this->outputRows;

        cout << " ";
        for(int i = 0; i < colwidth.size(); i++)
        {
            cout  << "---";
            for(int j = 0; j < colwidth[i]; j++)
            {
                cout << "-";
            }
        }
        cout << "-" << endl;

        for(int i = 0; i < header.size(); i++)
        {
            cout << " | " << setw(colwidth[i]) << header[i];
        }
        cout << " | " << endl;

        for(int i = 0; i < rows.size(); i++)
        {
            for(int j = 0; j < rows[i].size(); j++)
            {
                cout << " | " << setw(colwidth[j]) << rows[i][j];
            }
            cout << " | " << endl;
        }

        cout << " ";
        for(int i = 0; i < colwidth.size(); i++)
        {
            cout  << "---";
            for(int j = 0; j < colwidth[i]; j++)
            {
                cout << "-";
            }
        }
        cout << "-" << endl;
    }
};

class Projection
{
private:
    File file;
    OutputTable outputTable;

    bool isThereInputHeader = false;
    map<string, bool>markInputHeader;
    map<int, bool>markInputHeaderIndex;

    void processHeaders(string headerRow)
    {
        headerRow = getLineCommaReplaceWithSpace(headerRow);
        string columnName;

        stringstream ss(headerRow);
        int totalColumnCnt = 0;
        while(ss >> columnName)
        {
            if(this->isThereInputHeader)
            {
                if(this->markInputHeader[columnName])
                {
                    this->markInputHeaderIndex[totalColumnCnt] = 1;
                    this->outputTable.addOutputHeader(columnName);
                    this->outputTable.addColumnWidth(columnName.size());
                }
            }
            else
            {
                this->markInputHeaderIndex[totalColumnCnt] = 1;
                this->outputTable.addOutputHeader(columnName);
                this->outputTable.addColumnWidth(columnName.size());
            }
            totalColumnCnt++;
        }
    }

    void processRows(string valueRow)
    {
        valueRow = getLineCommaReplaceWithSpace(valueRow);
        vector<int>rows;
        int value;

        stringstream ss(valueRow);

        vector<string>headers = this->outputTable.getOutputHeader();
        int totalColumnCnt = 0, selectedColumnCnt = 0;
        while(ss >> value)
        {
            if(this->markInputHeaderIndex[totalColumnCnt++])
            {
                rows.pb(value);
                this->outputTable.setColumnWidth(selectedColumnCnt++, log10(value) + 1);
            }
        }
        this->outputTable.addOutputRows(rows);
    }

public:
    Projection(string fileName)
    {
        this->file.setFileName(fileName);
    }

    void addHeader(string header)
    {
        this->isThereInputHeader = true;
        this->markInputHeader[header] = 1;
    }

    void readRows()
    {
        string tableRows;
        bool isHeader = true;
        this->file.openFile();
        while(this->file.fin >> tableRows)
        {
            if(isHeader)
            {
                processHeaders(tableRows);
                isHeader = false;
            }
            else
            {
                processRows(tableRows);
            }
        }
    }

    void printTable()
    {
        this->outputTable.printTable();
    }

    OutputTable getOutputTable()
    {
        return this->outputTable;
    }
};

void processProjectQuery()
{
    string fileName;
    string parameter;

    cin >> fileName;
    Projection projection(fileName);

    getline(cin, parameter);
    stringstream ss(parameter);

    while(ss >> parameter)
    {
        projection.addHeader(parameter);
    }

    projection.readRows();
    projection.printTable();
}

int main()
{
    string query;

    while(cin >> query)
    {
        if(query == "project")
        {
            processProjectQuery();
        }
    }

    return 0;
}
