#include "mywindow.h"
#include "ui_mywindow.h"
#include <math.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cstdlib>

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{

    ui->setupUi(this);


    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();

    do_wyswietlenia = new QImage(szer,wys,QImage::Format_RGB32);



    clear(do_wyswietlenia);

}

MyWindow::~MyWindow()
{
    delete ui;
}


void MyWindow::ScanLine(std::vector<wektor> points)
{

    std::vector<Polaczone_pkt> polaczone_pkt;
    int x;
    int Ax,Ay,Bx,By;
    unsigned int ilosc_przeciec;
    int y_min = points[0].y;
    int y_max = points[0].y;
    int tab[170];
    int p;
    for(unsigned int i = 0; i < points.size() ; i++)
    {
        if(points[i].y > y_max)
            y_max = points[i].y;
        if(points[i].y < y_min)
            y_min = points[i].y;

        if(i+1 < points.size())
        polaczone_pkt.push_back(Polaczone_pkt({points[i].x, points[i].y, points[i+1].x, points[i+1].y}));
        if (i == points.size()-1)
        {
            polaczone_pkt.push_back(Polaczone_pkt({points[i].x, points[i].y, points[0].x, points[0].y}));
        }

    }
        for(int y = y_min; y <= y_max; y++)
        {
            ilosc_przeciec = 0;
            for(unsigned int i = 0; i < polaczone_pkt.size(); i++)
            {
                if((polaczone_pkt[i].B.y >= y && polaczone_pkt[i].A.y <= y) || (polaczone_pkt[i].B.y <= y && polaczone_pkt[i].A.y >= y))
                {
                    if(polaczone_pkt[i].B.y <= polaczone_pkt[i].A.y )
                    {
                        Ax = polaczone_pkt[i].A.x;
                        Ay = polaczone_pkt[i].A.y;
                        Bx = polaczone_pkt[i].B.x;
                        By = polaczone_pkt[i].B.y;
                    }
                    else if (polaczone_pkt[i].B.y > polaczone_pkt[i].A.y)
                    {
                        Ax = polaczone_pkt[i].B.x;
                        Ay = polaczone_pkt[i].B.y;
                        Bx = polaczone_pkt[i].A.x;
                        By = polaczone_pkt[i].A.y;
                    }

                   if(By < Ay)
                   {
                       p = Ay;
                   }
                   else
                   {
                       p = By;
                   }

                    if(y != p)
                    {
                        x = int(Ax + (float)(y - Ay)*(Bx - Ax)/(By - Ay));
                        tab[ilosc_przeciec] = x;
                        ilosc_przeciec++;
                    }
                }


            }
            std::sort(tab, tab + ilosc_przeciec);

            for(unsigned int i = 0; i < ilosc_przeciec; i++)
             {
                if(i%2==0)
                while(tab[i] < tab[i+1])
                {
                   x = tab[i];
                   unsigned char *ptr;
                   int szer = do_wyswietlenia->width();
                   int wys = do_wyswietlenia->height();
                   if(x < 0 || y < 0 || x >= szer || y >= wys)
                   return;
                   ptr = do_wyswietlenia->bits();
                   ptr[szer*4*y + 4*x] = 0;
                   ptr[szer*4*y + 4*x+1] = 255;
                   ptr[szer*4*y + 4*x+2] = 0;
                   tab[i]++;
                }
              }

        }

}



void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    // rysuje punkty kontrolne w ksztalcie kwadratu------------------------------------------------------
    for(unsigned int i = 0; i < spr_przycisk.punkty_kotrolne.size(); i++)
        {
            for(int x = spr_przycisk.punkty_kotrolne[i].x - 3; x < spr_przycisk.punkty_kotrolne[i].x +3; x++)
            {
                for(int y = spr_przycisk.punkty_kotrolne[i].y - 3; y < spr_przycisk.punkty_kotrolne[i].y + 3; y++)
                {
                    unsigned char *ptr;
                    int szer = do_wyswietlenia->width();
                    int wys = do_wyswietlenia->height();
                    if(x < 0 || y < 0 || x >= szer || y >= wys)
                        return;
                    ptr = do_wyswietlenia->bits();
                    ptr[szer*4*y + 4*x] = 0;
                    ptr[szer*4*y + 4*x+1] = 255;
                    ptr[szer*4*y + 4*x+2] = 0;
                }
            }
        }
// -----------------------------------------------------------------------------------------------------------


    // jezeli beda przynajmniej 2 punkty kontrolne to rysuj

    /*
    int Ax,Ay;
    int Bx,By;
    int tak = 0;
    if(spr_przycisk.punkty_kotrolne.size()>2)
    {
       Ax = spr_przycisk.punkty_kotrolne[0].x;
       Ay = spr_przycisk.punkty_kotrolne[0].y;
       Bx = spr_przycisk.punkty_kotrolne[spr_przycisk.punkty_kotrolne.size()-1].x;
       By = spr_przycisk.punkty_kotrolne[spr_przycisk.punkty_kotrolne.size()-1].y;

       for(int x = Ax - 3; x < Ax +3; x++)
       {
           for(int y = Ay - 3; y < Ay + 3; y++)
           {
               if((Bx == x) && (By ==y))
               {
                   tak = 1;
               }
           }
       }

    }
    if(tak == 1)
    {
    */
    ScanLine(spr_przycisk.punkty_kotrolne);
    //tak = 0;
    // }

    p.drawImage(poczX,poczY,*do_wyswietlenia);

}



void MyWindow::clear(QImage(* img))
{
    unsigned char *ptr;

    ptr = img->bits();

    int i,j;

    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j]=255;
            ptr[szer*4*i + 4*j + 1] = 255;
            ptr[szer*4*i + 4*j + 2] = 255;
        }
    }
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    x -= poczX;
    y -= poczY;

    if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys))
    {
        spr_przycisk.Wcisniecie_myszki(event, wektor(x, y));
    }

    update();
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();

    x -= poczX;
    y -= poczY;

    update();
}


void MyWindow::on_exitButton_clicked()
{
    qApp->quit();
}


