#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <iostream>
#include<math.h>
#include <QMainWindow>
#include <algorithm>
#include <functional>
#include <QPainter>
#include <QImage>
#include <QIntValidator>
#include <QLineEdit>

#include <QMouseEvent>

class wektor
{
   public:
    int x;
    int y;

    wektor(int a, int b) : x(a), y(b) {}
};


class Polaczone_pkt
{
public:
    wektor A, B;

    Polaczone_pkt(int Ax, int Ay, int Bx, int By) : A(Ax, Ay), B(Bx, By) {}
};

class Sprawdz_przycisk
{
public:

    std::vector<wektor> punkty_kotrolne;

    // typ bool ktory mowi czy punkt konktrolny zostal przycisniety lewym przyciskiem, ustawiam na false
    bool czy_przesunac_pkt = false;
     unsigned int bufor;

     // typ bool sprawdzajacy czy punkt kontrolny zostal przycisniety (lewym przyciskiem lub prawym)
    bool Czy_wcisniety_pkt(int x, int y)
    {
        unsigned int i = 0;

        while (i < punkty_kotrolne.size())
        {
            if(x >= punkty_kotrolne[i].x - 5 && x <= punkty_kotrolne[i].x + 5)
                if(y >= punkty_kotrolne[i].y - 5 && y <= punkty_kotrolne[i].y + 5)
                {
                    bufor = i;
                    return true;
                }
            i++;
        }
        return false;
    }

    void Wcisniecie_myszki(QMouseEvent *event, wektor bufor_przycisk)
    {
        if(Czy_wcisniety_pkt(bufor_przycisk.x, bufor_przycisk.y))
        {
            if(event->button() == Qt::LeftButton)
                czy_przesunac_pkt = true;
            if(event->button() == Qt::RightButton)
            {
                punkty_kotrolne.erase(punkty_kotrolne.begin() + bufor);
                czy_przesunac_pkt = false;
            }
        }
        else
            punkty_kotrolne.push_back(bufor_przycisk);
    }
};





namespace Ui {
    class MyWindow;
}


class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);

    // Deklaracja destruktora
    ~MyWindow();

private:
    Ui::MyWindow *ui;


    // obraz do wyswietlenia
    QImage *do_wyswietlenia;
    // obraz przechowujace dane do przerzucenia do obrazu "do_wyswietlenia"
    QImage *tymczasowy_obraz;

    Sprawdz_przycisk spr_przycisk;

    int szer;
    int wys;
    int poczX;
    int poczY;

    bool Lewy_przycisk = false;
    void clear(QImage* img);


private slots:
    // void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void ScanLine(std::vector<wektor> points);


};

#endif // MYWINDOW_H
