#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <opencv2/opencv.hpp>

class ImageFocusQualityInterface;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void evaluateImage(const cv::Mat &img);
signals:
    void displayImage(const QImage &img);

private slots:
    void on_btn_selectfile_clicked();

private:
    Ui::MainWindow *ui;
    ImageFocusQualityInterface * ifq_;
    QString select_image();

};

#endif // MAINWINDOW_H
