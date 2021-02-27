#include<iostream>
#include<iomanip>
#include<vector>

#include "method.h"

using namespace std;

class MackMethod : public MethodBase
{
private:
    int n;
    vector<vector<int>> matrix, marks, initial;
    vector<bool> isColumnMarked;
    vector<int> minElements;

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

    void PrintMatrix() {
        cout << "Matrix:\n";
        for(int i = 0;i < n;++i) {
            for(int j = 0;j < n;++j) {
                PrintElement(matrix[i][j]);
                if(!marks.empty()) {
                    if(marks[i][j] == 1)
                        cout << "*";
                    else if(marks[i][j] == -1)
                        cout << "^";
                    else
                        cout << " ";
                }
                cout << " ";
            }
            cout << "\n";
        }
        if(!isColumnMarked.empty()) {
            for(int i = 0;i < n;++i)
                cout << "_____";
            cout << "\n";
            for(int i = 0;i < n;++i)
                if(isColumnMarked[i])
                    cout << "  #  ";
            cout << "\n";
        }
    }
    void FindMin() {
        cout << "Step 1,2\n";
        cout << "Select minimal element in each row\n";

        minElements.resize(n);
        marks.assign(n, vector<int>(n));
        for(int i = 0;i < n;++i) {
            int minElement = matrix[i][0];
            for(int j = 1;j < n;++j) {
                if(matrix[i][j] < minElement)
                    minElement = matrix[i][j];
            }

            for(int j = 0;j < n;++j) {
                if(matrix[i][j] == minElement) {
                    marks[i][j] = 1;
                    break;
                }
            }

            minElements[i] = minElement;
        }
        PrintMatrix();
    }

    bool ifOnlyInColum(int col, int cur = -1) {
        for(int i = 0;i < n;++i) {
            if(marks[i][col] && i != cur)
                return false;
        }
        return true;
    }
    void CheckMarks() {
        cout << "Step 3,4,5\n";
        cout << "Look through marks in columns\n";

        for(int i = 0;i < n;++i) {
            bool hasChanges;
            do {
                hasChanges = false;
                for(int j = 0;j < n;++j) {
                    if(matrix[i][j] == minElements[i]) {
                        if(marks[i][j]) {
                            if(ifOnlyInColum(j, i))
                                break;
                        }
                        else {
                            if(ifOnlyInColum(j, i)) {
                                for(int k = 0;k < n;++k)
                                    marks[i][k] = 0;
                                marks[i][j] = 1;
                            }
                            else
                                marks[i][j] = -1;

                            hasChanges = true;
                            break;
                        }
                    }
                }
            } while(hasChanges);
        }
        PrintMatrix();
    }

    bool CheckResult() {
        cout << "Step 6\n";
        cout << "Check if answer found\n";

        vector<bool> found(n, false);
        for(int i = 0;i < n;++i)
        for(int j = 0;j < n;++j)
            if(marks[i][j] == 1) {
                if(found[j])
                    return false;

                found[j] = true;
                break;
            }

        return true;
    }

    void MarkColumns() {
        cout << "Step 7\n";
        cout << "Mark columns with several marked elements\n";

        isColumnMarked.assign(n, false);
        vector<bool> found(n, false);
        for(int i = 0;i < n;++i)
        for(int j = 0;j < n;++j)
            if(marks[i][j]) {
                if(found[j])
                    isColumnMarked[j] = true;

                found[j] = true;
            }
        PrintMatrix();
    }

    void FindRows() {
        cout << "Step 8,9,10,11,12\n";
        cout << "Find rows with marked \'*\' elements in marked columns\nAdd minimal differences between elements\n";

        vector<int> difference(n, 0);
        int minDif = -1;

        for(int i = 0;i < n;++i)
        for(int j = 0;j < n;++j) {
            if(!isColumnMarked[j])
                continue;

            if(marks[i][j] == 1) {
                for(int k = 0;k < n;++k) {
                    if(!isColumnMarked[k]) {
                        if(minDif < 0)
                            minDif = matrix[i][k] - matrix[i][j];
                        else
                            minDif = min(minDif, matrix[i][k] - matrix[i][j]);
                    }
                }
            }
        }

        if(minDif > 0)
            for(int i = 0;i < n;++i)
            for(int j = 0;j < n;++j)
                if(isColumnMarked[j]) {
                    matrix[i][j] += minDif;
                }

        PrintMatrix();
        isColumnMarked.clear();
    }

    void PrintElement(int x) { cout << setw(3) << setfill(' ') << x; }
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
        cout << "Mack method\n";
        ReadMatrix();
        while(true) {
            FindMin();
            CheckMarks();
            if(CheckResult())
                break;

            MarkColumns();
            FindRows();
        }

        PrintResult();
    }
};
