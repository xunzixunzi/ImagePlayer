#ifndef IMAGECAROUSEL_H
#define IMAGECAROUSEL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QPushButton>
#include <QLayout>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSize>
#include <QLineEdit>
#include <QIntValidator>

class ImageCarousel : public QWidget
{
    Q_OBJECT

public:
    ImageCarousel(QWidget *parent = nullptr);
    ~ImageCarousel();

private:
    // 按钮
    QPushButton* btnStart;
    QPushButton* btnOpen;
    QPushButton* btnClose;
    QPushButton* btnNext;
    QPushButton* btnPrevious;
    QPushButton* btnDelete;
    QPushButton* btnExport;
    // 图片显示
    QPixmap pixImage;
    QLabel* labelImage;
    QTimer* timer;
    QList<QFileInfo>* fileInfo;
    int imageSerialNum = 0;
    bool startFlag = 0;
    // 延时
    QLabel* labelDelay;
    QLineEdit* lEditDelay;
    int msec = 1000;
    QString imageDir;
    // 布局
    QGridLayout* gLay;
    QHBoxLayout* hLayDelay;
    QHBoxLayout* hLaySwitch;
    QHBoxLayout* hLayTool;
    const int windowWidth = 800;
    const int windowHeight = 600;
    int deletFlag = 1;
    QLabel* labelPath;
    QString windowTitle = "图片播放器";
    void initUI();
    void connectSignalSlot();


private slots:
    void imageDisplay();
    void imageStartPause();
    void imageRead();
    void imagePrevious();
    void imageNext();
    void imageDelet();
    void serialNumPlus();
    void setMsec();
};
#endif // IMAGECAROUSEL_H
