#include<bits/stdc++.h>
using namespace std;

#define sf              scanf
#define pf              printf
#define pb              push_back
#define MAX_POINTER     100

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

bool pair_compare_less(pair<int, int>a, pair<int, int>b)
{
    if(a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first < b.first;
}

bool file_exists(string fileName)
{
    ifstream ifile;
    ifile.open(fileName);
    if(ifile)
    {
        return true;
    }
    return false;
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
        this->fin.open(this->fileName, ios_base::in);
    }
    void openFileForOutput()
    {
        this->fout.open(this->fileName, ios_base::out | ios_base::trunc);
    }
    void closeFileForInput()
    {
        this->fin.close();
    }
    void closeFileForOutput()
    {
        this->fout.close();
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
    void setOutputHeaders(vector<string>headers)
    {
        this->outputHeader = headers;
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
        cout << "~~ " << rows.size() << " rows returned." << endl;
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
        this->file.closeFileForInput();
    }

    void setOutputHeaders(vector<string>headers)
    {
        this->outputTable.setOutputHeaders(headers);
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
        this->outputFile.closeFileForOutput();
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
            this->file[serialNo].closeFileForInput();
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
        this->outputFile.closeFileForOutput();
    }
};

/*
    BPlusTree
*/

class BPlusTreeNode
{
private:
    File file;
    int isOriginalData;
    int isLeaf;
    int numberOfPointers;
    int numberOfValues;
    vector<string>pointers;
    vector<pair<int, int>>values;
    vector<int>originalValue;

    void updateNumberOfPointers()
    {
        this->numberOfPointers = this->pointers.size();
    }

    void updateNumberOfValues()
    {
        this->numberOfValues = this->values.size();
    }

public:
    void setFileName(string fileName)
    {
        this->file.setFileName(fileName);
    }

    string getFileName()
    {
        return this->file.getFileName();
    }

    bool isOriginalDataNode()
    {
        return this->isOriginalData;
    }

    bool isLeafNode()
    {
        return this->isLeaf;
    }

    void updateOriginalDataStatus(bool isOriginalData)
    {
        this->isOriginalData = isOriginalData;
    }

    void updateLeafNodeStatus(bool isLeaf)
    {
        this->isLeaf = isLeaf;
    }

    bool isPageFull()
    {
        return this->numberOfValues == MAX_POINTER;
    }

    vector<string> getPointers()
    {
        return this->pointers;
    }

    vector<pair<int, int>> getValues()
    {
        return this->values;
    }

    void setPointers(vector<string>pointers)
    {
        this->pointers = pointers;
        this->updateNumberOfPointers();
    }

    void setValues(vector<pair<int, int>>values)
    {
        this->values = values;
        this->updateNumberOfValues();
    }

    void setOriginalValues(vector<int>originalValue)
    {
        this->originalValue = originalValue;
    }

    vector<int> getOriginalValues()
    {
        return this->originalValue;
    }

    void readRows()
    {
        this->file.openFileForInput();
        this->file.fin >> isOriginalData;
        if(this->isOriginalData)
        {
            int temp;
            while(this->file.fin >> temp)
            {
                this->originalValue.pb(temp);
            }
        }
        else
        {
            this->file.fin >> isLeaf;
            this->file.fin >> numberOfPointers;
            this->file.fin >> numberOfValues;
            bool isPointer = true;
            string fileName;
            pair<int, int>val;
            while(1)
            {
                if(isPointer)
                {
                    if(this->pointers.size() < this->numberOfPointers)
                    {
                        this->file.fin >> fileName;
                        this->pointers.pb(fileName);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if(this->values.size() < this->numberOfValues)
                    {
                        this->file.fin >> val.first >> val.second;
                        this->values.pb(val);
                    }
                    else
                    {
                        break;
                    }
                }
                isPointer = !isPointer;
            }
        }
        this->file.closeFileForInput();
    }

    void writeRows()
    {
        this->file.openFileForOutput();
        this->file.fout << this->isOriginalData << endl;
        if(this->isOriginalData)
        {
            for(int i = 0; i < this->originalValue.size(); i++)
            {
                if(i)
                {
                    this->file.fout << " ";
                }
                this->file.fout << this->originalValue[i];
            }
            this->file.fout << endl;
        }
        else
        {
            this->file.fout << this->isLeaf << endl;
            this->file.fout << this->numberOfPointers << endl;
            this->file.fout << this->numberOfValues << endl;
            for(int i = 0, j = 0, k = 0; i < this->pointers.size() + this->values.size(); i++)
            {
                if(i & 1)
                {
                    this->file.fout << this->values[j].first << " " << this->values[j].second << endl;
                    j++;
                }
                else
                {
                    this->file.fout << this->pointers[k] << endl;
                    k++;
                }
            }
        }
        this->file.closeFileForOutput();
    }
};

class BPlusTree
{
private:
    File metadata;
    string filePrefix;
    string nextFileSerial;
    string headers;
    string rootFileName;
    vector<int>originalDataToInsert;

    void incrementNextFileSerial()
    {
        int num = 0;
        for(int i = 0; i < nextFileSerial.size(); i++)
        {
            num = (num * 10) + (nextFileSerial[i] - '0');
        }
        num++;
        string temp;
        while(num)
        {
            temp += ((num % 10) + '0');
            num /= 10;
        }
        if(temp == "")
        {
            temp += "0";
        }
        reverse(temp.begin(), temp.end());
        nextFileSerial = temp;
    }

public:

    void setMetaDataFileName(string fileName)
    {
        this->metadata.setFileName(fileName);
    }

    void setFilePrefix(string filePrefix)
    {
        this->filePrefix = filePrefix;
    }

    string getFilePrefix()
    {
        return this->filePrefix;
    }

    string getNextFileNameWithPrefix()
    {
        string nextFileName = this->filePrefix + this->nextFileSerial;
        incrementNextFileSerial();
        writeMetaData();
        return nextFileName;
    }

    void setRootFileName(string rootFileName)
    {
        this->rootFileName = rootFileName;
    }

    void setNextFileSerial(string nextFileSerial)
    {
        this->nextFileSerial = nextFileSerial;
    }

    string getHeaders()
    {
        return this->headers;
    }

    void setHeaders(string headers)
    {
        this->headers = headers;
    }

    string getRootFileName()
    {
        return this->rootFileName;
    }

    string getNextFileSerial()
    {
        return this->getNextFileSerial();
    }

    void setOriginalDataToBeInserted(vector<int>originalDataToInsert)
    {
        this->originalDataToInsert = originalDataToInsert;
    }

    void readMetaData()
    {
        this->metadata.openFileForInput();
        this->metadata.fin >> nextFileSerial;
        this->metadata.fin >> filePrefix;
        this->metadata.fin >> rootFileName;
        string temp, newHeaders;
        while(this->metadata.fin >> temp)
        {
            if(newHeaders.size())
            {
                newHeaders += " ";
            }
            newHeaders += temp;
        }
        headers = newHeaders;
        //cout << nextFileSerial << " : " << filePrefix << " : " << rootFileName << " : " << headers << endl;
        this->metadata.closeFileForInput();
    }

    void writeMetaData()
    {
        this->metadata.openFileForOutput();
        this->metadata.fout << nextFileSerial << endl;
        this->metadata.fout << filePrefix << endl;
        this->metadata.fout << rootFileName << endl;
        this->metadata.fout << headers << endl;
        this->metadata.closeFileForOutput();
    }

    string insertOriginalDataAndGetFileName()
    {
        BPlusTreeNode bPlusTreeNodeOriginalData;
        bPlusTreeNodeOriginalData.setFileName(this->getNextFileNameWithPrefix());
        bPlusTreeNodeOriginalData.updateOriginalDataStatus(1);
        bPlusTreeNodeOriginalData.setOriginalValues(this->originalDataToInsert);
        bPlusTreeNodeOriginalData.writeRows();
        return bPlusTreeNodeOriginalData.getFileName();
    }

    pair<pair<int, int>, string> insertData(pair<int, int>insertValue, string currentNodeFileName)
    {
        //cout << "currentNodeFileName: " << currentNodeFileName << endl;
        //return;
        BPlusTreeNode currentNode;
        currentNode.setFileName(currentNodeFileName);
        currentNode.readRows();
        pair<File, pair<int, int>> insertNode;
        vector<string> pointers = currentNode.getPointers();
        vector<pair<int, int>>values = currentNode.getValues();

        //Leaf node
        if(currentNode.isLeafNode())
        {
            bool isInserted = false;
            int loc = 0;
            for(int i = 0; i < values.size(); i++)
            {
                if(pair_compare_less(insertValue, values[i]))
                {
                    isInserted = true;
                    loc = i;
                    break;
                }
            }
            if(!isInserted)
            {
                loc = values.size();
            }

            string originalDataFileName = insertOriginalDataAndGetFileName();
            pointers.insert(pointers.begin() + loc, originalDataFileName);
            values.insert(values.begin() + loc, insertValue);
            currentNode.setPointers(pointers);
            currentNode.setValues(values);
            currentNode.writeRows();

            if(currentNode.isPageFull())
            {
                BPlusTreeNode leftNode, rightNode;

                leftNode.setFileName(currentNode.getFileName());
                rightNode.setFileName(this->getNextFileNameWithPrefix());

                leftNode.updateOriginalDataStatus(0);
                rightNode.updateOriginalDataStatus(0);

                leftNode.updateLeafNodeStatus(1);
                rightNode.updateLeafNodeStatus(1);

                vector<string>leftPointers, rightPointers;
                vector<pair<int, int>>leftValues, rightValues;

                for(int i = 0; i < currentNode.getValues().size(); i++)
                {
                    if(i < (MAX_POINTER / 2))
                    {
                        leftPointers.pb(currentNode.getPointers()[i]);
                        leftValues.pb(currentNode.getValues()[i]);
                    }
                    else
                    {
                        rightPointers.pb(currentNode.getPointers()[i]);
                        rightValues.pb(currentNode.getValues()[i]);
                    }
                }
                leftPointers.pb(rightNode.getFileName());
                rightPointers.pb(currentNode.getPointers()[currentNode.getPointers().size() - 1]);

                //cout << rightNode.getFileName() << ", " << currentNode.getPointers()[currentNode.getPointers().size() - 1] << endl;

                leftNode.setPointers(leftPointers);
                rightNode.setPointers(rightPointers);

                leftNode.setValues(leftValues);
                rightNode.setValues(rightValues);

                //cout << "FileName: " << leftNode.getFileName() << ", " << rightNode.getFileName() << endl;
                //cout << "Pointers: " << leftNode.getPointers().size() << ", " << rightNode.getPointers().size() << endl;
                //cout << "Values: " << leftNode.getValues().size() << ", " << rightNode.getValues().size() << endl;

                leftNode.writeRows();
                rightNode.writeRows();

                return make_pair(rightValues[0], rightNode.getFileName());
            }
            return make_pair(make_pair(0, 0), "");
        }
        //Non-Leaf Node
        else
        {
            bool isFound = false;
            string childFileName;
            for(int i = 0; i < values.size(); i++)
            {
                if(pair_compare_less(insertValue, values[i]))
                {
                    isFound = true;
                    childFileName = pointers[i];
                    break;
                }
            }
            if(!isFound)
            {
                childFileName = pointers[pointers.size() - 1];
            }

            pair<pair<int, int>, string> pointerToBeAdded;
            pointerToBeAdded = insertData(insertValue, childFileName);

            if(pointerToBeAdded.second.size())
            {
                bool isInserted = false;
                int loc = 0;
                for(int i = 0; i < values.size(); i++)
                {
                    if(pair_compare_less(pointerToBeAdded.first, values[i]))
                    {
                        isInserted = true;
                        loc = i;
                        break;
                    }
                }
                if(!isInserted)
                {
                    loc = values.size();
                }

                pointers.insert(pointers.begin() + loc + 1, pointerToBeAdded.second);
                values.insert(values.begin() + loc, pointerToBeAdded.first);
                currentNode.setPointers(pointers);
                currentNode.setValues(values);
                currentNode.writeRows();

                if(currentNode.isPageFull())
                {
                    BPlusTreeNode leftNode, rightNode;

                    leftNode.setFileName(currentNode.getFileName());
                    rightNode.setFileName(this->getNextFileNameWithPrefix());

                    leftNode.updateOriginalDataStatus(0);
                    rightNode.updateOriginalDataStatus(0);

                    leftNode.updateLeafNodeStatus(0);
                    rightNode.updateLeafNodeStatus(0);

                    vector<string>leftPointers, rightPointers;
                    vector<pair<int, int>>leftValues, upperValues, rightValues;

                    for(int i = 0; i < currentNode.getPointers().size(); i++)
                    {
                        if(i <= (MAX_POINTER / 2))
                        {
                            leftPointers.pb(currentNode.getPointers()[i]);
                        }
                        else
                        {
                            rightPointers.pb(currentNode.getPointers()[i]);
                        }
                    }

                    for(int i = 0; i < currentNode.getValues().size(); i++)
                    {
                        if(i < (MAX_POINTER / 2))
                        {
                            leftValues.pb(currentNode.getValues()[i]);
                        }
                        else if(i == (MAX_POINTER / 2))
                        {
                            upperValues.pb(currentNode.getValues()[i]);
                        }
                        else
                        {
                            rightValues.pb(currentNode.getValues()[i]);
                        }
                    }

                    //cout << rightNode.getFileName() << ", " << currentNode.getPointers()[currentNode.getPointers().size() - 1] << endl;

                    leftNode.setPointers(leftPointers);
                    rightNode.setPointers(rightPointers);

                    leftNode.setValues(leftValues);
                    rightNode.setValues(rightValues);

                    //cout << "FileName: " << leftNode.getFileName() << ", " << rightNode.getFileName() << endl;
                    //cout << "Pointers: " << leftNode.getPointers().size() << ", " << rightNode.getPointers().size() << endl;
                    //cout << "Values: " << leftNode.getValues().size() << ", " << rightNode.getValues().size() << endl;

                    leftNode.writeRows();
                    rightNode.writeRows();

                    return make_pair(upperValues[0], rightNode.getFileName());
                }
            }
        }
        return make_pair(make_pair(0, 0), "");
    }

    OutputTable searchData(pair<int, int>searchValue, string currentNodeFileName, bool isAlreadyMatched, bool isAllLeft)
    {
        //cout << "currentNodeFileName: " << currentNodeFileName << endl;
        //return;
        BPlusTreeNode currentNode;
        currentNode.setFileName(currentNodeFileName);
        currentNode.readRows();

        vector<string> pointers = currentNode.getPointers();
        vector<pair<int, int>>values = currentNode.getValues();

        //Original data
        if(currentNode.isLeafNode())
        {
            //cout << "Reached" << endl;
            OutputTable outputTable;

            string header = getLineCommaReplaceWithSpace(this->getHeaders());
            stringstream ss(header);

            while(ss >> header)
            {
                outputTable.addOutputHeader(header);
                outputTable.addColumnWidth(header.size());
            }

            bool done = false;
            while(!done)
            {
                for(int i = 0; i < values.size(); i++)
                {
                    //cout << "Check: " << searchValue.first << ", " << values[i].first << endl;
                    if(searchValue.first < values[i].first)
                    {
                        done = true;
                        break;
                    }
                    if(searchValue.first == values[i].first)
                    {
                        BPlusTreeNode node;
                        node.setFileName(pointers[i]);
                        node.readRows();
                        outputTable.addOutputRows(node.getOriginalValues());
                        for(int j = 0; j < node.getOriginalValues().size(); j++)
                        {
                            outputTable.setColumnWidth(j, log10(node.getOriginalValues()[j]) + 1);
                        }
                    }
                }

                if(done)
                {
                    break;
                }

                //cout << "Last : " << pointers[pointers.size() - 1] << endl;
                BPlusTreeNode temp;
                temp.setFileName(pointers[pointers.size() - 1]);
                temp.readRows();
                if(!temp.isOriginalDataNode())
                {
                    //cout << "Aschi" << endl;
                    pointers.clear();
                    values.clear();
                    pointers = temp.getPointers();
                    values = temp.getValues();
                }
                else
                {
                    done = true;
                }
            }
            return outputTable;
        }
        bool isFound = false;
        string childNode;

        for(int i = 0; i < values.size(); i++)
        {
            //cout << searchValue.first << " " << searchValue.second << endl;
            if(isAlreadyMatched || isAllLeft)
            {
                if(searchValue.first == values[i].first)
                {
                    isFound = true;
                    isAlreadyMatched = true;
                    isAllLeft = false;
                    childNode = pointers[i];
                    break;
                }
            }
            if(searchValue.first < values[i].first)
            {
                isFound = true;
                childNode = pointers[i];
                break;
            }
            if(searchValue.first == values[i].first)
            {
                isAlreadyMatched = true;
                isAllLeft = false;
                if(i + 1 < pointers.size())
                {
                    isFound = true;
                    childNode = pointers[i + 1];
                }
                break;
            }
        }
        if(!isFound)
        {
            childNode = pointers[pointers.size() - 1];
        }
        return searchData(searchValue, childNode, isAlreadyMatched, isAllLeft);
    }

    void printOriginalData(string parent, string ownFileName, vector<int>originalData)
    {
        for(int i = 0; i < 50; i++)
        {
            cout << "-";
        }
        cout << "\n";
        cout << " >> Parent  Pointer Name : ## " << parent << endl;
        cout << " >> Current Pointer Name : ## " << ownFileName << "\n" << endl;

        OutputTable outputTable;
        stringstream ss(this->getHeaders());
        vector<string>headers;
        string columnName;
        while(ss >> columnName)
        {
            headers.pb(columnName);
        }
        outputTable.setOutputHeaders(headers);
        outputTable.addOutputRows(originalData);
        for(int i = 0; i < headers.size(); i++)
        {
            outputTable.addColumnWidth(headers[i].size());
            outputTable.setColumnWidth(i, log10(originalData[i]) + 1);
        }
        outputTable.printTable();
        for(int i = 0; i < 50; i++)
        {
            cout << "-";
        }
        cout << "\n\n\n";
    }

    void printBPlusTreeNode(string parent, string ownFileName, vector<string> pointers, vector<pair<int, int>>values)
    {
        for(int i = 0; i < 50; i++)
        {
            cout << "-";
        }
        cout << "\n";
        cout << " >> Parent  Pointer Name : ## " << parent << endl;
        cout << " >> Current Pointer Name : ## " << ownFileName << "\n" << endl;
        for(int i = 0, j = 0, k = 0; i < pointers.size() + values.size(); i++)
        {
            if(i & 1)
            {
                cout << "Attribute Value : "  <<values[j].first << ", ROW_NUM : " << values[j].second << endl;
                j++;
            }
            else
            {
                cout << "## " << pointers[k] << endl;
                k++;
            }
        }
        for(int i = 0; i < 50; i++)
        {
            cout << "-";
        }
        cout << "\n\n\n";
    }

    void printBPlusTree()
    {
        this->readMetaData();
        queue<pair<string, string>>Q;
        Q.push(make_pair("NULL", this->rootFileName));
        while(!Q.empty())
        {
            pair<string, string> node = Q.front();
            string parentFileName = node.first;
            string fileName = node.second;

            Q.pop();

            BPlusTreeNode currentNode;
            currentNode.setFileName(fileName);
            currentNode.readRows();

            vector<string>pointers = currentNode.getPointers();
            vector<pair<int, int>>values = currentNode.getValues();

            if(currentNode.isOriginalDataNode())
            {
                printOriginalData(parentFileName, fileName, currentNode.getOriginalValues());
            }
            else
            {
                printBPlusTreeNode(parentFileName, fileName, pointers, values);
            }

            if(currentNode.isLeafNode() && pointers.size() > values.size())
            {
                for(int i = 0; i < pointers.size() - 1; i++)
                {
                    Q.push(make_pair(fileName, pointers[i]));
                }
            }
            else
            {
                for(int i = 0; i < pointers.size(); i++)
                {
                    Q.push(make_pair(fileName, pointers[i]));
                }
            }
        }
    }

};

/*
    Select:

*/

class Select
{
private:
    File file;
    BPlusTree bPlusTree;
    OutputTable outputTable;
    File outputFile;

    bool isBPlusTree;
    int searchValue;
    bool bPlusTreeAlreadyBuilt;
    string bPlusTreeColumn;
    map<string, bool>markInputHeader;
    vector<int>markedInputHeaderIndex;

    void createRootNode(vector<string>pointers, vector<pair<int, int>>values, bool isLeafNode)
    {
        BPlusTreeNode bPlusTreeNode;
        bPlusTreeNode.setFileName(bPlusTree.getNextFileNameWithPrefix());
        bPlusTreeNode.updateOriginalDataStatus(0);
        bPlusTreeNode.updateLeafNodeStatus(isLeafNode);
        bPlusTreeNode.setPointers(pointers);
        bPlusTreeNode.setValues(values);
        bPlusTreeNode.writeRows();
        bPlusTree.setRootFileName(bPlusTreeNode.getFileName());
        bPlusTree.writeMetaData();
    }

    void createMetaData(string metaDataFileName)
    {
        bPlusTree.setFilePrefix(metaDataFileName);
        bPlusTree.setRootFileName(metaDataFileName + "0");
        bPlusTree.setNextFileSerial("0");
        bPlusTree.writeMetaData();
    }

    void processHeaders(string headerRow)
    {
        headerRow = getLineCommaReplaceWithSpace(headerRow);

        string columnName;

        stringstream ss(headerRow);
        int totalColumnCnt = 0;
        while(ss >> columnName)
        {
            if(this->markInputHeader[columnName])
            {
                if(this->bPlusTreeColumn == "")
                {
                    this->bPlusTreeColumn = columnName;
                }
                this->markedInputHeaderIndex.push_back(totalColumnCnt);
            }
            this->outputTable.addOutputHeader(columnName);
            this->outputTable.addColumnWidth(columnName.size());
            totalColumnCnt++;
        }

        if(this->isBPlusTree)
        {
            this->bPlusTreeAlreadyBuilt = true;
            this->bPlusTree.setHeaders(headerRow);
            string metaDataFileName = this->file.getFileName() + "." + this->bPlusTreeColumn + ".btree";
            bPlusTree.setMetaDataFileName(metaDataFileName);
            if(!file_exists(metaDataFileName))
            {
                this->bPlusTreeAlreadyBuilt = false;
                createMetaData(metaDataFileName);
                vector<string>pointers;
                vector<pair<int, int>>pairs;
                createRootNode(pointers, pairs, 1);
            }
            bPlusTree.readMetaData();
        }
    }

    void processRowsLinearSearch(string valueRow)
    {
        valueRow = getLineCommaReplaceWithSpace(valueRow);
        vector<int>rows;
        int value;

        stringstream ss(valueRow);

        while(ss >> value)
        {
            rows.pb(value);
        }
        bool isInsert = false;
        if(this->markedInputHeaderIndex.size() == 2)
        {
            if(rows[this->markedInputHeaderIndex[0]] == rows[this->markedInputHeaderIndex[1]])
            {
                isInsert = true;
            }
        }

        if(isInsert)
        {
            for(int i = 0; i < rows.size(); i++)
            {
                this->outputTable.setColumnWidth(i, log10(rows[i]) + 1);
            }
            this->outputTable.addOutputRows(rows);
        }
    }

    void processRowsBPlusTree(string valueRow, int serial)
    {
        valueRow = getLineCommaReplaceWithSpace(valueRow);
        vector<int>rows;
        int value;

        stringstream ss(valueRow);

        while(ss >> value)
        {
            rows.pb(value);
        }

        bPlusTree.setOriginalDataToBeInserted(rows);
        pair<pair<int, int>, string>newRoot;
        newRoot = bPlusTree.insertData(make_pair(rows[this->markedInputHeaderIndex[0]], serial), bPlusTree.getRootFileName());
        if(newRoot.second.size())
        {
            vector<string>pointers;
            vector<pair<int, int>>values;
            pointers.pb(bPlusTree.getRootFileName());
            pointers.pb(newRoot.second);
            values.pb(newRoot.first);
            createRootNode(pointers, values, 0);
        }
    }

    void searchBPlusTree()
    {
        this->outputTable = bPlusTree.searchData(make_pair(this->searchValue, -1e9), bPlusTree.getRootFileName(), false, true);
    }

public:
    Select(string fileName, string outputFileName)
    {
        this->file.setFileName(fileName);
        this->outputFile.setFileName(outputFileName);
    }

    void addHeader(string header1, bool isBPlusTree, string header2, int searchValue)
    {
        this->isBPlusTree = isBPlusTree;
        this->markInputHeader[header1] = 1;
        if(this->isBPlusTree)
        {
            this->searchValue = searchValue;
        }
        else
        {
            this->markInputHeader[header2] = 1;
        }
    }

    void readRows()
    {
        string tableRows;

        this->file.openFileForInput();
        bool isMetaDataExist;

        if(this->file.fin >> tableRows)
        {
            processHeaders(tableRows);
        }

        if(this->isBPlusTree)
        {
            if(!this->bPlusTreeAlreadyBuilt)
            {
                int rowCounter = 0;
                while(this->file.fin >> tableRows)
                {
                    processRowsBPlusTree(tableRows, ++rowCounter);
                }
            }
            searchBPlusTree();
        }
        else
        {
            while(this->file.fin >> tableRows)
            {
                processRowsLinearSearch(tableRows);
            }
        }

        this->file.closeFileForInput();
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
        this->outputFile.closeFileForOutput();
    }
};


/*
    Join
*/

class Join
{
private:
    File file[2];
    OutputTable outputTable;
    File outputFile;
    string crossProductFileName;
    vector<string>allHeader;
    vector<string>commonHeader;

    void findCommonAttributes()
    {
        map<string, int>exist;
        map<string, int> :: iterator it;
        string tableRows, column;
        for(int serial = 0; serial < 2; serial++)
        {
            this->file[serial].openFileForInput();
            this->file[serial].fin >> tableRows;
            tableRows = getLineCommaReplaceWithSpace(tableRows);
            stringstream ss(tableRows);
            while(ss >> column)
            {
                exist[column]++;
            }
            this->file[serial].closeFileForInput();
        }
        for(it = exist.begin(); it != exist.end(); it++)
        {
            allHeader.pb(it->first);
            if(it->second == 2)
            {
                commonHeader.pb(it->first);
            }
        }
    }

    void applyCrossProduct()
    {
        this->crossProductFileName = this->outputFile.getFileName() + "_cross.csv";
        Cross cross = Cross(this->file[0].getFileName(), this->file[1].getFileName(), this->crossProductFileName);
        cross.readRows();
        cross.saveOutput();
    }

    void applySelectOperator(string inputFile, string outputFile, string parameter1, string parameter2)
    {
        Select select = Select(inputFile, outputFile);
        select.addHeader(parameter1, false, parameter2, 0);
        select.readRows();
        select.saveOutput();
    }

    void applyProjectOperator()
    {
        string fileName = this->outputFile.getFileName() + "_select" + ((char)(this->commonHeader.size() - 1 + '0')) + ".csv";
        string parameter;

        Projection projection(fileName, this->outputFile.getFileName());

        map<string, string>newName;
        for(int i = 0; i < this->allHeader.size(); i++)
        {
            bool isCommonHeader = false;
            for(int j = 0; j < this->commonHeader.size(); j++)
            {
                if(allHeader[i] == commonHeader[j])
                {
                    isCommonHeader = true;
                    break;
                }
            }
            if(isCommonHeader)
            {
                string fileName = this->file[0].getFileName();
                size_t pos = fileName.find(".csv");
                fileName = (pos == std::string::npos) ? fileName : fileName.replace(pos, 4, "");
                if(this->file[0].getFileName() == this->file[1].getFileName())
                {
                    fileName += (0 + '0' + 1);
                }
                parameter = fileName + "." + this->allHeader[i];
            }
            else
            {
                parameter = this->allHeader[i];
            }
            newName[parameter] = this->allHeader[i];
            projection.addHeader(parameter);
        }

        projection.readRows();


        vector<string>headers;
        OutputTable outputTable = projection.getOutputTable();
        for(int i = 0; i < outputTable.getOutputHeader().size(); i++)
        {
            string column = newName[outputTable.getOutputHeader()[i]];
            headers.pb(column);
        }

        projection.setOutputHeaders(headers);
        projection.printTable();
        projection.saveOutput();
    }

public:
    Join(string fileName1, string fileName2, string outputFileName)
    {
        this->file[0].setFileName(fileName1);
        this->file[1].setFileName(fileName2);
        this->outputFile.setFileName(outputFileName);
    }

    void printTable()
    {
        this->outputTable.printTable();
    }

    OutputTable getOutputTable()
    {
        return this->outputTable;
    }

    void readRows()
    {
        this->findCommonAttributes();
        this->applyCrossProduct();
        string inputFile, outputFile, param[2];
        for(int i = 0; i < this->commonHeader.size(); i++)
        {
            for(int j = 0; j < 2; j++)
            {
                string fileName = this->file[j].getFileName();
                size_t pos = fileName.find(".csv");
                fileName = (pos == std::string::npos) ? fileName : fileName.replace(pos, 4, "");
                if(this->file[0].getFileName() == this->file[1].getFileName())
                {
                    fileName += (0 + '0' + 1);
                }
                param[j] = fileName + "." + this->commonHeader[i];
            }

            if(i)
            {
                inputFile = this->outputFile.getFileName() + "_select" + ((char)(i - 1 + '0')) + ".csv";
            }
            else
            {
                inputFile = this->crossProductFileName;
            }
            outputFile = this->outputFile.getFileName() + "_select" + ((char)(i + '0')) + ".csv";
            this->applySelectOperator(inputFile, outputFile, param[0], param[1]);
        }
        this->applyProjectOperator();
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
        this->outputFile.closeFileForOutput();
    }
};


bool checkFileExistence(string filename)
{
    if(!std::fstream{filename})
    {
        return false;
    }
    return true;

}

void raiseFileNotFoundError(string filename)
{
    cout<<"ERROR: '"<< filename <<"' not found."<<endl;
    cout<<"Fix 1: Check file name."<<endl;
    cout<<"Fix 2: File Extension (.csv) is required."<<endl;
    cout<<"Fix 3: Make sure your file is in the same folder as Advanced_Database_Project.exe file."<<endl;
    cout<<"Fix 4: Make sure your file is not open."<<endl;

    cout<<endl;
}

void raiseBtreeFileNotFoundError(string filename) {
    cout<<"ERROR: B+ tree '"<< filename <<"' not found."<<endl;
    cout<<"Fix 1: Check file name."<<endl;
    cout<<"Fix 2: File Extension (.btree) is required."<<endl;
}

void raiseInvalidParameterError() {
    cout<<"ERROR: Invalid table parameter."<<endl<<endl;
}

bool checkParameters(string fileName, vector<string>headers)
{
    File file;
    file.setFileName(fileName);
    file.openFileForInput();
    string tableRows, columnName;
    file.fin >> tableRows;
    tableRows = getLineCommaReplaceWithSpace(tableRows);
    stringstream ss(tableRows);
    map<string, bool>mark;
    while(ss >> columnName)
    {
        mark[columnName] = true;
    }
    for(int i = 0; i < headers.size(); i++)
    {
        if(!mark[headers[i]])
        {
            return false;
        }
    }
    return true;
}

double getProcessDuration(std::clock_t start) {
    return ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
}

void showProcessTime(double duration) {
    cout<<"(In "<<duration<<" sec(s))"<<endl<<endl;
}

void queryProcessingTimerMsg() {
    cout<< "Query processing. Please wait. . ."<<endl;
}
void processProjectQuery()
{
    string fileName;
    string outputFileName;
    string parameter;

    cin >> fileName >> outputFileName;

    queryProcessingTimerMsg();

    bool fileExists = checkFileExistence(fileName);

    if(!fileExists)
    {
        raiseFileNotFoundError(fileName);
        return;
    }

    vector<string>headers;

    Projection projection(fileName, outputFileName);

    getline(cin, parameter);
    stringstream ss(parameter);

    while(ss >> parameter)
    {
        headers.pb(parameter);
        projection.addHeader(parameter);
    }

    if(!checkParameters(fileName, headers))
    {
        raiseInvalidParameterError();
        return;
    }

    std::clock_t start;
    start = std::clock();

    projection.readRows();
    projection.printTable();
    showProcessTime(getProcessDuration(start));

    projection.saveOutput();
}


void processCrossQuery()
{
    string fileName1, fileName2, outputFileName;

    cin >> fileName1 >> fileName2 >> outputFileName;

    queryProcessingTimerMsg();

    bool file1Exists = checkFileExistence(fileName1);
    bool file2Exists = checkFileExistence(fileName2);

    if(!file1Exists || !file2Exists)
    {
        if(!file1Exists && !file2Exists)
        {
            raiseFileNotFoundError(fileName1 + ", " + fileName2);
        }
        else if(!file1Exists)
        {
            raiseFileNotFoundError(fileName1);
        }
        else if(!file2Exists)
        {
            raiseFileNotFoundError(fileName2);
        }
        return;
    }

    Cross cross(fileName1, fileName2, outputFileName);

    std::clock_t start;
    start = std::clock();

    cross.readRows();
    cross.printTable();

    showProcessTime(getProcessDuration(start));

    cross.saveOutput();
}

void processSelectQuery()
{
    string fileName, outputFileName;
    string parameter1, parameter2;

    cin >> fileName >> outputFileName;

    queryProcessingTimerMsg();

    bool fileExists = checkFileExistence(fileName);

    if(!fileExists)
    {
        raiseFileNotFoundError(fileName);
        return;
    }

    Select select(fileName, outputFileName);

    cin >> parameter1 >> parameter2;

    bool isNumber = true;
    int searchValue = 0;
    for(int i = 0; i < parameter2.size(); i++)
    {
        if(!('0' <= parameter2[i] && parameter2[i] <= '9'))
        {
            isNumber = false;
            break;
        }
        searchValue = (searchValue * 10) + (parameter2[i] - '0');
    }

    vector<string>headers;
    headers.pb(parameter1);
    if(!isNumber)
    {
        if(parameter1 == parameter2)
        {
            cout<<"ERROR: 'param1: "<< parameter1 <<"' and param2: " <<parameter2<<"' cannot be equal."<<endl;
            return;
        }
        headers.pb(parameter2);
    }

    if(!checkParameters(fileName, headers))
    {
        raiseInvalidParameterError();
        return;
    }

    select.addHeader(parameter1, isNumber, parameter2, searchValue);

    std::clock_t start;
    start = std::clock();

    select.readRows();
    select.printTable();

    showProcessTime(getProcessDuration(start));


    select.saveOutput();
}

void processJoinQuery()
{
    string fileName1, fileName2, outputFileName;

    cin >> fileName1 >> fileName2 >> outputFileName;

    queryProcessingTimerMsg();

    bool file1Exists = checkFileExistence(fileName1);
    bool file2Exists = checkFileExistence(fileName2);

    if(!file1Exists || !file2Exists)
    {
        if(!file1Exists && !file2Exists)
        {
            raiseFileNotFoundError(fileName1 + ", " + fileName2);
        }
        else if(!file1Exists)
        {
            raiseFileNotFoundError(fileName1);
        }
        else if(!file2Exists)
        {
            raiseFileNotFoundError(fileName2);
        }
        return;
    }

    Join join(fileName1, fileName2, outputFileName);
    std::clock_t start;
    start = std::clock();

    join.readRows();

    showProcessTime(getProcessDuration(start));
}

void processBTreeQuery()
{
    string fileName;

    cin >> fileName;

    bool fileExists = checkFileExistence(fileName);

    if(!fileExists)
    {
        raiseFileNotFoundError(fileName);
        return;
    }

    size_t pos = fileName.find(".btree");

    if((pos == std::string::npos))
    {
        raiseBtreeFileNotFoundError(fileName);
        return;
    }

    BPlusTree bPlusTree;
    bPlusTree.setMetaDataFileName(fileName);
    bPlusTree.printBPlusTree();
}

void readAndPrintFile (string path)
{
    std::ifstream file(path);
    std::string str;
    while (std::getline(file, str))
    {
        cout<<str<<endl;
    }
    cout<<endl;
    file.close();
}

void printConsole()
{
    readAndPrintFile("resources/head.txt");

}

void loadHelpFile()
{
    readAndPrintFile("resources/helpfile.txt");
}

void showOperatorManual()
{
    string operatorName;
    cin >> operatorName;

    if (operatorName == "select")
    {
        readAndPrintFile("resources/man-select.txt");
    }
    else if (operatorName == "project")
    {
        readAndPrintFile("resources/man-project.txt");
    }
    else if (operatorName == "cross")
    {
        readAndPrintFile("resources/man-cross.txt");
    }
    else if (operatorName == "btree") {
        readAndPrintFile("resources/man-btree.txt");
    }
    else if (operatorName == "join") {
        readAndPrintFile("resources/man-join.txt");
    }
    else
    {
        cout<<"ERROR: '"<< operatorName <<"' not found."<<endl;
    }
}

int main()
{
    string query;

    printConsole();

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
        else if(query == "select")
        {
            processSelectQuery();
        }
        else if(query == "join")
        {
            processJoinQuery();
        }
        else if(query == "btree")
        {
            processBTreeQuery();
        }
        else if(query == "clear")
        {
            system("CLS");
            printConsole();
        }
        else if (query == "exit")
        {
            exit(0);
        }
        else if (query == "-h")
        {
            loadHelpFile();
        }
        else if (query == "man")
        {
            showOperatorManual();
        }
        else
        {
            cout<<"ERROR: '"<< query <<"' is not defined."<<endl;
            cout<<"Enter -h to check the query syntax."<<endl<<endl;
            cin.sync();
        }
    }

    return 0;
}
