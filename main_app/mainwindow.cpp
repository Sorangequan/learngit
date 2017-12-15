#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagefocusqualityssim.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

QImage cvMat2QImage(const cv::Mat& mat);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ifq_(new ImageFocusQualitySSIM)
{
    ui->setupUi(this);
    connect(this, SIGNAL(displayImage(const QImage &)),ui->centralWidget, SLOT( displayImage(const QImage &)));
}

MainWindow::~MainWindow()
{
    if(ifq_){
        delete ifq_;
    }
    delete ui;

}

void MainWindow::evaluateImage(const cv::Mat &img)
{
    // for display
    QImage qimg = cvMat2QImage(img);
    emit displayImage(qimg);
    // calculate score
    float score = ifq_->evalQuality(img);
    // update UI
    ui->lineEdit->setText(QString::number(score));
}

void MainWindow::on_btn_selectfile_clicked()
{
    // select image
    QString fileName = select_image();
    // load image
    if(fileName == ""){
        QMessageBox::warning(this,"Error", "No image selected", QMessageBox::Ok,QMessageBox::NoButton);
        return ;
    }

    cv::Mat img = cv::imread(fileName.toStdString());
    evaluateImage(img);
}

QString MainWindow::select_image()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Image"), "./", tr("Image Files (*.png *.jpg *.bmp)"));
    return fileName;
}

QImage cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

