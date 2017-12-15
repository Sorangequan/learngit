/**
 * Class UIImageDisplay is a UI class inherited from QGraphicsView which
 * can be used to display a graphic view. It implemented image display
 * and image scale.
 * By default, the mouse wheel event will cause image scale, and pressing
 * downthe left button can drag and move the image, also there are four
 * scale slots for easily connecting.
 */

#ifndef UIIMAGEDISPLAY_H
#define UIIMAGEDISPLAY_H

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPixmapItem;

const qreal DEFAULT_SCALE_FACTOR = 1.25;

/**
 * @class UIImageDisplay UIImageDisplay.h UIImageDisplay.cpp
 * @brief For single image display, inherited from QGraphicsView,
 * use graphics view framework, integrated scene and pixmap item with view.
 */
class UIImageDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    /**
     * @brief constructor: init scene and set view property
     */
    explicit UIImageDisplay(QWidget *parent = 0);
    /**
     * @brief destructor: delete all graphics item without parent
     */
    virtual ~UIImageDisplay();
    /**
     * @return return true if image displayed, otherwise return false
     */
    bool displayed() const;
    /**
     * @brief set image displayed or not
     */
    void setDisplayed(bool displayed);

public slots:
    /**
     * @brief accept a QImage format reference to display image
     */
    void displayImage(const QImage &img);
    /**
     * @brief read an image from file path and display
     * @param filePath: image path
     */
    void displayImage(const QString &filePath);
    /**
     * @brief displayImage: Override function of displayImage(const QImage &img)
     * @param img: QImage format
     * @param fileName: image file name, not path
     */
    void displayImage(const QImage &img, const QString &fileName);
    /**
     * @brief zoom in the image
     */
    void zoomIn();
    /**
     * @brief zoom out the image
     */
    void zoomOut();
    /**
     * @brief display the original size the image
     */
    void originalSize();
    /**
     * @brief zoom the image to fit the window size
     */
    void fitToWindow();
    /**
     * @brief accept qreal or double to scale the image
     */
    void setScaleFactor(const qreal factor);
    /**
     * @brief get current scale factor of the image
     * @return scale factor in double
     */
    qreal getScaleFactor() const;
    /**
     * @brief set pixmapItem_ as null
     */
    void clearImage();

signals:
    /**
     * @brief get new scale factor
     * @param factor: current view scale factor
     */
    void scaleFactorChanged(const qreal factor);
    void imageDisplayed(bool);
    /**
     * @brief file name, image width * image height, image size
     */
    void fileInfoSignal(const QString&);
    /**
     * @brief get horizontal scrollBar changed size in integer
     */
    void hScrollBarChanged(int);
    /**
     * @brief get vertical scrollBar changed size in integer
     */
    void vScrollBarChanged(int);

protected:
    /**
     * @brief wheel event will cause image scale, the scale factor is different with DEFAULT_SCALE_FACTOR
     */
    void wheelEvent(QWheelEvent *event);
    void scaleImage(const qreal factor);
    /**
     * @brief to emit the scrollbar value when move the image
     */
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene *scene_;
    QGraphicsPixmapItem *pixmapItem_;
    bool displayed_;
    bool mousePressed_;
    int hScrollBar_;
    int vScrollBar_;
    void emitScaleFactor();
    /**
     * @brief get the detail file info of the image
     */
    void displayFileInfo(const QImage &img, const QString &fileName);
    /**
     * @brief set whether the pixmap is displayed
     */
    void pixmapDisplayed(bool display);
};

#endif // UIIMAGEDISPLAY_H
