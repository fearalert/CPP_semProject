#include "widget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
      setWindowTitle("KU Loader 1.0.0");
          setFixedSize(1570,800);
       QPixmap pix(":/image/logo.png");
       QPixmap pix1(":/image/about.png");

        int w = ui->logo->width();
        int h = ui->logo->height();
        ui->logo->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->logo2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
        ui->about->setPixmap(pix1.scaled(w,h,Qt::KeepAspectRatio));
        ui->pushButton->setIcon(QIcon(":/image/house.png"));
        ui->pushButton_2->setIcon(QIcon(":/image/download.png"));
        ui->pushButton_3->setIcon(QIcon(":/image/info.png"));
        ui->pauseButton->setIcon(QIcon(":/image/pause.png"));
        ui->resumeButton->setIcon(QIcon(":/image/resume.png"));

    thread = new MyThread;

    connect(thread, SIGNAL(numberChanged(double, double, double, double)),
            this, SLOT(onNumberChanged(double, double, double, double)));
    connect(thread, SIGNAL(idIsOne()), this, SLOT(ifIdIsOne()));
    connect(thread, SIGNAL(success()), this, SLOT(onSuccess()));
    connect(thread, SIGNAL(failure()), this, SLOT(onFailure()));
}

std::string Widget::fname = "";
std::string Widget::url = "";
std::string Widget::directory = "";


Widget::~Widget()
{
  delete ui;
}




