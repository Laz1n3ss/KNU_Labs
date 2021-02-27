#include<iostream>
#include<iomanip>
#include<vector>

#include "method.h"

using namespace std;

class HungaryMethod: public MethodBase {
private:
    int n;
    vector<vector<int>> matrix, marks, initial;
    vector<bool> isRowMarked, isColumnMarked;

    void ReadMatrix() {
        cout << "Enter size(n)\n";
        cin >> n;

        cout << "Enter matrix\n";
        cout << "\tElement should be separated by spaces\n";

        matrix.assign(n, vector<int>(n));
        initial.assign(n, vector<int>(n));

        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                cin >> matrix[i][j];
                initial[i][j] = matrix[i][j]; // To find answer at the end
            }
        }
    }

    void PrintElement(int x) { cout << setw(3) << setfill(' ') << x; }
    void SubstractFromRow() {
        cout << "Step 1\n";
        cout << "\tSubtract minimal element from each row\n";

        vector<int> minElements(n);
        for(int i = 0;i < n;++i) {
            int minElement = matrix[i][0];
            for(int j = 0;j < n;++j)
                minElement = min(minElement, matrix[i][j]);

            for(int j = 0;j < n;++j)
                matrix[i][j] -= minElement;

            minElements[i] = minElement;
        }

        cout << "Result matrix\n";
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                PrintElement(matrix[i][j]);
                cout << ' ';
            }
            cout << " | ";
            PrintElement(-minElements[i]);
            cout << "\n";
        }
        cout << "\t---\n";
    }
    void SubstractFromColumn() {
        cout << "Step 2\n";
        cout << "\tSubtract minimal element from each column\n";

        vector<int> minElements(n);
        for(int j = 0;j < n;++j) {
            int minElement = matrix[0][j];
            for(int i = 0;i < n;++i)
                    minElement = min(minElement, matrix[i][j]);

            for(int i = 0;i < n;++i)
                matrix[i][j] -= minElement;

            minElements[j] = minElement;
        }

        cout << "Result matrix\n";
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                PrintElement(matrix[i][j]);
                cout << ' ';
            }
            cout << '\n';
        }
        for(int i = 0;i < n;++i) {
            cout << "____";
        }
        cout << '\n';
        for(int i = 0;i < n;++i) {
            PrintElement(-minElements[i]);
            cout << " ";
        }
        cout << "\n";
        cout << "\t---\n";
    }
    void PrintMark(int m) {
        if(m == 1)
            cout << "*";
        else if(m == -1)
            cout << "^";
        else
            cout << " ";
    }
    bool MarkRowsElements() {
        cout << "Step 3\n";
        cout << "\tMark zeros in each row\n";

        bool hasChanges = false;

        vector<int> minElements(n);
        for(int i = 0;i < n;++i) {
            int zeroIndex = -1;

            for(int j = 0;j < n;++j) {
                if(matrix[i][j] == 0) {
                    if(marks[i][j] == 1) {
                        zeroIndex = -1;
                        break;
                    }
                    else if(marks[i][j] == -1) {
                        continue;
                    }

                    if(zeroIndex >= 0) {
                        zeroIndex = -1;
                        break;
                    }
                    else {
                        zeroIndex = j;
                    }

                }
            }

            if(zeroIndex >= 0) {
                for(int i2 = 0;i2 < n;++i2) {
                    if(matrix[i2][zeroIndex] == 0)
                        marks[i2][zeroIndex] = -1;
                }
                marks[i][zeroIndex] = 1;
                hasChanges = true;
            }
        }

        if(hasChanges) {
            cout << "Result matrix\n";
            for(int i = 0;i < n;++i) {
                for(int j = 0;j < n;++j) {
                    PrintElement(matrix[i][j]);
                    PrintMark(marks[i][j]);
                    cout << ' ';
                }
                cout << '\n';
            }
        }
        else
            cout << "No changes\n";
        cout << "\t---\n";

        return hasChanges;
    }
    bool MarkColumnsElements() {
        cout << "Step 4\n";
        cout << "\tMark zeros in each column\n";

        bool hasChanges = false;

        vector<int> minElements(n);
        for(int j = 0;j < n;++j) {
            int zeroIndex = -1;

            for(int i = 0;i < n;++i) {
                if(matrix[i][j] == 0) {
                    if(marks[i][j] == 1) {
                        zeroIndex = -1;
                        break;
                    }
                    else if(marks[i][j] == -1) {
                        continue;
                    }

                    if(zeroIndex >= 0) {
                        zeroIndex = -1;
                        break;
                    }
                    else {
                        zeroIndex = i;
                    }

                }
            }

            if(zeroIndex >= 0) {
                for(int j2 = 0;j2 < n;++j2) {
                    if(matrix[zeroIndex][j2] == 0)
                        marks[zeroIndex][j2] = -1;
                }
                marks[zeroIndex][j] = 1;

                hasChanges = true;
            }
        }

        if(hasChanges) {
            cout << "Result matrix\n";
            for(int i = 0;i < n;++i) {
                for(int j = 0;j < n;++j) {
                    PrintElement(matrix[i][j]);
                    PrintMark(marks[i][j]);
                    cout << ' ';
                }
                cout << '\n';
            }
        }
        else
            cout << "No changes\n";
        cout << "\t---\n";

        return hasChanges;
    }
    void MarkElements() {
        marks.assign(n, vector<int>(n, 0));

        bool rowChanges = MarkRowsElements(),
            columnChanges = MarkColumnsElements();

        cout << "Step 5\n";
        cout << "\tRepeat steps 3,4 until needed\n";

        while(columnChanges) {
            rowChanges = MarkRowsElements(),
            columnChanges = MarkColumnsElements();
        };
    }
    bool CheckLine() {
        cout << "Step 6\n";
        cout << "\tCheck matrix state\n";

        int markedCnt = 0;
        for(int i = 0;i < n;++i)
        for(int j = 0;j < n;++j) {
            if(marks[i][j] == 1)
                markedCnt++;
            else if(matrix[i][j] == 0 && marks[i][j] == 0) {
                marks[i][j] = 1;

                MarkLines();
                return CheckLine();
            }
        }
        return markedCnt == n;
    }
    void MarkRows() {
        cout << "Step 7\n";
        cout << "\tMark rows that no marked \'*\' zero\n";

        isRowMarked.assign(n, 0);
        isColumnMarked.assign(n, 0);

        for(int i = 0;i < n;++i) {
            int hasMarkedZero = false;
            for(int j = 0;j < n && !hasMarkedZero;++j) {
                if(marks[i][j] == 1)
                    hasMarkedZero = true;
            }
            isRowMarked[i] = !hasMarkedZero;
        }

        cout << "Result matrix\n";
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                PrintElement(matrix[i][j]);
                PrintMark(marks[i][j]);
                cout << ' ';
            }
            cout << " | ";
            if(isRowMarked[i])
                cout << "#";
            else
                cout << " ";
            cout << "\n";
        }
    }
    bool MarkColumns() {
        cout << "Step 8\n";
        cout << "\tMark columns that has marked \'^\' zero in marked rows\n";

        bool hasChanges = false;
        for(int j = 0;j < n;++j) {
            if(isColumnMarked[j])
                continue;

            int hasMarkedZero = false;
            for(int i = 0;i < n && !hasMarkedZero;++i) {
                if(isRowMarked[i] && marks[i][j] == -1)
                    hasMarkedZero = true;
            }
            if(hasMarkedZero)
                hasChanges = true;
            isColumnMarked[j] = hasMarkedZero;
        }

        if(hasChanges) {
            cout << "Result matrix\n";
            for(int i = 0;i < n;++i) {
                for(int j = 0;j < n;++j) {
                    PrintElement(matrix[i][j]);
                    PrintMark(marks[i][j]);
                    cout << ' ';
                }
                cout << " | ";
                if(isRowMarked[i])
                    cout << "#";
                else
                    cout << " ";
                cout << "\n";
            }
            for(int j = 0;j < n;++j)
                cout << "_____";
            cout << '\n';

            for(int j = 0;j < n;++j) {
                if(isColumnMarked[j])
                    cout << "  # ";
                cout << " ";
            }
            cout << '\n';
        }
        else
            cout << "No changes\n";

        return hasChanges;
    }
    bool ReMarkRows() {
        cout << "Step 9\n";
        cout << "\tMark rows that has marked \'*\' zero in marked columns\n";

        bool hasChanges = false;
        for(int i = 0;i < n;++i) {
            if(isRowMarked[i])
                continue;

            int hasMarkedZero = false;
            for(int j = 0;j < n && !hasMarkedZero;++j) {
                if(isColumnMarked[j] && marks[i][j] == 1)
                    hasMarkedZero = true;
            }
            if(hasMarkedZero)
                hasChanges = true;
            isRowMarked[i] = hasMarkedZero;
        }

        if(hasChanges) {
            cout << "Result matrix\n";
            for(int i = 0;i < n;++i) {

                for(int j = 0;j < n;++j) {
                    PrintElement(matrix[i][j]);
                    PrintMark(marks[i][j]);
                    cout << ' ';
                }
                cout << " | ";
                if(isRowMarked[i])
                    cout << "#";
                else
                    cout << " ";
                cout << "\n";
            }
            for(int j = 0;j < n;++j)
                cout << "_____";
            cout << '\n';

            for(int j = 0;j < n;++j) {
                if(isColumnMarked[j])
                    cout << "  # ";
                cout << " ";
            }
            cout << "\n";
        }
        else
            cout << "No changes\n";

        return hasChanges;
    }
    void MarkLines() {

        MarkRows();

        bool columnChanges = MarkColumns(),
            rowChanges = ReMarkRows();

        if(rowChanges) {
            cout << "Step 10\n";
            cout << "\tRepeat steps 8,9 until needed\n";
             while(rowChanges) {
                columnChanges = MarkColumns();
                rowChanges = ReMarkRows();
            }
        }
        else {
            cout << "Step 10 is not needed\n";
        }
    }
    void ReEvaluate() {
        int minElement = -1;
        for(int i = 0;i < n;++i) {
            if(isRowMarked[i])
            for(int j = 0;j < n;++j) {
                if(!isColumnMarked[j]) {
                    if(minElement == -1)
                        minElement = matrix[i][j];
                    else
                        minElement = min(minElement, matrix[i][j]);
                }
            }
        }
        for(int i = 0;i < n;++i) {
            if(isRowMarked[i])
                for(int j = 0;j < n;++j)
                    matrix[i][j] -= minElement;
        }
        for(int j = 0;j < n;++j) {
            if(isColumnMarked[j])
            for(int i = 0;i < n;++i)
                matrix[i][j] += minElement;
        }


        cout << "Minimal element " << minElement << '\n';
        cout << "Result matrix\n";
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                PrintElement(matrix[i][j]);
                cout << ' ';
            }
            cout << " | ";
            if(isRowMarked[i])
                PrintElement(-minElement);
            cout << '\n';
        }
        for(int i = 0;i < n;++i) {
            cout << "____";
        }
        cout << '\n';
        for(int i = 0;i < n;++i) {
            if(isColumnMarked[i])
                PrintElement(minElement);
            else
                cout << "   ";
            cout << " ";
        }
        cout << "\n";
        cout << "\t---\n";
    }

    void PrintResult() {
        cout << "Answer Found!!!\n";

        int total = 0;
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                if(marks[i][j] == 1) {
                    cout << "1";
                    total += initial[i][j];
                }
                else
                    cout << "0";
                cout << " ";
            }
            cout << "\n";
        }

        cout << "Answer " << total << '\n';
    }

public:
    void Start() {
        cout << "Hungary method\n";
        ReadMatrix();
        SubstractFromRow();
        SubstractFromColumn();
        while(true) {
            MarkElements();
            if(CheckLine())
                break;

            MarkLines();
            ReEvaluate();
        }

        PrintResult();
    }
};
