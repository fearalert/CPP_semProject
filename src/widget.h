#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QProgressBar>
#include "aria2cpp.h"
#include <QThread>
#include <mythread.h>
#include "ui_widget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    static std::string url;
    static std::string directory;
    static std::string fname;


public:
    MyThread *thread;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    std::string getUrl() const;
    void setUrl(const std::string &value);

    std::string getDirectory() const;
    void setDirectory(const std::string &value);

    std::string getFname() const;
    void setFname(const std::string &value);

public:
    //check and convert
    QString convertSpeed(double);

    QString convertFileSize(double);

    bool checkDirectory(QString);

    //clear window for next download
    void resetWindow();

private slots:    
    void onNumberChanged(double, double, double, double);
    void ifIdIsOne();
    void onSuccess();
    void onFailure();

private slots:
   void on_quiteButton_clicked();
   void on_browseButton_clicked();
   void on_downloadButton_clicked();
   void on_pauseButton_clicked();
   void on_resumeButton_clicked();

   void on_pushButton_2_clicked();
   void on_pushButton_clicked();
   void on_pushButton_3_clicked();
   void on_pushButton_4_clicked();

   void on_urlPath_editingFinished();
   void on_directory_editingFinished();
   void on_fileName_editingFinished();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
