#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include "Utils.cpp"

#include <QMainWindow>
#include "Descriptor.h"
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_Basic_Log_out_clicked();

    void on_Premium_Log_out_clicked();

    void on_load_Data_Button_clicked();

    void on_add_Data_Button_clicked();

    void on_set_Data_Button_clicked();

    void on_delete_Data_Button_clicked();

    void on_sort_Data_clicked();

    void on_save_After_Add_Data_Button_clicked();

    void on_save_After_Update_Data_Butto_clicked();

    void on_save_After_Delete_Data_Button_clicked();

    void on_sort_By_Title_Button_clicked();

    void on_sort_By_Price_Button_clicked();


    void on_Descriptor_Table_doubleClicked(const QModelIndex &index);


    void on_laplacien_Button_clicked();

    void on_gradient_Button_clicked();

    void on_average_Button_clicked();

    void on_median_Button_clicked();

    void on_r_Min_HorizontalSlider_valueChanged(int value);

    void on_r_Max_HorizontalSlider_valueChanged(int value);

    void on_g_Min_HorizontalSlider_valueChanged(int value);

    void on_g_Max_HorizontalSlider_valueChanged(int value);

    void on_b_Min_HorizontalSlider_valueChanged(int value);

    void on_b_Max_HorizontalSlider_valueChanged(int value);


    void on_actionImage_triggered();

    void on_edge_Enhancement_Button_clicked();

    void on_gray_Scale_Button_clicked();

    void on_rgb_Button_clicked();

    void on_histogramm_Button_clicked();



    void on_filter_Data_clicked();

    void on_price_Min_SpinBox_valueChanged(int arg1);

    void on_price_Max_SpinBox_valueChanged(int arg1);

    void on_allow_Filter_RadioButton_clicked();

    void on_deny_Filter_RadioButton_clicked();

    void on_not_Defined_Filter_RadioButton_clicked();


    void filter_Data(string access_type);

    void on_load_Data_Button_Basic_clicked();

    void on_Descriptor_Table_Basic_cellDoubleClicked(int row, int column);

    void on_canny_Button_clicked();

    void on_segment_Detection_Button_clicked();

    void on_t_Count_Hough_VerticalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
