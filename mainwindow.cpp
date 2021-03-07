#include "mainwindow.h"
#include "ui_mainwindow.h"

// Konstruktor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Pobieramy współrzędne (względem głównego okna) lewego, górnego naroznika ramki
    startX = ui->frame->x();
    startY = ui->frame->y();

    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();

    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    default_image = new QImage(width, height, QImage::Format_RGB32);


    // Wczytujemy plik graficzny do drugiego obiektu klasy QImage
    // jeżeli ścieżka zaczyna się od dwukropka tzn. że plik
    // znajduje się w zasobach projektu
    images[0] = new QImage(":/1.jpg");
    images[1] = new QImage(":/2.jpg");
    images[2] = new QImage(":/3.jpg");
    images[3] = new QImage(":/4.jpg");
    images[4] = new QImage(":/5.jpg");

    clearSettings();
    clean(default_image);
    update();
}
//*****************************************************************************************************


// Destruktor
MainWindow::~MainWindow()
{
    delete ui;
    delete default_image;
    for (int i=0; i<=4; i++)
        delete images[i];
    delete [] *images;

}
//*****************************************************************************************************


// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);

    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    draw_images();
    p.drawImage(startX, startY, *default_image);

}
//*****************************************************************************************************

void MainWindow::draw_images() {
    QImage *copy_image = new QImage(width, height, QImage::Format_RGB32);

    clean(copy_image);

    for (int i=0; i<image_number; i++)
    {
         if(isActiveLayer[i])
         {

             alfaBlednig(images[i], copy_image, alfaValues[i], function[i]);

             *copy_image = default_image->copy(0,0,width,height);
         }
    }
    delete copy_image;
}




void MainWindow::alfaBlednig(QImage *upImage, QImage *downImage, double alfa, int function) {

    int red, green, blue;

    unsigned char *pic1, *pic2, *def_pic;
    def_pic = default_image->bits();
    pic1 = upImage->bits();
    pic2 = downImage->bits();

    switch (function)
    {
        case 0: //normal blending
        for (int i=0; i<height; i++)
        {
             for (int j=0; j<width; j++)
             {
             def_pic[width*4*i + 4 * j + 2] = alfa*pic1[width*4*i + 4 * j + 2]+(1.0-alfa)*pic2[width*4*i + 4 * j + 2];
             def_pic[width*4*i + 4 * j + 1] = alfa*pic1[width*4*i + 4 * j + 1]+(1.0-alfa)*pic2[width*4*i + 4 * j + 1];
             def_pic[width*4*i + 4 * j]     = alfa*pic1[width*4*i + 4 * j    ]+(1.0-alfa)*pic2[width*4*i + 4 * j    ];

            }
    }
        break;
        case 1: //mnozenie
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      mnozenie(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    mnozenie(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     mnozenie(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
             def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
             def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
             def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 2: //dark blending
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      min(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    min(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     min(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 3: //light blending
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      max(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    max(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     max(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 4: //screen mode
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      screenMode(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    screenMode(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     screenMode(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 5: //Overlay Mode
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      overlayMode(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    overlayMode(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     overlayMode(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 6: //Diffrent Mode
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      diffMode(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    diffMode(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     diffMode(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 7: //Additive Mode
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      AdditiveMode(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    AdditiveMode(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     AdditiveMode(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }
        break;
        case 8: //SubtractiveMode Mode
        for (int i=0; i<height; i++) {
             for (int j=0; j<width; j++)
             {
                 red =      SubtractiveMode(pic1[width*4*i + 4 * j + 2], pic2[width*4*i + 4 * j + 2]);
                 green =    SubtractiveMode(pic1[width*4*i + 4 * j + 1], pic2[width*4*i + 4 * j + 1]);
                 blue =     SubtractiveMode(pic1[width*4*i + 4 * j + 0], pic2[width*4*i + 4 * j + 0]);
                 def_pic[width*4*i + 4 * j + 2] = alfa*red+     (1.0-alfa)*pic2[width*4*i + 4 * j + 2];
                 def_pic[width*4*i + 4 * j + 1] = alfa*green+   (1.0-alfa)*pic2[width*4*i + 4 * j + 1];
                 def_pic[width*4*i + 4 * j]     = alfa*blue+    (1.0-alfa)*pic2[width*4*i + 4 * j    ];

        }
        }


    }
}
int MainWindow::mnozenie(int x, int y) {
    int wynik = x*y;
    return wynik >> 8;
}

int MainWindow::min(int x, int y) {
    if (x<y) {
        return x;
    } else {
        return y;
    }
}
int MainWindow::max(int x, int y) {
    if (x>y) {
        return x;
    } else {
        return y;
    }
}
int MainWindow::screenMode(int x, int y) {
    int wynik = 255-((255-x)*(255-y)>>8);
    return wynik;
}
int MainWindow::overlayMode(int x, int y) {
    int wynik;
    if (x<128){
        wynik = x*y>>7;
    } else if(x>=128) {
        wynik= 255-((255-x)*(255-y) >> 7);
    }
    return wynik;
}
int MainWindow::diffMode(int x, int y) {
    int wynik = abs(x-y);
    return wynik;
}

int MainWindow::AdditiveMode(int x, int y){
    int wynik;
    min(x+y,255);
    return wynik;
}
int MainWindow::SubtractiveMode(int x, int y){
    int wynik;
    max(x+y-255,0);
    return wynik;
}



// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaźnik na obiekt aplikacji
    // metoda quit() powoduje zakończenie działania aplikacji z kodem wyjścia 0 (sukces)
    qApp->quit();
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_cleanButton_clicked()
{
     clean(default_image);

    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}


// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::clean(QImage *image)
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;
    wsk = image->bits();

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
       //wsk = image->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[width*4*i + 4*j + 2] = 0; // ustawiamy składową ,,czerwoną'' na 0
            wsk[width*4*i + 4*j + 1] = 0; // ustawiamy składową ,,zielonąą'' na 0
            wsk[width*4*i + 4*j] = 0;   // ustawiamy składową ,,niebieską'' na 0


        }
    }
}

//*****************************************************************************************************
void MainWindow::clearSettings() {

    for (int i=0; i<image_number; i++) {
        isActiveLayer[i] = false;
        alfaValues[i] = 0.0;
        function[i] = 0;
    }
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setChecked(true);
    ui->checkBox_4->setChecked(true);
    ui->checkBox_5->setChecked(true);

    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);

    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_5->setCurrentIndex(0);

    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider_2->setValue(0);
    ui->horizontalSlider_3->setValue(0);
    ui->horizontalSlider_4->setValue(0);
    ui->horizontalSlider_5->setValue(0);


    clean(default_image);
    update();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isActiveLayer[0] = true;
        ui->horizontalSlider->setEnabled(true);
        ui->comboBox->setEnabled(true);
        alfaValues[0] = ui->horizontalSlider->value()/100.0;

    }
    else
    {
        isActiveLayer[0] = false;
        ui->horizontalSlider->setEnabled(false);
        ui->comboBox->setEnabled(false);
        alfaValues[0] = 0.0;

    }
    update();
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isActiveLayer[1] = true;
        ui->horizontalSlider_2->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        alfaValues[1] = ui->horizontalSlider_2->value()/100.0;

    }
    else
    {
        isActiveLayer[1] = false;
        ui->horizontalSlider_2->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        alfaValues[1] = 0.0;

    }
    update();
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isActiveLayer[2] = true;
        ui->horizontalSlider_3->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        alfaValues[2] = ui->horizontalSlider_3->value()/100.0;

    }
    else
    {
        isActiveLayer[2] = false;
        ui->horizontalSlider_3->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        alfaValues[2] = 0.0;

    }
    update();
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isActiveLayer[3] = true;
        ui->horizontalSlider_4->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        alfaValues[3] = ui->horizontalSlider_4->value()/100.0;

    }
    else
    {
        isActiveLayer[3] = false;
        ui->horizontalSlider_4->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        alfaValues[3] = 0.0;

    }
    update();
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        isActiveLayer[4] = true;
        ui->horizontalSlider_5->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        alfaValues[4] = ui->horizontalSlider_5->value()/100.0;

    }
    else
    {
        isActiveLayer[4] = false;
        ui->horizontalSlider_5->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        alfaValues[4] = 0.0;

    }
    update();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    alfaValues[0] = value/100.0;
    update();
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    alfaValues[1] = value/100.0;
    update();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    alfaValues[2] = value/100.0;
    update();
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    alfaValues[3] = value/100.0;
    update();
}

void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    alfaValues[4] = value/100.0;
    update();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    function[0]=index;
    update();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    function[1]=index;
    update();
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    function[2]=index;
    update();
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    function[3]=index;
    update();
}

void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    function[4]=index;
    update();
}

void MainWindow::on_cleanButton_2_clicked()
{
    QImage *copy_image2 = new QImage(width, height, QImage::Format_RGB32);
    *copy_image2 = default_image->copy(0,0,width,height);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.png)"));
    if (!fileName.isEmpty())
    {
      copy_image2->save(fileName);
    }
}

void MainWindow::on_cleanButton_3_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        images[0]->load(fileName);
        update();
    }
}

void MainWindow::on_cleanButton_4_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        images[1]->load(fileName);
        update();
    }
}

void MainWindow::on_cleanButton_5_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        images[2]->load(fileName);
        update();
    }
}

void MainWindow::on_cleanButton_6_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        images[3]->load(fileName);
        update();
    }
}

void MainWindow::on_cleanButton_7_clicked()
{
    // Wyświetlamy okno wyboru pliku.
    // Jeżeli plik zostanie wybrany funkcja zwróci jego nazwę
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "../obrazki", "Image Files (*.png *.jpg *.bmp)");

    // Jeżeli nazwa nie jest pusta (plik został wybrany)
    if(!fileName.isNull())
    {
        // Ładujemy wybrany plik
        images[4]->load(fileName);
        update();
    }
}
