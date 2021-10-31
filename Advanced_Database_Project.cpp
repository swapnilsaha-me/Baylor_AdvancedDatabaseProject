#include<bits/stdc++.h>
using namespace std;

#define sf              scanf
#define pf              printf
#define pb              push_back
#define MAX_POINTER     4

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

void processSelectQuery()
{
    string fileName, outputFileName;
    string parameter1, parameter2;

    cin >> fileName >> outputFileName;
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

    select.addHeader(parameter1, isNumber, parameter2, searchValue);
    select.readRows();
    select.printTable();
    select.saveOutput();
}

int main()
{
    string query;

    while(cout << "Query >> ", cin >>
            query)
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
        else if(query == "clear")
        {
            system("CLS");
        }
    }

    return 0;
}

