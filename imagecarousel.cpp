#include "imagecarousel.h"

ImageCarousel::ImageCarousel(QWidget *parent)
    : QWidget(parent)
{
    timer = new QTimer(this);

    // 获取系统默认图片文件夹路径
    imageDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

    initUI();
    connectSignalSlot();

    this->setWindowTitle(windowTitle);
    this->setMinimumSize(windowWidth, windowHeight);
    this->setWindowIcon(QIcon(":/img/img/play.png"));
}

ImageCarousel::~ImageCarousel()
{
}

void ImageCarousel::initUI()
{
    // 完成图片加载
    labelImage = new QLabel(this);
    labelImage->setStyleSheet("border:1px solid black");
    labelImage->setAlignment(Qt::AlignCenter);
    labelPath = new QLabel(this);
    labelPath->setMaximumHeight(15);

    // 添加按钮
    btnOpen = new QPushButton("打开", this);
    btnOpen->setMaximumWidth(70);
    btnStart = new QPushButton("开始", this);
    btnStart->setMaximumWidth(70);
    btnStart->setEnabled(false);
    btnClose = new QPushButton("关闭", this);
    btnClose->setMaximumWidth(70);
    btnPrevious = new QPushButton("<", this);
    btnPrevious->setMaximumWidth(70);
    btnPrevious->setEnabled(false);
    btnNext = new QPushButton(">", this);
    btnNext->setMaximumWidth(70);
    btnNext->setEnabled(false);
    btnDelete = new QPushButton("删除", this);
    btnDelete->setMaximumWidth(70);
    btnDelete->setEnabled(false);
    btnExport = new QPushButton("导出", this);
    btnExport->setMaximumWidth(70);
    btnExport->setEnabled(false);

    // 延时控件
    labelDelay = new QLabel("间隔（秒）", this);
    labelDelay->setMaximumWidth(70);
    lEditDelay = new QLineEdit(QString::number(msec/1000), this);
    lEditDelay->setMaximumWidth(70);
    lEditDelay->setAlignment(Qt::AlignCenter);
    lEditDelay->setValidator(new QIntValidator(1, 99, this));  //设置QLineEdit只能输入数字

    //延时布局
    hLayDelay = new QHBoxLayout();
    hLayDelay->addWidget(labelDelay);
    hLayDelay->addWidget(lEditDelay);
    hLayDelay->addStretch();

    // 切换按钮布局
    hLaySwitch = new QHBoxLayout();
    hLaySwitch->addStretch();
    hLaySwitch->addWidget(btnPrevious);
    hLaySwitch->addWidget(btnStart);
    hLaySwitch->addWidget(btnNext);
    hLaySwitch->addStretch();

    // 功能按钮布局
    hLayTool = new QHBoxLayout();
    hLayTool->addStretch();
    hLayTool->addWidget(btnDelete);
    hLayTool->addWidget(btnExport);

    // 添加主布局
    gLay = new QGridLayout(this);
    gLay->addWidget(btnOpen, 0, 0, 1, 1);
    gLay->addLayout(hLayDelay, 0, 1, 1, 2);
    gLay->addLayout(hLaySwitch, 0, 3, 1, 3);
    gLay->addLayout(hLayTool, 0, 6, 1, 2);
    gLay->addWidget(btnClose, 0, 8, 1, 1);
    gLay->addWidget(labelImage, 1, 0, 3, 9);
    gLay->addWidget(labelPath, 4, 0, 1, 9);
}

void ImageCarousel::connectSignalSlot()
{
    connect(btnOpen, &QPushButton::clicked, this, &ImageCarousel::imageRead);
    connect(btnStart, &QPushButton::clicked, this, &ImageCarousel::imageStartPause);
    connect(btnClose, &QPushButton::clicked, this, &ImageCarousel::close);
    connect(timer, &QTimer::timeout, this, &ImageCarousel::serialNumPlus);
    connect(timer, &QTimer::timeout, this, &ImageCarousel::imageDisplay);
    connect(btnPrevious, &QPushButton::clicked, this, &ImageCarousel::imagePrevious);
    connect(btnNext, &QPushButton::clicked, this, &ImageCarousel::imageNext);
    connect(btnDelete, &QPushButton::clicked, this, &ImageCarousel::imageDelet);
    connect(lEditDelay, &QLineEdit::returnPressed, this, &ImageCarousel::setMsec);
}

void ImageCarousel::imageRead()
{
    // 打开文件系统选择文件
    QString imagePath = QFileDialog::getOpenFileName(
                this, "选择一张图片", imageDir,
                "images(*.png *jpeg *bmp *.gif *.jpg)");

    // 获取文件夹路径
    QString temp = imagePath;
    for (int i = temp.length(); i > 0; i--)
    {
        if (temp[i - 1] == "/")
        {
            imageDir = temp;
            break;
        }
        else
        {
            temp.chop(1);
        }
    }

    // 设置读取文件的过滤器
    QStringList filter;
    filter << "*.png" << "*.jpg" << "*.bmp"
           << "*.gif" << "*.jpeg";

    // 读取文件
    QDir* dir = new QDir(imageDir);
    dir->setNameFilters(filter);
    fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    for (int i = 0; i < fileInfo->count(); i++)
    {
        if (imagePath == fileInfo->at(i).filePath())
        {
            imageSerialNum = i;
            break;
        }
    }
    if (imagePath != NULL)
    {
        imageDisplay();
        btnStart->setEnabled(true);
        btnPrevious->setEnabled(true);
        btnNext->setEnabled(true);
        btnDelete->setEnabled(true);
    }
}

void ImageCarousel::imageDisplay()
{
    QString temp = fileInfo->at(imageSerialNum).filePath();
    pixImage.load(temp);
    if (pixImage.size().width() > labelImage->size().width() ||
        pixImage.size().height() > labelImage->size().height())
        pixImage = pixImage.scaled(labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (pixImage.width() == 0 && pixImage.height() == 0)
        imageNext();
    else
        labelImage->setPixmap(pixImage);

    for (int i = 0; i < temp.length(); i++)
    {
        if (temp[i] == "/")
            temp.replace(i, 1, "\\");
    }
    labelPath->setText(temp);
    this->setWindowTitle(windowTitle +" @" + fileInfo->at(imageSerialNum).fileName());
}

void ImageCarousel::imageStartPause()
{
    deletFlag = 1;
    if (startFlag == 0)
    {
        timer->start(msec);
        startFlag = 1;
        btnPrevious->setEnabled(false);
        btnNext->setEnabled(false);
        btnDelete->setEnabled(false);
        btnStart->setText("暂停");
    }
    else
    {
        timer->stop();
        startFlag = 0;
        btnPrevious->setEnabled(true);
        btnNext->setEnabled(true);
        btnDelete->setEnabled(true);
        btnStart->setText("开始");
    }
}

void ImageCarousel::imagePrevious()
{
    if (deletFlag == 0)
    {
        imageSerialNum--;
        deletFlag = 1;
    }
    if (--imageSerialNum < 0)
        imageSerialNum = fileInfo->count() - 1;
    imageDisplay();
}

void ImageCarousel::imageNext()
{
    if (deletFlag == 0)
    {
        imageSerialNum--;
        deletFlag = 1;
    }
    if (++imageSerialNum == fileInfo->count())
        imageSerialNum = 0;
    imageDisplay();
}

void ImageCarousel::imageDelet()
{
    QFile fileTemp(fileInfo->at(imageSerialNum).filePath());
    fileTemp.remove();
    if (imageSerialNum == fileInfo->count() - 1)
    {
        deletFlag = 1;
        imageSerialNum = 0;
        imageDisplay();
    }
    else
    {
        imageDisplay();
        deletFlag = 0;
    }
    // 设置读取文件的过滤器
    QStringList filter;
    filter << "*.png" << "*.jpg" << "*.bmp"
           << "*.gif" << "*.jpeg";

    // 读取文件
    QDir* dir = new QDir(imageDir);
    dir->setNameFilters(filter);
    fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
}

void ImageCarousel::serialNumPlus()
{
    if (++imageSerialNum >= fileInfo->count())
        imageSerialNum = 0;
}

void ImageCarousel::setMsec()
{
    msec = lEditDelay->text().toInt();
    msec *= 1000;
    imageStartPause();
    imageStartPause();
}
