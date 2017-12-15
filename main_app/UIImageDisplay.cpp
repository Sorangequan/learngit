#include "UIImageDisplay.h"
#include <QGraphicsScene>
#include <QMessageBox>
#include <QDebug>
#include <qmath.h>
#include <QWheelEvent>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <opencv2/opencv.hpp>
#include <iostream>

// for image rotate clockwise 90
//#define ROTATE_90

UIImageDisplay::UIImageDisplay(QWidget *parent) :
    QGraphicsView(parent),
    scene_(new QGraphicsScene(this)),
    pixmapItem_(new QGraphicsPixmapItem),
    displayed_(false),
    mousePressed_(false),
    hScrollBar_(0),
    vScrollBar_(0)
{
    setBackgroundRole(QPalette::Dark);  // view background color
    setAlignment(Qt::AlignCenter);  // item alignment in the view
    setCacheMode(QGraphicsView::CacheBackground);
    setDragMode(ScrollHandDrag);  // scroll hand drag mode is for view dragging
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // view transformation point
    setResizeAnchor(QGraphicsView::AnchorViewCenter);  // view resize point

    scene_->addItem(pixmapItem_);  // add item to scene
    this->setScene(scene_);  // add scene to view
}

UIImageDisplay::~UIImageDisplay()
{
    // pixmapItem_ has no parent, so delete is needed
    if(pixmapItem_){
        delete pixmapItem_;
        pixmapItem_ = nullptr;
    }
}

void UIImageDisplay::clearImage()
{
    // set a null QPixmap to pixmapItem_, that is to clear pixmapItem_ content
    pixmapItem_->setPixmap(QPixmap());
    scene_->setSceneRect(pixmapItem_->boundingRect());

    pixmapDisplayed(false);
}

void UIImageDisplay::pixmapDisplayed(bool display)
{
    displayed_ = display;
    emit imageDisplayed(display);
}

void UIImageDisplay::displayImage(const QImage &img)
{
    //TODO: progress bar for loading image
//    resetTransform();

    //if no image
    if(img.isNull())
    {
        qDebug() << "Can't display this picture because the image file has error.";
        return;
    }

    // QImage to QPixmap    
    pixmapItem_->setPixmap(QPixmap::fromImage(img));
    // make the scene rect same as the image rect
    scene_->setSceneRect(scene_->itemsBoundingRect());

    // if the real size of the image is larger than the view rect,
    // than transform the view to display the whole image;
    // if the image is not larger than the view rect, than display the real size of image
    QRectF bounds = QRectF(scene_->itemsBoundingRect().x(), scene_->itemsBoundingRect().y(),\
                           scene_->itemsBoundingRect().width(), scene_->itemsBoundingRect().height());
    if(bounds.width()>this->width() || bounds.height()>this->height())
    {
        fitInView(bounds, Qt::KeepAspectRatio);
    }

    // emit scale factor of the view
    emitScaleFactor();
    pixmapDisplayed(true);
}

void UIImageDisplay::displayImage(const QString &filePath)
{
    QString imgName = filePath.section('/', -1, -1);
    QImage img = QImage(filePath);

    displayFileInfo(img, imgName);
    displayImage(img);
}

void UIImageDisplay::displayImage(const QImage &img, const QString &fileName)
{
    displayFileInfo(img, fileName);
    displayImage(img);
}

void UIImageDisplay::displayFileInfo(const QImage &img, const QString &fileName)
{
    QString fileInfo;

    // calculate image size
    qreal imgKBytes = img.byteCount()/1024.0;
    QString imgSize = QString::number(imgKBytes) + "KB";
    if(imgKBytes/1024.0 > 1.0){
        imgKBytes = imgKBytes/1024.0;
        imgSize = QString::number(imgKBytes) + "MB";
        if(imgKBytes/1024.0 > 1.0){
            imgKBytes = imgKBytes/1024.0;
            imgSize = QString::number(imgKBytes) + "GB";
        }
    }

    // file name, image width * image height, image size
    fileInfo = fileName + " " + QString::number(img.width()) + "*"
            + QString::number(img.height()) + " " + imgSize;

    emit fileInfoSignal(fileInfo);
}

void UIImageDisplay::zoomIn()
{
    scaleImage(DEFAULT_SCALE_FACTOR);
    emitScaleFactor();
}

void UIImageDisplay::zoomOut()
{
    scaleImage(1. / DEFAULT_SCALE_FACTOR);
    emitScaleFactor();
}

void UIImageDisplay::originalSize()
{
    setScaleFactor(1);
    emitScaleFactor();
}

void UIImageDisplay::fitToWindow()
{
    // Scales the view matrix to ensure that the scene rectangle rect fits inside the viewport.
    QRectF bounds = QRectF(scene_->itemsBoundingRect().x(), scene_->itemsBoundingRect().y(), scene_->itemsBoundingRect().width(), scene_->itemsBoundingRect().height());
    fitInView(bounds, Qt::KeepAspectRatio);

    emitScaleFactor();
}

void UIImageDisplay::setScaleFactor(const qreal factor)
{
    if(pixmapItem_->pixmap().isNull()){
//        QMessageBox::warning(this, tr("Scale Error"),
//                                 tr("There is no picture opened."));

        return;
    }
#ifdef ROTATE_90
    // transform().m12(): vertical shearing factor
    qreal currentScale = transform().m12();
    qreal newScale = factor/currentScale;
    scale(newScale, newScale);
    pixmapItem_->setTransformationMode(
                transform().m12() < 1 ? Qt::SmoothTransformation : Qt::FastTransformation);
#else
    // transform().m11(): horizontal scaling factor
    qreal currentScale = transform().m11();
    qreal newScale = factor / currentScale;
    // scales the current view transformation by (x, y)
    scale(newScale, newScale);

    // balance the performance and display effect
    // SmoothTransformation: The resulting image is transformed using bilinear filtering
    // FastTransformation: The transformation is performed quickly, with no smoothing
    pixmapItem_->setTransformationMode(
                transform().m11() < 1 ? Qt::SmoothTransformation : Qt::FastTransformation);
#endif
}

qreal UIImageDisplay::getScaleFactor() const
{
#ifdef ROTATE_90
    return transform().m12();
#else
    return transform().m11();
#endif
}

void UIImageDisplay::wheelEvent(QWheelEvent *event)
{
    if(pixmapItem_->pixmap().isNull())
        return;
    qreal factor = qPow(DEFAULT_SCALE_FACTOR, event->delta() / 150.0);
    scaleImage(factor);
    emitScaleFactor();
    event->accept();
}

void UIImageDisplay::scaleImage(const qreal factor){
    if(pixmapItem_->pixmap().isNull()){
//        QMessageBox::warning(this, tr("Scale Error"),
//                                 tr("There is no picture opened."));

        return;
    }
#ifdef ROTATE_90
    qreal currentScale = transform().m12();
#else
    qreal currentScale = transform().m11();
#endif
    qreal newScale = currentScale * factor;
    qreal actualFactor = factor;

    // limit the scale bound
    if (newScale > 1000){
        actualFactor = 1000./currentScale;
    }else if (newScale < 0.001){
        actualFactor = 0.001/currentScale;
    }

    scale(actualFactor, actualFactor);

    // balance the performance and display effect
#ifdef ROTATE_90
    pixmapItem_->setTransformationMode(
                transform().m12() < 1 ? Qt::SmoothTransformation : Qt::FastTransformation);
#else
    pixmapItem_->setTransformationMode(
                transform().m11() < 1 ? Qt::SmoothTransformation : Qt::FastTransformation);
#endif
}

void UIImageDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mousePressed_ = true;
    }

    QGraphicsView::mousePressEvent(event);
}

void UIImageDisplay::mouseMoveEvent(QMouseEvent *event)
{
    // get current scroll bar value and set to hScrollBar_ and vScrollBar_
    if ((event->buttons() & Qt::LeftButton) && mousePressed_) {
        if (hScrollBar_ != this->horizontalScrollBar()->value()) {
            hScrollBar_ = this->horizontalScrollBar()->value();
            emit hScrollBarChanged(hScrollBar_);
        }
        if (vScrollBar_ != this->verticalScrollBar()->value()) {
            vScrollBar_ = this->verticalScrollBar()->value();
            emit vScrollBarChanged(vScrollBar_);
        }
    }

    QGraphicsView::mouseMoveEvent(event);
}

void UIImageDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && mousePressed_) {
        mousePressed_ = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

bool UIImageDisplay::displayed() const
{
    return displayed_;
}

void UIImageDisplay::setDisplayed(bool displayed)
{
    displayed_ = displayed;
}

void UIImageDisplay::emitScaleFactor()
{
#ifdef ROTATE_90
    qreal factor = transform().m12();
#else
    qreal factor = transform().m11();
#endif
    emit scaleFactorChanged(factor);
}

//void UIImageDisplay::resizeEvent(QResizeEvent *event)
//{
//    QGraphicsView::resizeEvent(event);
//    QTransform matrix;
//    qreal xscale =this->width()/this->sceneRect().width();
//    qreal yscale = this->height()/this->sceneRect().height();
//    xscale = yscale = qMin(xscale,yscale);
//    matrix.scale(xscale,yscale);
//    this->setTransform(matrix);
//}
