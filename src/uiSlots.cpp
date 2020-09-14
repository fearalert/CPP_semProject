#include "widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <thread>
#include <QUrl>
#include <QDesktopServices>


void Widget::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_pushButton_4_clicked()
{
    QString link = "https://github.com/AnuragTimilsina/KUloader";
    QDesktopServices::openUrl(QUrl(link));
}
void Widget::on_quiteButton_clicked()
{
    qDebug() << "Quit clicked";
    QApplication::quit();
}

void Widget::on_browseButton_clicked()
{
    qDebug() << "Browse clicked";
    QString path = QFileDialog::getExistingDirectory(this, "Choose Directory");

    if(path.isEmpty())
        return;
    else {
        ui->directory->setText(path);
        setDirectory(path.toStdString());
    }
}

void Widget::on_downloadButton_clicked()
{
    qDebug() << "Download CLicked";
    bool isDirectoryValid = checkDirectory(QString::fromStdString(getDirectory()));

    if(isDirectoryValid) {
        ui->downloadButton->setDisabled(true);
//        std::thread worker(&Widget::doDownload, this);
//            if(this->isStopped) {
//                worker.join();
//            }
//            doDownload();
        thread->start();
    }
    else {
        QMessageBox::warning(this, "ERROR MESSAGE", "Your directory path is invalid.\nPlease enter a valid path", QMessageBox::Ok);
    }
}

void Widget::on_pauseButton_clicked()
{
    qDebug() << "Pause Clicked";
    int value = thread->session->pause_download(thread->tempID);
    qDebug() << value;
}
void Widget::on_resumeButton_clicked()
{
    qDebug() << "Resume Clicked";
    int value = thread->session->resume_download(thread->tempID);
    qDebug() << value;
}

void Widget::on_urlPath_editingFinished()
{
    this->setUrl(ui->urlPath->text().toStdString());
}

void Widget::on_directory_editingFinished()
{
    this->setDirectory(ui->directory->text().toStdString());
}

void Widget::on_fileName_editingFinished()
{
    this->setFname(ui->fileName->text().toStdString());
}

void Widget::onNumberChanged(double fileSize, double progressValue, double downSpeedInBytes, double upSpeedInBytes)
{
    ui->fileSize->setText(this->convertFileSize(fileSize));

    QString convertedSpeed = this->convertSpeed(downSpeedInBytes);

    QString convertedUploadSpeed = this->convertSpeed(upSpeedInBytes);
    ui->progressBar->setValue(progressValue);
    ui->downloadSpeed->setText(convertedSpeed);

    ui->uploadSpeed->setText(convertedUploadSpeed);
}

void Widget::ifIdIsOne()
{
    QMessageBox::warning(this, "ERROR MESSAGE", "Downnload Error.\nPlease check if your URL is valid.");
    ui->downloadButton->setDisabled(false);
}
void Widget::onSuccess()
{
    this->resetWindow();
}
void Widget::onFailure()
{
    QMessageBox::warning(this, "ERROR MESSAGE", "Download Failed.\nPlease check if your URL is valid");
    ui->downloadButton->setDisabled(false);
}

void Widget::resetWindow()
{
    ui->progressBar->setValue(100);

    QMessageBox::information(this, "MESSAGE", "Your download is complete", QMessageBox::Ok);
//    this->isStopped = true;
    ui->downloadButton->setDisabled(false);
    ui->progressBar->reset();
    ui->urlPath->setText("");
    ui->directory->setText("");
    ui->fileName->setText("");
    ui->fileSize->setText("");
    ui->downloadSpeed->setText("");
    ui->uploadSpeed->setText("");


//    Widget::progress->setValue(100);

//    QMessageBox::information(this, "MESSAGE", "Your download is complete", QMessageBox::Ok);
//    this->isStopped = true;
//    Widget::downloadButton->setDisabled(false);
//    Widget::progress->reset();
//    Widget::urlPath->setText("");
//    Widget::directoryPath->setText("");
//    Widget::fileName->setText("");
//    Widget::fileSize->setText("");
//    Widget::downloadSpeed->setText("");
//    Widget::uploadSpeed->setText("");
}
