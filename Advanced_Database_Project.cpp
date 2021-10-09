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

        if(colwidth.size() == 0)
        {
            return;
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

        for(int i = 0; i < header.size(); i++)
        {
            cout << " | " << setw(colwidth[i]) << header[i];
        }
        cout << " | " << endl;

        cout << " ";
        for(int i = 0; i < colwidth.size(); i++)
        {
            cout  << "+--";
            for(int j = 0; j < colwidth[i]; j++)
            {
                cout << "-";
            }
        }
        cout << "+" << endl;

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


class Cross
{
private:
    File file1;
    File file2;
    OutputTable outputTable;

public:
    Cross(string fileName1, string fileName2)
    {
        this->file1.setFileName(fileName1);
        this->file2.setFileName(fileName2);
    }

    void readRows()
    {
        Projection projection1 = Projection(this->file1.getFileName());
        Projection projection2 = Projection(this->file2.getFileName());

        projection1.readRows();
        projection2.readRows();

        OutputTable outputTable1 = projection1.getOutputTable();
        OutputTable outputTable2 = projection2.getOutputTable();

        vector<int>colwidth1 = outputTable1.getColumnWidth();
        vector<int>colwidth2 = outputTable2.getColumnWidth();

        if(colwidth1.size() == 0 || colwidth2.size() == 0)
        {
            return;
        }

        for(int i = 0; i < colwidth1.size(); i++)
        {
            this->outputTable.addColumnWidth(colwidth1[i]);
        }
        for(int i = 0; i < colwidth2.size(); i++)
        {
            this->outputTable.addColumnWidth(colwidth2[i]);
        }

        vector<string>header1 = outputTable1.getOutputHeader();
        vector<string>header2 = outputTable2.getOutputHeader();

        for(int i = 0; i < header1.size(); i++)
        {
            this->outputTable.addOutputHeader(header1[i]);
        }
        for(int i = 0; i < header2.size(); i++)
        {
            this->outputTable.addOutputHeader(header2[i]);
        }

        vector<vector<int>>rows1 = outputTable1.getOutputRows();
        vector<vector<int>>rows2 = outputTable2.getOutputRows();

        for(int i = 0; i < rows1.size(); i++)
        {
            for(int j = 0; j < rows2.size(); j++)
            {
                vector<int>rows;
                for(int k = 0; k < header1.size(); k++)
                {
                    rows.pb(rows1[i][k]);
                }
                for(int k = 0; k < header2.size(); k++)
                {
                    rows.pb(rows2[j][k]);
                }
                this->outputTable.addOutputRows(rows);
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

void processCrossQuery()
{
    string fileName1, fileName2;

    cin >> fileName1 >> fileName2;
    Cross cross(fileName1, fileName2);

    cross.readRows();
    cross.printTable();
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
        else if(query == "cross")
        {
            processCrossQuery();
        }
    }

    return 0;
}
