#pragma once

#include<iostream>
#include <string>



using namespace std;




class Descriptor
{
    string path_;

    int* _vect = NULL;



public:
    Descriptor(string path);

    int get_Rows_Size();
    int get_Columns_Size();

    string* Load_Data();

    string Get_Cell_from_CSV(int columns, int row_idx, int col_idx);


    void update_Data(int row_idx, int col_idx, string new_value);

    void delete_Data(int rwo_idx);

    void add_Data(string path, int id, string name, string source, int price, string access);

    void sort_Data(int col_idx);

    void filter_Data(int col_idx, string filter_token);


};




