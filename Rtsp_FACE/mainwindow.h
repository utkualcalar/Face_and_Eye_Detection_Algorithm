#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ffmpegdecoder_face.h"
#include "opencv2/objdetect.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace cv;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    FFmpegDecoder *decoder;
    void loadcascade();
    CascadeClassifier faceCascade, eyeCascade;
};
#endif // MAINWINDOW_H
