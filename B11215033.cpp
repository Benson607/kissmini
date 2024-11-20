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
    string start_status;
    vector<vector<Box>> table;
    vector<vector<string>> node_list;
    vector<string> status_list;
    vector<vector<string>> form;
    vector<string> input_list;
    vector<int> del_list;
    vector<vector<string>> output_kiss_node_list;
    Kiss() {
        start = 0;
        input_digit = 0;
        output_size = 0;
        node_size = 0;
        status_size = 0;
        start_status = "";
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
    int add_del(int del_int) {
        for (int i: del_list) {
            if (i == del_int) {
                return 1;
            }
        }
        del_list.push_back(del_int);
        return 0;
    }
    void replace(int old, int next) {
        if (add_del(old)) {
            return;
        }
        string next_str = to_string(next);
        string old_str = to_string(old);
        for (int i = 0; i < form.size(); i++) {
            for (int j = 1; j < form[0].size(); j += 2) {
                if (form[i][j] == old_str) {
                    form[i][j] = next_str;
                }
            }
        }
    }
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
    void del() {
        for (int i = 0; i < del_list.size(); i++) {
            int min = i;
            for (int j = i + 1; j < del_list.size(); j++) {
                if (del_list[min] > del_list[j]) {
                    min = j;
                }
            }
            int tmp = del_list[i];
            del_list[i] = del_list[min];
            del_list[min] = tmp;
        }
        for (int i = 0; i < del_list.size(); i++) {
            form.erase(form.begin() + del_list[i] - i);
        }
    }
    void write_kiss(string path) {
        ofstream output;
        output.open(path);
        
        output << ".start_kiss" << endl;
        output << ".i " << input_digit << endl;
        output << ".o " << output_size << endl;
        output << ".p " << input_list.size() * form.size() << endl;
        output << ".s " << form.size() << endl;
        output << ".r " << start_status << endl;

        for (int i = 0; i < form.size(); i++) {
            for (int j = 0; j < input_list.size(); j++) {
                vector<string> one_node;
                one_node.push_back(input_list[j]);
                one_node.push_back(form[i][0]);
                one_node.push_back(status_list[atoi(form[i][j * 2 + 1].c_str())]);
                one_node.push_back(form[i][j * 2 + 2]);
                output_kiss_node_list.push_back(one_node);
                output << one_node[0] << " " << one_node[1] << " " << one_node[2] << " " << one_node[3] << endl;
            }
        }

        output << ".end_kiss" << endl;
        
        output.close();
    }
    void write_dot(string path) {
        ofstream output;
        output.open(path);

        output << "digraph STG {" << endl;
        output << "    rankdir=LR;" << endl << endl;
        output << "    INIT [shape=point]" << endl;

        for (int i = 0; i < form.size(); i++) {
            output << "    " << form[i][0] << " [label=\"" << form[i][0] << "\"]" << endl;
        }
        output << endl;

        output << "    INIT -> " << start_status << ";" << endl;

        for (int i = 0; i < output_kiss_node_list.size(); i++) {
            vector<string> labels;
            labels.push_back(output_kiss_node_list[i][0]);
            labels[labels.size() - 1].append("/");
            labels[labels.size() - 1].append(output_kiss_node_list[i][3]);
            vector<int> output_del_list;
            for (int j = i + 1; j < output_kiss_node_list.size(); j++) {
                if (output_kiss_node_list[i][1] == output_kiss_node_list[j][1] && output_kiss_node_list[i][2] == output_kiss_node_list[j][2]) {
                    labels.push_back(output_kiss_node_list[j][0]);
                    labels[labels.size() - 1].append("/");
                    labels[labels.size() - 1].append(output_kiss_node_list[j][3]);
                    output_del_list.push_back(j);
                }
            }
            for (int j = 0; j < output_del_list.size(); j++) {
                output_kiss_node_list.erase(output_kiss_node_list.begin() + output_del_list[j] - j);
            }
            string labels_str = "";
            for (int j = 0; j < labels.size(); j++) {
                labels_str.append(labels[j]);
                if (j != labels.size() - 1) {
                    labels_str.append(",");
                }
            }
            output << "    " << output_kiss_node_list[i][1] << " -> " << output_kiss_node_list[i][2] << " [label=\"" << labels_str << "\"];" << endl;
        }

        output << "}" << endl;

        output.close();
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
            string start_status;
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

    input.close();

    kiss.transfer_form();
    kiss.transfer_table();
    kiss.easy();
    kiss.del();

    for (int i = 0; i < kiss.form.size(); i++) {
        for (int j = 0; j < kiss.form[0].size(); j++) {
            cout << kiss.form[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    kiss.write_kiss(argv[2]);
    kiss.write_dot(argv[3]);
    
    return 0;
}