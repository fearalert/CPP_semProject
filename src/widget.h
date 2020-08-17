#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <cstring>
#include "aria2cpp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    std::string url;
    std::string fileName;
    std::string directory;

    Widget(QWidget *parent = nullptr);

    void setUrl(std::string);
    std::string getUrl();

    void setDirectory(std::string);
    std::string getdirectory();

    void setFileName(std::string);
    std::string getFileName();

    int doDownload();
    void printInfo();

    QString convertSpeed(double);

    QString convertFileSize(double);

    bool checkURL(QString);

    bool checkDirectory(QString);


    ~Widget();

private slots:
    void on_downloadButton_clicked();

    void on_quitButton_clicked();

    void on_urlPath_editingFinished();

    void on_urlPath_textChanged(const QString &arg1);

    void on_fileName_editingFinished();

    void on_fileName_textChanged(const QString &arg1);

    void on_browseButton_clicked();

    void on_directory_editingFinished();

    void on_directory_textChanged(const QString &arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
