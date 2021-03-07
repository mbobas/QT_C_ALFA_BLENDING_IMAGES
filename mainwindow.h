#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

// Deklaracja głównej klasy naszego programu
// Jest ona podklasą klasy QMainWindow
// standardowej klasy Qt reprezentującej główne okno aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

// Sloty to specjalne funkcje (metody) wywoływane
// po wystąpieniu pewnych zdarzeń np. po kliknięciu przycisku
private slots:
    void on_exitButton_clicked();
    void on_cleanButton_clicked();

    void paintEvent(QPaintEvent*);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

    void on_checkBox_5_stateChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_5_currentIndexChanged(int index);

    void on_cleanButton_2_clicked();

    void on_cleanButton_3_clicked();

    void on_cleanButton_4_clicked();

    void on_cleanButton_5_clicked();

    void on_cleanButton_6_clicked();

    void on_cleanButton_7_clicked();

private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    static const int image_number = 5;
    QImage *images[image_number];
    QImage *default_image;

    //QImage *img, *img_org;
    int width, height, startX, startY;

    bool isActiveLayer[image_number];
    double alfaValues[image_number];
    int function[image_number];

    void clearSettings();
    void draw_images();
    void clean(QImage *image);
    void alfaBlednig(QImage *upImage, QImage *downImage, double alfa, int function);
    int mnozenie(int x, int y);
    int min(int x, int y);
    int max(int x, int y);
    int screenMode(int x, int y);
    int overlayMode(int x, int y);
    int diffMode(int x, int y);
    int AdditiveMode(int x, int y);
    int SubtractiveMode(int x, int y);
};

#endif // MAINWINDOW_H
