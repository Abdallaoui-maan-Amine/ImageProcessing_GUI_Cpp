#include "Descriptor.h"
#include<math.h>
#include<iostream>
#include <vector>
#include <fstream>
#include <algorithm>


#include <string>

using namespace std;


vector<string> split_text(string text, string delimeter) {
    vector<string> splited_text;
    int pos = 0;
    while (pos != -1) {
        pos = text.find(delimeter);
        splited_text.push_back(text.substr(0, pos));
        text = text.substr(pos + delimeter.length());
    }
    return splited_text;
}






int get_csv_rows(string path) {
    fstream file;
    string text = "haha";
    file.open(path, ios::in);
    int row = 0;
    while (text != "") {
        getline(file, text);
        //cout << "The line contains : " << text;
        row++;
    }
    return row - 1;

}





int get_csv_columns(string path) {
    fstream file;
    file.open(path, ios::in);

    string text;
    getline(file, text);
    int count = 0;


    for (int i = 0; i < text.size(); i++)
        if (text[i] == ',') count++;

    return count+1;
}



string dataBase_path = "";
string filtred_DataBase = "";









Descriptor::Descriptor(string path)
{
    path_ = path;

}




int Descriptor::get_Rows_Size()
{
    return get_csv_rows(path_);

}


int Descriptor::get_Columns_Size()
{
    return get_csv_columns(path_);

}






string Descriptor::Get_Cell_from_CSV(int columns, int row_idx, int col_idx)
{

    fstream file;
    string text = "haha";
    file.open(path_, ios::in);

    for (int i = 0; i < row_idx+1; i++) {
        getline(file, text);
    }

    vector< string> descriptor(columns, "");
    descriptor = split_text(text, ", ");

    return  descriptor[col_idx];

}



bool sortcol_byNumber(const vector<string>& v1, const vector<string>& v2) {
    return stoi(v1[4]) < stoi(v2[4]);
}

bool sortcol_byText(const vector<string>& v1, const vector<string>& v2) {
    return v1[2] < v2[2];
}




void Descriptor::sort_Data(int col_idx) {

    fstream file;
    file.open(path_, ios::in);

    string text;

    int rows = get_csv_rows(path_);
    int columns = get_csv_columns(path_);

    vector< vector<string> > data(rows, vector<string>(columns, ""));

    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text(text, ", ");
        data[i] = descriptor;
    }


    if (col_idx == 2) {

        sort(data.begin(), data.end(), sortcol_byText);
        cout << "\n\n\n Column 2 has been sorted succefully .. ! \n\n";
    }
    else if (col_idx == 4) {
        sort(data.begin(), data.end(), sortcol_byNumber);
        cout << "\n\n\n Column 4 has been sorted succefully .. ! \n\n";
    }


    file.close();

    // Save data
    file.open(path_, ios::out);    //"D:/C++_Folder/Files/out.csv"

    cout << "\n\n my data : \n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == columns - 1) {
                file << data[i][j];
                cout << data[i][j];
            }
            else {
                file << data[i][j] << ", ";
                cout << data[i][j] << "/ ";
            }

        }
        file << "\n";
        cout << "\n";
    }
    cout << "\n\n\n";

    cout << "\n\n\n      [+] Data has been sorted succefully..!\n";

}







void Descriptor::filter_Data(int col_idx, string filter_token){
    fstream file;
    file.open(path_, ios::in);

    string text;

    int rows = get_csv_rows(path_);
    int columns = get_csv_columns(path_);

    vector< vector<string> > data(rows, vector<string>(columns, ""));

    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text(text, ", ");
        if (col_idx == 5){
            if (descriptor[col_idx] == filter_token){
                data[i] = descriptor;
            } else if (descriptor[col_idx] == filter_token){
                data[i] = descriptor;
            }
        }
    }



    file.close();

    // Save data
    file.open(filtred_DataBase, ios::out);    //"D:/C++_Folder/Files/out.csv"

    cout << "\n\n my data : \n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == columns - 1) {
                file << data[i][j];
                cout << data[i][j];
            }
            else {
                file << data[i][j] << ", ";
                cout << data[i][j] << "/ ";
            }

        }
        file << "\n";
        cout << "\n";
    }

}


void Descriptor::add_Data(string path, int id, string name, string source, int price, string access) {
    fstream file;
    file.open(path_, ios::in);

    string text;


    int rows = get_csv_rows(path_);
    int columns = get_csv_columns(path_);

    int new_rows = rows + 1;

    vector< vector<string> > data(new_rows, vector<string>(columns, ""));


    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text(text, ", ");
        data[i] = descriptor;
    }

    vector< string> new_data = { path, to_string(id), name, source, to_string(price), access };
    data[new_rows - 1] = new_data;


    file.close();

    // Save data
    file.open(path_, ios::out);    //"D:/C++_Folder/Files/out.csv"

    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == columns - 1) {
                file << data[i][j];
            }
            else {
                file << data[i][j] << ", ";
            }

        }
        file << "\n";
    }

    cout << "\n\n\n      [+] Data has been added succefully..!\n";

}




void Descriptor::delete_Data(int row_idx)
{
    fstream file;
    file.open(path_, ios::in);

    string text;


    int rows = get_csv_rows(path_);
    int columns = get_csv_columns(path_);

    vector< vector<string> > data(rows-1, vector<string>(columns, ""));
    int idx=0;

    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text(text, ", ");

        if (i != row_idx) {
            data[idx] = descriptor;
            idx++;
        }

    }




    file.close();

    // Save data
    file.open(path_, ios::out);    //"D:/C++_Folder/Files/out.csv"

    for (int i = 0; i < rows - 1; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == columns - 1) {
                file << data[i][j];
            }
            else {
                file << data[i][j] << ", ";
            }

        }
        file << "\n";
    }

    cout << "\n\n\n      [+] Data has been deleted succefully..!\n";

}



void Descriptor::update_Data(int row_idx, int col_idx, string new_value)
{

    fstream file;
    file.open(path_, ios::in);

    string text;


    int rows = get_csv_rows(path_);
    int columns = get_csv_columns(path_);

    vector< vector<string> > data(rows, vector<string>(columns ,""));

    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text(text, ", ");
        //descriptor = split_text(text);

        data[i] = descriptor;

        if (i == row_idx) {
            data[i][col_idx] = new_value;
        }

    }

    file.close();

    // Save data
    file.open(path_, ios::out);    //"D:/C++_Folder/Files/out.csv"

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == columns - 1) {
                file << data[i][j];
            }
            else {
                file << data[i][j] << ", ";
            }

        }
        file << "\n";
    }

    cout << "\n\n\n      [+] Data has been saved succefully..!\n";

}



