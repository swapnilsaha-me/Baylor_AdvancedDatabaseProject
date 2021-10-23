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
    7. What will be the output if multiple rows provide same pairs of tuple?
        Should we output all? Or only one row?
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
    ofstream fout;

    void setFileName(string fileName)
    {
        this->fileName = fileName;
    }
    string getFileName()
    {
        return this->fileName;
    }
    void openFileForInput()
    {
        this->fin.open(this->fileName, ios::in);
    }
    void openFileForOutput()
    {
        this->fout.open(this->fileName, ios::out);
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
        cout << "~~ " << rows.size() << " rows returned.\n\n\n" << endl;
    }
};

class Projection
{
private:
    File file;
    OutputTable outputTable;
    File outputFile;

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
    Projection(string fileName, string outputFileName)
    {
        this->file.setFileName(fileName);
        this->outputFile.setFileName(outputFileName);
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
        this->file.openFileForInput();
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

    void saveOutput()
    {
        this->outputFile.openFileForOutput();
        vector<string>outputHeader = this->outputTable.getOutputHeader();
        vector<vector<int>>outputRows = this->outputTable.getOutputRows();
        for(int i = 0; i < outputHeader.size(); i++)
        {
            if(i)
            {
                this->outputFile.fout << ",";
            }
            this->outputFile.fout << outputHeader[i];
        }
        this->outputFile.fout << endl;

        for(int i = 0; i < outputRows.size(); i++)
        {
            for(int j = 0; j < outputRows[i].size(); j++)
            {
                if(j)
                {
                    this->outputFile.fout << ",";
                }
                this->outputFile.fout << outputRows[i][j];
            }
            this->outputFile.fout << endl;
        }
    }
};

/*
    Cross:
        1. If we have time, then we will try to implement cross for multiple table.
*/

class Cross
{
private:
    File file[2];
    OutputTable processedTable[2];
    OutputTable outputTable;
    File outputFile;
    map<string, int>markHeader;

    void processHeaders(string headerRow, int serailNo)
    {
        headerRow = getLineCommaReplaceWithSpace(headerRow);
        string columnName;

        stringstream ss(headerRow);
        while(ss >> columnName)
        {
            this->markHeader[columnName]++;
            this->processedTable[serailNo].addOutputHeader(columnName);
            this->processedTable[serailNo].addColumnWidth(columnName.size());
        }
    }

    void processRows(string valueRow, int serailNo)
    {
        valueRow = getLineCommaReplaceWithSpace(valueRow);
        vector<int>rows;
        int value;

        stringstream ss(valueRow);

        int selectedColumnCnt = 0;
        while(ss >> value)
        {
            rows.pb(value);
            this->processedTable[serailNo].setColumnWidth(selectedColumnCnt++, log10(value) + 1);
        }
        this->processedTable[serailNo].addOutputRows(rows);
    }

public:
    Cross(string fileName1, string fileName2, string outputFileName)
    {
        this->file[0].setFileName(fileName1);
        this->file[1].setFileName(fileName2);
        this->outputFile.setFileName(outputFileName);
    }

    void readRows()
    {
        string tableRows;
        bool isHeader;

        for(int serialNo= 0; serialNo < 2; serialNo++)
        {
            isHeader = true;
            this->file[serialNo].openFileForInput();
            while(this->file[serialNo].fin >> tableRows)
            {
                if(isHeader)
                {
                    processHeaders(tableRows, serialNo);
                    isHeader = false;
                }
                else
                {
                    processRows(tableRows, serialNo);
                }
            }
        }

        for(int serialNo = 0; serialNo < 2; serialNo++)
        {
            string fileName = this->file[serialNo].getFileName();
            size_t pos = fileName.find(".csv");
            fileName = (pos == std::string::npos) ? fileName : fileName.replace(pos, 4, "");
            if(this->file[0].getFileName() == this->file[1].getFileName())
            {
                fileName += (serialNo + '0' + 1);
            }

            vector<string>headers = this->processedTable[serialNo].getOutputHeader();
            for(int i = 0; i < headers.size(); i++)
            {
                if(this->markHeader[headers[i]] > 1)
                {
                    string columnName = fileName + "." + headers[i];
                    this->outputTable.addOutputHeader(columnName);

                    int columnWidth = max((int)columnName.size(), this->processedTable[serialNo].getColumnWidth()[i]);
                    this->outputTable.addColumnWidth(columnWidth);
                }
                else
                {
                    string columnName = headers[i];
                    this->outputTable.addOutputHeader(columnName);

                    int columnWidth = max((int)columnName.size(), this->processedTable[serialNo].getColumnWidth()[i]);
                    this->outputTable.addColumnWidth(columnWidth);
                }
            }
        }

        for(int i = 0; i < this->processedTable[0].getOutputRows().size(); i++)
        {
            for(int j = 0; j < this->processedTable[1].getOutputRows().size(); j++)
            {
                vector<int>rows;
                for(int k = 0; k < this->processedTable[0].getOutputRows()[i].size(); k++)
                {
                    rows.pb(this->processedTable[0].getOutputRows()[i][k]);
                }
                for(int k = 0; k < this->processedTable[1].getOutputRows()[j].size(); k++)
                {
                    rows.pb(this->processedTable[1].getOutputRows()[j][k]);
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

    void saveOutput()
    {
        this->outputFile.openFileForOutput();
        vector<string>outputHeader = this->outputTable.getOutputHeader();
        vector<vector<int>>outputRows = this->outputTable.getOutputRows();
        for(int i = 0; i < outputHeader.size(); i++)
        {
            if(i)
            {
                this->outputFile.fout << ",";
            }
            this->outputFile.fout << outputHeader[i];
        }
        this->outputFile.fout << endl;

        for(int i = 0; i < outputRows.size(); i++)
        {
            for(int j = 0; j < outputRows[i].size(); j++)
            {
                if(j)
                {
                    this->outputFile.fout << ",";
                }
                this->outputFile.fout << outputRows[i][j];
            }
            this->outputFile.fout << endl;
        }
    }
};


void processProjectQuery()
{
    string fileName;
    string outputFileName;
    string parameter;

    cin >> fileName >> outputFileName;
    Projection projection(fileName, outputFileName);

    getline(cin, parameter);
    stringstream ss(parameter);

    while(ss >> parameter)
    {
        projection.addHeader(parameter);
    }

    projection.readRows();
    projection.printTable();
    projection.saveOutput();
}

void processCrossQuery()
{
    string fileName1, fileName2, outputFileName;

    cin >> fileName1 >> fileName2 >> outputFileName;
    Cross cross(fileName1, fileName2, outputFileName);

    cross.readRows();
    cross.printTable();
    cross.saveOutput();
}

int main()
{
    string query;

    while(cout << "Query >> ", cin >> query)
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
