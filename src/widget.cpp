#include "widget.h"
#include "ui_widget.h"
#include <aria2/aria2.h>
#include <utility>
#include <memory>
#include <QString>
#include <cstring>
#include "aria2cpp.h"
#include <QMessageBox>
#include <iostream>
#include <QUrl>
#include <QTcpSocket>
#include <QFileDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("KU Loader 1.0.0");
}

void Widget::setUrl(std::string url)
{
    this->url = url;
}

std::string Widget::getUrl()
{
    return this->url;
}

void Widget::setDirectory(std::string directory)
{
    this->directory = directory;
}

std::string Widget::getdirectory()
{
    return this->directory;
}

void Widget::setFileName(std::string fname)
{
    this->fileName = fname;
}

std::string Widget::getFileName()
{
    return fileName;
}

int Widget::doDownload()
{

    auto url = this->url;


    const auto outputFileName = this->fileName;
    const auto outputDirectory = this->fileName;

    auto& session = aria2cpp::Session::init();
    session.set_global_option({{"max-overall-download-limit", "10M"},
                              {"max-overall-upload-limit", "10M"}});
    auto [response_value, id] = session.add_download(
            {url}, {{"out", outputFileName}, {"dir", outputDirectory}});

    if (response_value < 0) {
        QMessageBox::warning(this, "ERROR", "Failed to add downlaod.\nPlease provide a valid URL", QMessageBox::Ok);
        return EXIT_FAILURE;
    }
//            std::cout << "Added download. Download ID is:" << id << std::endl;

    auto start = std::chrono::steady_clock::now();
    for (int response_value = 0; (response_value = session.run()) == 1;) {
        auto now = std::chrono::steady_clock::now();
        auto count =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
                .count();
        if (count >= 2000) {
            start = now;
            printInfo();
        }
    }

    //create a sepaerate function to perform these;

        ui->progressBar->setValue(100);

        QMessageBox::information(this, "MESSAGE", "Your download is complete", QMessageBox::Ok);
        ui->downloadButton->setDisabled(false);
        ui->progressBar->reset();
        ui->urlPath->setText("");
        ui->directory->setText("");
        ui->fileName->setText("");
        ui->fileSize->setText("");
        ui->downloadSpeed->setText("");
        ui->uploadSpeed->setText("");


        return EXIT_SUCCESS;
}



void Widget::printInfo()
{
    auto& session = aria2cpp::Session::get();
//    auto stat = session.get_global_statistics(); // send this parameter instead



//    std::cerr << "Overall #Active:" << stat.numActive
//              << "#waiting:" << stat.numWaiting
//              << "Dspeed:" << stat.downloadSpeed / 1024 << "KB/s"
//              << "Uspeed:" << stat.uploadSpeed / 1024 << "KB/s" << std::endl;

    auto ids = session.get_active_downloads();
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    int index = 0;

    for (auto id : ids) {
        auto download_handle = session.get_download_handle(id);

        if (index < 1) {
            double size = download_handle->getTotalLength();


            ui->fileSize->setText(convertFileSize(size));
            index++;
        }

        if (download_handle) {

            double value = (download_handle->getTotalLength() > 0 ?
                           (100 * download_handle->getCompletedLength() /
                            download_handle->getTotalLength()) : 0);

            double speedInBytes = download_handle->getDownloadSpeed();

            double uploadSpeedInBytes = download_handle->getUploadSpeed();

            QString convertedSpeed = convertSpeed(speedInBytes);

            QString convertedUploadSpeed = convertSpeed(uploadSpeedInBytes);
            ui->progressBar->setValue(value);

            ui->downloadSpeed->setText(convertedSpeed);

            ui->uploadSpeed->setText(convertedUploadSpeed);



        }
    }
}

QString Widget::convertSpeed(double speedInBytes)
{
    double speedInKb;
    double speedInMb;

    if(speedInBytes > 1024) {
        speedInKb = speedInBytes / 1024;

        if(speedInKb > 1024) {
            speedInMb = speedInKb / 1024;
            return QString::number(speedInMb) + "MB/s";
        }
        else {
            return QString::number(speedInKb) + "KB/s";
        }
    }
    else {
        return QString::number(speedInBytes) + "B/s";
    }
}

QString Widget::convertFileSize(double sizeInByte)
{
    double sizeInKb, sizeInMb, sizeInGb;

    if (sizeInByte > 1024) {
        sizeInKb = sizeInByte / 1024;

        if(sizeInKb > 1024) {
            sizeInMb = sizeInKb / 1024;
        }
        else {
            return QString::number(sizeInKb) + "KB";
        }
        if(sizeInMb > 1024) {
            sizeInGb = sizeInMb / 1024;
            return QString::number(sizeInGb) + "GB";
        }
        else{
            return QString::number(sizeInMb) + "MB";
        }
    }
    else {
        return QString::number(sizeInByte) + "B";
    }
}

//stack overflow

bool Widget::checkURL(QString url)
{

    QUrl myURL(url);

    QTcpSocket socket;

    socket.connectToHost(url, 80);
    if(socket.waitForConnected()) {
        return true;
        socket.close();
    }
    else {
        return false;
    }
}

bool Widget::checkDirectory(QString path)
{

    QDir dir(path);

    if(!dir.exists()) {
        return false;
    }

    return true;

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_downloadButton_clicked()
{
    bool isDirectoryValid = checkDirectory(QString::fromStdString(getdirectory()));
    bool isUrlValid = checkURL(QString::fromStdString(getdirectory()));

    if(!isUrlValid) {
        QMessageBox::warning(this, "ERROR MESSAGE", "Your URL is invalid.\nPlease enter a valid URL", QMessageBox::Ok);
    }
    else {
        if(isDirectoryValid) {

    //    bool isValid = checkURL(QString::fromStdString(getUrl()));

    //    if (isValid == true) {
            ui->downloadButton->setDisabled(true);
            doDownload();
    //    }
    //    else {
    //        QMessageBox::warning(this, "ERROR MESSAGE", "Your URL is invalid.\nPlease enter a valid URL.", QMessageBox::Ok);
    //    }
        }

        else {
            QMessageBox::warning(this, "ERROR MESSAGE", "Your directory path is invalid.\nPlease enter a valid path", QMessageBox::Ok);

        }
    }

}

void Widget::on_quitButton_clicked()
{
    QApplication::quit();
}

void Widget::on_urlPath_editingFinished()
{

    this->setUrl(ui->urlPath->text().toStdString());

}

void Widget::on_urlPath_textChanged(const QString &arg1)
{
    this->setUrl(arg1.toStdString());
}

void Widget::on_fileName_editingFinished()
{
    this->setFileName(ui->fileName->text().toStdString());
}

void Widget::on_fileName_textChanged(const QString &arg1)
{
    this->setFileName(arg1.toStdString());
}



void Widget::on_browseButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose Directory");

    if(path.isEmpty()) {
        return;
    }
    else {
    ui->directory->setText(path);
    }
}

void Widget::on_directory_editingFinished()
{

//    bool isValid = checkDirectory(ui->directory->text());

//    if(isValid)
        this->setDirectory(ui->directory->text().toStdString());

//    else
//        QMessageBox::warning(this, "ERROR MESSAGE", "Your directory path is invalid.\nPlease enter valid path");

}





void Widget::on_directory_textChanged(const QString &arg1)
{
    this->setDirectory(arg1.toStdString());


}
