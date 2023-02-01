#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
 #include <QMessageBox>
#include <string>
#include <fstream>

#include "Descriptor.h"

#include<QThread>

#include <opencv2/opencv.hpp>


#include <QFileDialog>

#include "Image.h"
#include <QImage>
#include <QMatrix4x4>
#include <QInputDialog>









// %%%%%%%%%%%%%%%%%%%%% Authentification %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


vector<string> split_text_AU(string text, string delimeter) {
    vector<string> splited_text;
    int pos = 0;
    while (pos != -1) {
        pos = text.find(delimeter);
        splited_text.push_back(text.substr(0, pos));
        text = text.substr(pos + delimeter.length());
    }
    return splited_text;
}






int get_csv_rows_AU(string path) {
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





int get_csv_columns_AU(string path) {
    fstream file;
    file.open(path, ios::in);

    string text;
    getline(file, text);
    int count = 0;


    for (int i = 0; i < text.size(); i++)
        if (text[i] == ',') count++;

    return count+1;
}



string Authentificate(string userName, string passWord, string account_DataBase_Path){


    fstream file;
    file.open(account_DataBase_Path, ios::in);
    string text;

    int rows = get_csv_rows_AU(account_DataBase_Path);
    int columns = get_csv_columns_AU(account_DataBase_Path);

    vector< vector<string> > data(rows, vector<string>(columns, ""));

    string mode = "";

    for (int i = 0; i < rows; i++) {

        getline(file, text);

        vector< string> descriptor = split_text_AU(text, ", ");
        data[i] = descriptor;

        //cout << "\n\n Data Base : " << descriptor[0] << "      Entered : " << userName;
        //cout << "\n\n Data Base : " << descriptor[1] << "      Entered : " << passWord;


        if ((descriptor[0] == userName) && (descriptor[1] == passWord)){
            return descriptor[2];
            //qDebug() << "premium";
        }
    }
    return "None";


}






QPixmap Mat__to_QPixmap(Mat image){

    int rows = image.rows, cols = image.cols;
    QImage image_Q(cols, rows, QImage::Format_Grayscale16);   // use the overloaded constructor

    for (int i=0;i<rows-2;i++){
        for (int j=0;j<cols-2;j++){
            image_Q.setPixel(i,j,image.at<double>(i,j));
            //qDebug() << "Pixel("<<i<< "," << j << " == " << i,j,image.at<double>(i,j);
        }
    }

    qDebug() << "Image Dim : "<< rows << ", " << cols;
    QTransform t;
    t.rotate(90);
    QPixmap Result = QPixmap::fromImage(QImage(image_Q.transformed(t)));
    return Result;
}








MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setWindowIcon(QIcon("/home/theghost/Pictures/Icons/Icone_1.png"));



    /*
    Image I("/home/theghost/Pictures/img-1.png");

    Mat img = I.load_Image(0);
    Mat histImage = I.show_Histogramm(img);

    imshow("histImage", histImage);
    waitKey();
    */


    ui->add_Data_Frame->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);
    ui->descriptor_Visualization->setVisible(false);

    ui->image_Frame->setVisible(false);



}

MainWindow::~MainWindow()
{

    delete ui;
}











void MainWindow::on_pushButton_clicked()
{


    string userName = ui->Login->text().toStdString();
    string passWord = ui->Password->text().toStdString();
    string account_DataBase_Path = "/home/theghost/Documents/Files/Accounts_DataBase.csv";


    QString test1 = ui->Login->text();
    QString test2 = ui->Password->text();
    qDebug() << test1 << test2;


    string mode = Authentificate(userName,
                                 passWord,
                                 account_DataBase_Path);

    cout << "\n\nmode  : " << mode;

    if (userName == "" || passWord == "" ){

        QMessageBox::critical(this, tr("Error"),
                                            tr("       Usernamme or password is missed\n"
                                               "\nPlease verify that you have already fill them ..."),
                                            QMessageBox::Ok);




    } else {
        qDebug() << "-----> Nothing ... ::" << QString::fromStdString(mode);
        if (mode == "Premium\r"){
            qDebug() << "-----> Nothing ... in Premium:: " << QString::fromStdString(mode);
            ui->Login_frame->setVisible(false);
            ui->Premium->setVisible(true);
            ui->Premium->show();
            ui->add_Data_Frame->setVisible(false);

            QMessageBox::information(this, tr("Information"),
                                                tr("Welcome ..!\n"
                                                   "we are happy to see you again :)"),
                                                QMessageBox::Ok);

        } else if (mode == "Basic\r"){
            qDebug() << "-----> Nothing ... in Basic:: " << QString::fromStdString(mode);
            ui->Login_frame->setVisible(false);
            ui->Premium->setVisible(false);
            ui->Basic->setVisible(true);
            ui->Basic->show();

            QMessageBox::information(this, tr("Information"),
                                                tr("Welcome ..!\n"
                                                   "we are happy to see you again :)"),
                                                QMessageBox::Ok);

        } else if (mode == "None"){
            QMessageBox::critical(this, tr("Access Denied"),
                                                tr("       Usernamme or password is incorrect\n"
                                                   "\nPlease verify that you have already an account..!"),
                                                QMessageBox::Ok);
        }
    }

}






// ################### Log OUT from Basic mode ########################
void MainWindow::on_Basic_Log_out_clicked()
{
    ui->Login->setText("");
    ui->Password->setText("");

    ui->Login_frame->setVisible(true);
    ui->Basic->setVisible(false);

    QMessageBox::information(this, tr("Information"),
                                        tr("       You just logged out       \n"
                                           "\n        See you later       "),
                                        QMessageBox::Ok);

}




// ################### Log OUT from premium mode ########################
void MainWindow::on_Premium_Log_out_clicked()
{
    ui->Login->setText("");
    ui->Password->setText("");

    ui->Login_frame->setVisible(true);
    ui->Premium->setVisible(false);

    QMessageBox::information(this, tr("Information"),
                                        tr("       You just logged out       \n"
                                           "\n        See you later       "),
                                        QMessageBox::Ok);
}







// ############## Load Data #####################
void MainWindow::on_load_Data_Button_clicked()
{


    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    ui->Descriptor_Table->setRowCount(rows);
    ui->Descriptor_Table->verticalHeader()->setVisible(true);

    ui->Descriptor_Table->sortByColumn(4,Qt::AscendingOrder);
    ui->Descriptor_Table->horizontalHeader()->sortIndicatorOrder();


    for(int i =0; i<rows; i++){
        for(int j=0; j<columns; j++){
            string cell = myData.Get_Cell_from_CSV(columns, i,j);
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(cell)));
        }
    }

    ui->Descriptor_Table->setColumnWidth(0, 200);
    ui->Descriptor_Table->setColumnWidth(1, 60);
    ui->Descriptor_Table->setColumnWidth(4, 75);
}


void MainWindow::on_add_Data_Button_clicked()
{
    on_load_Data_Button_clicked();

    ui->Descriptor_Top_Frame->setGeometry(10, 0, 881, 401); // 190
    ui->add_Data_Frame->setGeometry(10, 400, 721, 371);

    ui->add_Data_Frame->setVisible(true);

    ui->descriptor_Visualization->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);

    QString path_name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.tif *.pbm *.xpm *.jpg)"));

    ui->path_Name_Add_Data_Edited_Text->setText(path_name);


}

void MainWindow::on_set_Data_Button_clicked()
{
    on_load_Data_Button_clicked();

    ui->Descriptor_Top_Frame->setGeometry(10, 80, 881, 401); // 190
    ui->update_Data_Frame->setGeometry(10, 480, 721, 181);

    ui->update_Data_Frame->setVisible(true);

    ui->descriptor_Visualization->setVisible(false);
    ui->add_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);
}


void MainWindow::on_delete_Data_Button_clicked()
{
    on_load_Data_Button_clicked();

    ui->Descriptor_Top_Frame->setGeometry(10, 100, 881, 401); // 190
    ui->delete_Data_Frame->setGeometry(10, 500, 721, 91);

    ui->delete_Data_Frame->setVisible(true);

    ui->descriptor_Visualization->setVisible(false);
    ui->add_Data_Frame->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);
}


void MainWindow::on_sort_Data_clicked()
{
    on_load_Data_Button_clicked();

    ui->Descriptor_Top_Frame->setGeometry(10, 80, 881, 401); // 190
    ui->sort_Data_Frame->setGeometry(10, 500, 721, 91);

    ui->sort_Data_Frame->setVisible(true);

    ui->descriptor_Visualization->setVisible(false);
    ui->add_Data_Frame->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);
}


void MainWindow::on_filter_Data_clicked()
{
    on_load_Data_Button_clicked();





    ui->Descriptor_Top_Frame->setGeometry(10, 100, 881, 401); // 190
    ui->filter_Data_Frame->setGeometry(30, 500, 701, 131);

    ui->filter_Data_Frame->setVisible(true);

    ui->descriptor_Visualization->setVisible(false);
    ui->add_Data_Frame->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
}






void MainWindow::on_save_After_Add_Data_Button_clicked()
{
    //ui->add_Data_Frame->setVisible(false);
    string path_name = ui->path_Name_Add_Data_Edited_Text->text().toStdString();
    int id = ui->id_Add_Data_Edited_Text->text().toInt();
    string title = ui->title_Add_Data_Edited_Text->text().toStdString();
    string source = ui->source_Add_Data_Edited_Text->text().toStdString();
    int price = ui->price_Add_Data_Edited_Text->text().toInt();
    string access = "Not defined";
    if (ui->Allow_radioButton->isChecked()){
        access = "Allow";
    } else if (ui->Deny_radioButton->isChecked()){
        access = "Deny";
    }




    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    myData.add_Data(path_name, id, title, source, price, access);

    on_load_Data_Button_clicked();


    ui->path_Name_Add_Data_Edited_Text->setText("");
    ui->id_Add_Data_Edited_Text->setText("");
    ui->title_Add_Data_Edited_Text->setText("");
    ui->source_Add_Data_Edited_Text->setText("");
    ui->price_Add_Data_Edited_Text->setText("");



}


void MainWindow::on_save_After_Update_Data_Butto_clicked()
{
    int line_Number = ui->line_Number_Update_Data_Edited_Text->text().toInt();
    int column_Number = ui->column_Number_Update_Data_Edited_Text->text().toInt();
    string new_Value = ui->new_Value_Add_Data_Edited_Text->text().toStdString();

    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    myData.update_Data(line_Number, column_Number, new_Value);

    on_load_Data_Button_clicked();

    ui->line_Number_Update_Data_Edited_Text->setText("");
    ui->column_Number_Update_Data_Edited_Text->setText("");
    ui->new_Value_Add_Data_Edited_Text->setText("");
}


void MainWindow::on_save_After_Delete_Data_Button_clicked()
{
    int line_Number = ui->line_Number_Delete_Data_Edited_Text->text().toInt()-1;


    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    myData.delete_Data(line_Number);

    on_load_Data_Button_clicked();

    ui->line_Number_Delete_Data_Edited_Text->setText("");
}


void MainWindow::on_sort_By_Title_Button_clicked()
{
    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int line_Number = 2;
    myData.sort_Data(line_Number);

    on_load_Data_Button_clicked();
}


void MainWindow::on_sort_By_Price_Button_clicked()
{
    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int line_Number = 4;
    myData.sort_Data(line_Number);

    on_load_Data_Button_clicked();
}











Mat image;
Image I("a");
QString text = "";

void MainWindow::on_Descriptor_Table_doubleClicked(const QModelIndex &index)
{

    int row_idx = index.row();
    int col_idx = index.column();


    qDebug() << "-----> index ("<<row_idx<<","<<col_idx<< ")  : " << ui->Descriptor_Table->item(row_idx, col_idx)->text();



    text = ui->Descriptor_Table->item(row_idx, col_idx)->text();
    string id = ui->Descriptor_Table->item(row_idx, col_idx+1)->text().toStdString();
    string name = ui->Descriptor_Table->item(row_idx, col_idx+2)->text().toStdString();
    string source = ui->Descriptor_Table->item(row_idx, col_idx+3)->text().toStdString();
    string price = ui->Descriptor_Table->item(row_idx, col_idx+4)->text().toStdString();
    string access = ui->Descriptor_Table->item(row_idx, col_idx+5)->text().toStdString();


    string descriptor_Information = "Image name      : " + name +
                                    "\nId                          : " + id +
                                    "\nSource                : " + source +
                                    "\nPrice                   : " + price +
                                    "\nPath image       : " + text.toStdString() +
                                    "\nAccess                : " + access;



    ui->add_Data_Frame->setVisible(false);
    ui->update_Data_Frame->setVisible(false);
    ui->delete_Data_Frame->setVisible(false);
    ui->sort_Data_Frame->setVisible(false);
    ui->filter_Data_Frame->setVisible(false);

    ui->descriptor_Visualization->setGeometry(60, 460, 620, 220);
    ui->Descriptor_Top_Frame->setGeometry(10, 50, 731, 401);
    ui->descriptor_Visualization->setVisible(true);
    ui->descriptor_Visualization->setText(QString::fromStdString(descriptor_Information));

    Image I(text.toStdString());



    image = I.load_Image(1);




    cvtColor(image, image, COLOR_BGR2RGB);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)));
    ui->pre_Image_Figure->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)));




}



void MainWindow::on_laplacien_Button_clicked()
{
    vector<vector<double>> laplacien_filter
        {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}
        };



    Image I(text.toStdString());
    image = I.load_Image(0);
    Mat convolved_Image = I.applyConvolution(image, laplacien_filter, 0);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(convolved_Image.data, convolved_Image.cols, convolved_Image.rows, convolved_Image.step, QImage::Format_Grayscale8)));



}


void MainWindow::on_gradient_Button_clicked()
{
    vector<vector<double>> Gradient_filter_x
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };

    vector<vector<double>> Gradient_filter_y
        {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
        };

    Image I(text.toStdString());
    image = I.load_Image(0);
    Mat convolved_X_Image = I.applyConvolution(image, Gradient_filter_x, 0);
    Mat convolved_Y_Image = I.applyConvolution(image, Gradient_filter_y, 0);
    Mat convolved_Image = I.apply_Gradient_Filter(convolved_X_Image, convolved_Y_Image);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(convolved_Image.data, convolved_Image.cols, convolved_Image.rows, convolved_Image.step, QImage::Format_Grayscale8)));
}



void MainWindow::on_histogramm_Button_clicked()
{

    Image I(text.toStdString());
    Mat img = I.load_Image(0);

    Mat histImage = I.show_Histogramm(img);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(histImage.data, histImage.cols, histImage.rows, histImage.step, QImage::Format_RGB888)));

}


void MainWindow::on_average_Button_clicked()
{
    Image I(text.toStdString());
    Mat img = I.load_Image(0);

    Mat filterd_Image = I.apply_Average_Filter(img);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(filterd_Image.data, filterd_Image.cols, filterd_Image.rows, filterd_Image.step, QImage::Format_Grayscale8)));
}


void MainWindow::on_median_Button_clicked()
{
    Image I(text.toStdString());
    Mat img = I.load_Image(0);

    Mat filterd_Image = I.apply_Median_Filter(img);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(filterd_Image.data, filterd_Image.cols, filterd_Image.rows, filterd_Image.step, QImage::Format_Grayscale8)));
}


void MainWindow::on_r_Min_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}



void MainWindow::on_r_Max_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}


void MainWindow::on_g_Min_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}


void MainWindow::on_g_Max_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}


void MainWindow::on_b_Min_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}


void MainWindow::on_b_Max_HorizontalSlider_valueChanged(int value)
{
    int r_min_value = ui->r_Min_HorizontalSlider->value();
    int r_max_value = ui->r_Max_HorizontalSlider->value();

    int g_min_value = ui->g_Min_HorizontalSlider->value();
    int g_max_value = ui->g_Max_HorizontalSlider->value();

    int b_min_value = ui->b_Min_HorizontalSlider->value();
    int b_max_value = ui->b_Max_HorizontalSlider->value();


    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    Mat segmented_Image = I.apply_Segmentation_by_Color_Selection(img, r_min_value, r_max_value, g_min_value, g_max_value, b_min_value, b_max_value);


    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segmented_Image.data, segmented_Image.cols, segmented_Image.rows, segmented_Image.step, QImage::Format_RGB888)));
}



void MainWindow::on_actionImage_triggered()
{
    ui->frame_Visualization->setVisible(false);
    ui->image_Frame->setVisible(true);
}


void MainWindow::on_edge_Enhancement_Button_clicked()
{
    vector<vector<double>> laplacien_filter
        {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}
        };



    Image I(text.toStdString());
    image = I.load_Image(0);


    Mat edge_Enhanced_Image = I.applyConvolution(image, laplacien_filter, 1);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(edge_Enhanced_Image.data, edge_Enhanced_Image.cols, edge_Enhanced_Image.rows, edge_Enhanced_Image.step, QImage::Format_Grayscale8)));

}







void MainWindow::on_gray_Scale_Button_clicked()
{
    Image I(text.toStdString());
    Mat img = I.load_Image(0);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8)));
}


void MainWindow::on_rgb_Button_clicked()
{
    Image I(text.toStdString());
    Mat img = I.load_Image(1);

    cvtColor(img, img, COLOR_BGR2RGB);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888)));
}










void MainWindow::on_price_Min_SpinBox_valueChanged(int arg1)
{
    int min_Value = ui->price_Min_SpinBox->value();
    int max_Value = ui->price_Max_SpinBox->value();

    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);


    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    ui->Descriptor_Table->setRowCount(0);
    ui->Descriptor_Table->verticalHeader()->setVisible(true);

    vector< vector<string> > data(rows, vector<string>(columns, ""));
    vector<string> descriptor (6, "");

    int count = 0;
    for(int i =0; i<rows; i++){
        for(int j=0; j<columns; j++){
            string cell = myData.Get_Cell_from_CSV(columns, i,j);
            descriptor[j] = myData.Get_Cell_from_CSV(columns, i,j);
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(cell)));
        }

        if ((min_Value == 0) && (max_Value == 0) && (stoi(descriptor[4]) == 0)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "1) Free";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;


        } else if ((min_Value == 1000) && (max_Value == 1000) && (stoi(descriptor[4]) >= 1000)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "1) More than 1000";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;


        } else if ((stoi(descriptor[4]) >= min_Value) && (stoi(descriptor[4]) <= max_Value)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "1) Between";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;
        }


    }



    for(int i =0; i<count; i++){
        for(int j=0; j<columns; j++){
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(data[i][j])));
        }
    }

}


void MainWindow::on_price_Max_SpinBox_valueChanged(int arg1)
{
    int min_Value = ui->price_Min_SpinBox->value();
    int max_Value = ui->price_Max_SpinBox->value();

    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);


    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    ui->Descriptor_Table->setRowCount(0);
    ui->Descriptor_Table->verticalHeader()->setVisible(true);

    vector< vector<string> > data(rows, vector<string>(columns, ""));
    vector<string> descriptor (6, "");

    int count = 0;
    for(int i =0; i<rows; i++){
        for(int j=0; j<columns; j++){
            string cell = myData.Get_Cell_from_CSV(columns, i,j);
            descriptor[j] = myData.Get_Cell_from_CSV(columns, i,j);
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(cell)));
        }

        if ((min_Value == 0) && (max_Value == 0) && (stoi(descriptor[4]) == 0)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "2) Free";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;


        } else if ((min_Value == 1000) && (max_Value == 1000) && (stoi(descriptor[4]) >= 1000)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "2) More than 1000";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;


        } else if ((stoi(descriptor[4]) >= min_Value) && (stoi(descriptor[4]) <= max_Value)){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            qDebug() << "2) Between";
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;
        }


    }



    for(int i =0; i<count; i++){
        for(int j=0; j<columns; j++){
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(data[i][j])));
        }
    }

}


void MainWindow::on_allow_Filter_RadioButton_clicked()
{
    filter_Data("AL");

}


void MainWindow::on_deny_Filter_RadioButton_clicked()
{
    filter_Data("D");

}

void MainWindow::filter_Data(string access_type){

    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);


    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    ui->Descriptor_Table->setRowCount(0);
    ui->Descriptor_Table->verticalHeader()->setVisible(true);

    vector< vector<string> > data(rows, vector<string>(columns, ""));
    vector<string> descriptor (6, "");

    qDebug() << "Button is clicked ..!";

    int count = 0;
    for(int i =0; i<rows; i++){
        for(int j=0; j<columns; j++){
            string cell = myData.Get_Cell_from_CSV(columns, i,j);
            descriptor[j] = myData.Get_Cell_from_CSV(columns, i,j);
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(cell)));
        }


        if ((access_type == "AL") && (ui->allow_Filter_RadioButton->isChecked()) && (descriptor[5] == "Allow\r")){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;
        } else if ((access_type == "ND") && (ui->not_Defined_Filter_RadioButton->isChecked()) && (descriptor[5] == "Not defined\r")){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;
        } else if ((access_type == "D") && (ui->deny_Filter_RadioButton->isChecked()) && (descriptor[5] == "Deny\r")){
            ui->Descriptor_Table->insertRow( ui->Descriptor_Table->rowCount() );
            for(int j=0; j<columns; j++){
                string cell = myData.Get_Cell_from_CSV(columns, i,j);
                data[count][j] = cell;
            }
            count ++;
        }


    }



    for(int i =0; i<count; i++){
        for(int j=0; j<columns; j++){
            ui->Descriptor_Table->setItem( i, j, new QTableWidgetItem(QString::fromStdString(data[i][j])));
        }
    }

}


void MainWindow::on_not_Defined_Filter_RadioButton_clicked()
{
    filter_Data("ND");

}


void MainWindow::on_load_Data_Button_Basic_clicked()
{
    string path = "/home/theghost/Documents/Files/file.csv";
    Descriptor myData(path);

    int rows = myData.get_Rows_Size() ;
    int columns = myData.get_Columns_Size();

    ui->Descriptor_Table_Basic->setRowCount(0);
    ui->Descriptor_Table_Basic->horizontalHeader()->sortIndicatorOrder();


    int count = 0;
    for(int i =0; i<rows; i++){
        vector<string> descriptor(columns, "");

        for(int j=0; j<columns; j++){
            string cell = myData.Get_Cell_from_CSV(columns, i,j);
            descriptor[j] = cell;

        }
        qDebug() << QString::fromStdString(descriptor[5]);
        if (descriptor[5] == "Allow\r"){
            qDebug() << "----> " << QString::fromStdString(descriptor[5]);
            ui->Descriptor_Table_Basic->insertRow( ui->Descriptor_Table_Basic->rowCount() );
            for(int j=0; j<columns; j++){
                ui->Descriptor_Table_Basic->setItem( count, j, new QTableWidgetItem(QString::fromStdString(descriptor[j])));

            }
            count++;

        }
    }

    ui->Descriptor_Table_Basic->setColumnWidth(0, 200);
    ui->Descriptor_Table_Basic->setColumnWidth(1, 60);
    ui->Descriptor_Table_Basic->setColumnWidth(4, 75);
}


void MainWindow::on_Descriptor_Table_Basic_cellDoubleClicked(int row, int column)
{


    qDebug() << "-----> index ("<<row<<","<<column<< ")  : " << ui->Descriptor_Table_Basic->item(row, column)->text();



    text = ui->Descriptor_Table_Basic->item(row, column)->text();
    string id = ui->Descriptor_Table_Basic->item(row, column+1)->text().toStdString();
    string name = ui->Descriptor_Table_Basic->item(row, column+2)->text().toStdString();
    string source = ui->Descriptor_Table_Basic->item(row, column+3)->text().toStdString();
    string price = ui->Descriptor_Table_Basic->item(row, column+4)->text().toStdString();
    string access = ui->Descriptor_Table_Basic->item(row, column+5)->text().toStdString();


    string descriptor_Information = "Image name      : " + name +
                                    "\nId                          : " + id +
                                    "\nSource                : " + source +
                                    "\nPrice                   : " + price +
                                    "\nPath image       : " + text.toStdString() +
                                    "\nAccess                : " + access;




    ui->descriptor_Visualization_Basic->setVisible(true);
    ui->descriptor_Visualization_Basic->setText(QString::fromStdString(descriptor_Information));

    Image I(text.toStdString());



    image = I.load_Image(1);


    cvtColor(image, image, COLOR_BGR2RGB);

    ui->pre_Image_Figure_Basic->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)));
}




void MainWindow::on_canny_Button_clicked()
{
    Image I(text.toStdString());
    image = I.load_Image(1);


    Mat filterd_Image = I.apply_Canny_Filter(image);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(filterd_Image.data, filterd_Image.cols, filterd_Image.rows, filterd_Image.step, QImage::Format_Grayscale8)));

}


void MainWindow::on_segment_Detection_Button_clicked()
{
    Image I(text.toStdString());
    image = I.load_Image(1);


    Mat segment_Detection_Image = I.apply_hough_Algorithm(image, 80);
    cvtColor(segment_Detection_Image, segment_Detection_Image, COLOR_BGR2RGB);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segment_Detection_Image.data, segment_Detection_Image.cols, segment_Detection_Image.rows, segment_Detection_Image.step, QImage::Format_RGB888 )));

}


void MainWindow::on_t_Count_Hough_VerticalSlider_valueChanged(int value)
{

    qDebug() << "\n-----> " << value;
    Image I(text.toStdString());
    image = I.load_Image(1);


    Mat segment_Detection_Image = I.apply_hough_Algorithm(image, value);
    cvtColor(segment_Detection_Image, segment_Detection_Image, COLOR_BGR2RGB);

    ui->image_Figure->setPixmap(QPixmap::fromImage(QImage(segment_Detection_Image.data, segment_Detection_Image.cols, segment_Detection_Image.rows, segment_Detection_Image.step, QImage::Format_RGB888 )));

}

