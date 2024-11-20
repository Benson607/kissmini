#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Box {
public:
    int x;
    vector<vector<int>> status;
    Box() {
        x = 0;
    }
    Box(int x) {
        this->x = x;
    }
    Box(vector<string> status_l, vector<string> status_r) {
        x = 0;
        for (int i = 0; i < status_l.size(); i++) {
            vector<int> one_pair(2);
            one_pair[0] = atoi(status_l[i].c_str());
            one_pair[1] = atoi(status_r[i].c_str());
            status.push_back(one_pair);
        }
    }
    vector<int>& operator[](int i) {
        return status[i];
    }
};

class Kiss {
public:
    int start;
    int input_digit;
    int output_size;
    int node_size;
    int status_size;
    char start_status;
    vector<vector<Box>> table;
    vector<vector<string>> node_list;
    vector<string> status_list;
    vector<vector<string>> form;
    vector<string> input_list;
    Kiss() {
        start = 0;
        input_digit = 0;
        output_size = 0;
        node_size = 0;
        status_size = 0;
        start_status = 0;
    }
    void add_status(string status) {
        for (int i = 0; i < status_list.size(); i++) {
            if (status_list[i] == status) {
                return;
            }
        }
        status_list.push_back(status);
    }
    void add_input(string input) {
        for (string i: input_list) {
            if (i == input) {
                return;
            }
        }
        input_list.push_back(input);
    }
    string find_index_of_status(string status) {
        for (int i = 0; i < status_list.size(); i++) {
            if (status_list[i] == status) {
                return to_string(i);
            }
        }
        return "";
    }
    void transfer_form() {
        form = vector(status_list.size(), vector<string>(input_list.size() * 2 + 1, ""));
        for (int i = 0; i < status_list.size(); i++) {
            form[i][0] = status_list[i];
            for (int j = 0; j < node_list.size(); j++) {
                if (node_list[j][1] == form[i][0]) {
                    for (int k = 0; k < input_list.size(); k++) {
                        if (input_list[k] == node_list[j][0]) {
                            form[i][k * 2 + 1] = find_index_of_status(node_list[j][2]);
                            form[i][k * 2 + 2] = node_list[j][3];
                            break;
                        }
                    }
                }
            }
        }
    }
    void transfer_table() {
        table = vector(form.size(), vector<Box>());
        for (int i = 1; i < form.size(); i++) {
            table[i] = vector<Box>(i);
            for (int j = 0; j < i; j++) {
                int isx = 0;
                vector<string> status_l;
                vector<string> status_r;
                for (int k = 1; k < form[0].size(); k += 2) {
                    if (form[i][k + 1] != form[j][k + 1]) {
                        isx = 1;
                        break;
                    }
                    status_l.push_back(form[i][k]);
                    status_r.push_back(form[j][k]);
                }
                if (isx) {
                    table[i][j] = Box(1);
                }
                else {
                    table[i][j] = Box(status_l, status_r);
                }
            }
        }
    }
    void replace(int old, int next) {}
    void easy() {
        int keep = 1;
        while (keep) {
            keep = 0;
            for (int i = 0; i < table.size(); i++) {
                for (int j = 0; j < table[i].size(); j++) {
                    for (int k = 0; k < table[i][j].status.size(); k++) {
                        if (table[i][j][k][0] > table[i][j][k][1]) {
                            if (table[table[i][j][k][0]][table[i][j][k][1]].x) {
                                table[i][j] = Box(1);
                                keep = 1;
                                break;
                            }
                        }
                        else if (table[i][j][k][0] < table[i][j][k][1]) {
                            if (table[table[i][j][k][1]][table[i][j][k][0]].x) {
                                table[i][j] = Box(1);
                                keep = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
        for (int i = 0; i < table.size(); i++) {
            for (int j = 0; j < table[i].size(); j++) {
                if (!table[i][j].x) {
                    replace(i, j);
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    ifstream input;
	input.open(argv[1]);
    Kiss kiss;
    string line;
    while (getline(input, line)) {
        istringstream linestream(line);
        string order;
        linestream >> order;
        if (order == ".start_kiss") {
            kiss.start = 1;
        }
        else if (order == ".end_kiss") {
            break;
        }
        else if (order == ".i") {
            int num;
            linestream >> num;
            kiss.input_digit = num;
        }
        else if (order == ".o") {
            int num;
            linestream >> num;
            kiss.output_size = num;
        }
        else if (order == ".p") {
            int num;
            linestream >> num;
            kiss.node_size = num;
        }
        else if (order == ".s") {
            int num;
            linestream >> num;
            kiss.status_size = num;
        }
        else if (order == ".r") {
            char start_status;
            linestream >> start_status;
            kiss.start_status = start_status;
        }
        else {
            vector<string> node(4, "");
            node[0] = order;
            kiss.add_input(order);
            for (int j = 1; j < 4; j++) {
                linestream >> node[j];
            }
            kiss.add_status(node[1]);
            kiss.node_list.push_back(node);
        }
    }

    for (int i = 0; i < kiss.node_size; i++) {
        for (int j = 0; j < 4; j++) {
            cout << kiss.node_list[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (string i: kiss.status_list) {
        cout << i << " ";
    }
    cout << endl;

    cout << endl;

    kiss.transfer_form();

    for (int i = 0; i < kiss.form.size(); i++) {
        for (string j: kiss.form[i]) {
            cout << j << " ";
        }
        cout << endl;
    }

    cout << endl;

    kiss.transfer_table();

    for (int i = 0; i < kiss.table.size(); i++) {
        for (int j = 0; j < kiss.table[i].size(); j++) {
            if (kiss.table[i][j].x) {
                cout << "x" << "\t";
            }
            else {
                for (int k = 0; k < kiss.table[i][j].status.size(); k++) {
                    cout << kiss.table[i][j].status[k][0] << " " << kiss.table[i][j].status[k][1] << " ";
                }
            }
        }
        cout << "\t" << endl;
    }

    cout << endl;

    kiss.easy();

    for (int i = 0; i < kiss.table.size(); i++) {
        for (int j = 0; j < kiss.table[i].size(); j++) {
            if (kiss.table[i][j].x) {
                cout << "x" << "\t";
            }
            else {
                for (int k = 0; k < kiss.table[i][j].status.size(); k++) {
                    cout << kiss.table[i][j].status[k][0] << " " << kiss.table[i][j].status[k][1] << " ";
                }
            }
        }
        cout << "\t" << endl;
    }
    
    return 0;
}