#include<math.h>
#include<iostream>

#include <fstream>
#include <algorithm>


#include <string>



using namespace std;

vector<string> split_text_(string text) {

    vector<string> descriptor;

    string delimiter = ",";
    size_t pos = 0;
    string token;
    while ((pos = text.find(delimiter)) != string::npos) {
        token = text.substr(0, pos);
        descriptor.push_back(token);
        ///cout << "\n   > : " << token;

        text.erase(0, pos + delimiter.length());
    }

    return descriptor;
}

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
