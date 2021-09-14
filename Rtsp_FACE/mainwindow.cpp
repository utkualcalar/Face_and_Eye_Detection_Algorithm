//Yasar Utku Alcalar
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ffmpegdecoder_face.h"
#include "login_face.h"
#include <unistd.h>
#include <QPainter>
#include <QInputDialog>
#include <opencv2/opencv.hpp>
#include <QWidget>
#include <iostream>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <QElapsedTimer>
#include <QDebug>


using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Login l;
    l.exec();
    decoder = new FFmpegDecoder(l.getRtspURL().toStdString());
    decoder->connect();
    if(decoder->isConnected()) {
        new std::thread(&FFmpegDecoder::decode, decoder);
        loadcascade();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *)
{   cv::Mat src;
    static QElapsedTimer t;
    //t.start();

    if(decoder->isConnected())
    {
        decoder->mtx.lock();
        if(!decoder->decodedImgBuf.empty())
        {
            src = decoder->decodedImgBuf.front().clone();
            decoder->decodedImgBuf.pop_front();
        }
        decoder->mtx.unlock();

        if(!src.empty())
        {
            double scale_2 = 4;
            Mat crop;

            vector<Rect> faces, eyes;
            cv::Rect crp;
            faceCascade.detectMultiScale(src, faces, 1.9, 6);

            for(size_t i = 0; i < faces.size(); i++)
            {
                rectangle(src, faces[i].tl(), faces[i].br(), Scalar(0,255,0), 3);
                crp.x = faces[i].x;
                crp.y = faces[i].y;
                crp.width = (faces[i].width);
                crp.height = (faces[i].height);
                crop = src(crp);

                if (crop.cols * scale_2 < src.cols && crop.rows * scale_2 < src.rows)
                {
                    cv::resize(crop, crop, Size(crop.cols * scale_2, crop.rows * scale_2), 0, 0, INTER_LINEAR);
                    eyeCascade.detectMultiScale(crop, eyes, 1.1, 15);

                    for(size_t j = 0; j < eyes.size(); j++)
                    {
                        rectangle(src, crp.tl() + eyes[j].tl()/scale_2, crp.tl() + eyes[j].br()/scale_2, Scalar(0,0,255), 3);
                    }
                }

                else
                {
                    eyeCascade.detectMultiScale(crop, eyes, 1.1, 15);

                    for(size_t j = 0; j < eyes.size(); j++)
                    {
                        rectangle(src, crp.tl() + eyes[j].tl(), crp.tl() + eyes[j].br(), Scalar(0,0,255), 3);
                    }
                }
            }


            QPainter painter(this);
            QImage image = QImage(src.data, src.cols, src.rows, QImage::Format_RGB888);
            QPixmap  pix =  QPixmap::fromImage(image);
            painter.drawPixmap(0, 0, ui->centralwidget->width(), ui->centralwidget->height(), pix);
            this->setWindowTitle("Face and Eye Detection Application");
            //         qDebug() << t.elapsed();

            if (t.elapsed())
                painter.drawText(10, 10, QString("FPS: %1").arg(1000/t.restart()));
            update();
        } else {
            update();
            usleep(100000);
        }
        src.release();
    } else {
        update();
        usleep(100000);
    }
}



void MainWindow::loadcascade()
{
    // PreDefined trained XML classifiers with facial features
    faceCascade.load("/home/enes/Rtsp_FACE/haarcascades/haarcascade_frontalface_default.xml");
    eyeCascade.load("/home/enes/Rtsp_FACE/haarcascades/haarcascade_eye.xml");
}




















//cvtColor( src, src, COLOR_RGB2BGR); imshow( "src", src ); waitKey(0);}}


//            cvtColor( crop, crop, COLOR_RGB2BGR); imshow( "Cropped", crop ); waitKey(0);}}

//    smileCascade.load("/home/enes/Rtsp_FACE/haarcascades/haarcascade_smile.xml");

//    cvtColor( src, src, COLOR_RGB2BGR); imshow( "Face Detection", src ); waitKey(0);}}


/*      vector<Rect> faces, eyes;
      faceCascade.detectMultiScale(src, faces, 1.3, 5);

      for(int i = 0; i < faces.size(); i++)
      {
          rectangle(src, faces[i].tl(), faces[i].br(), Scalar(255,0,255), 3);
      }

      eyeCascade.detectMultiScale(src, eyes, 1.1, 22);

      for(int i = 0; i < eyes.size(); i++)
      {
          rectangle(src, eyes[i].tl(), eyes[i].br(), Scalar(255,255,255), 3);
      }

        */


//                  nestedCascade.detectMultiScale( smallImgROI, nestedObjects, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );


//                   nestedCascade.detectMultiScale(src, nestedObjects, 1.1, 22);

//                   for(int i = 0; i < nestedObjects.size(); i++)
//                   {
//                       rectangle(src, nestedObjects[i].tl(), nestedObjects[i].br(), Scalar(255,0,255), 3);
//                   }}



//qDebug()<<__LINE__;
//  cvtColor( src, src, COLOR_RGB2BGR); imshow( "Face Detection", src ); waitKey(1);}update();




//------------------------------------------------------------------------------------------------
//            vector<Rect> faces;
//            Mat gray, smallImg ;

//               cvtColor( src, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
//               double fx = 1 / scale;#include <QDebug>

//               // Resize the Grayscale Image
//               cv::resize( gray, smallImg, cv::Size(), fx, fx, INTER_LINEAR );
//               equalizeHist( smallImg, smallImg );

//               // Detect faces of different sizes using cascade classifier
//               cascade.detectMultiScale( smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE);

//               // Draw circles around the faces
//               for ( size_t i = 0; i < faces.size(); i++ )
//               {   //qDebug()<<__LINE__;
//                   Rect r = faces[i];
//                   Mat smallImgROI;
//                   vector<Rect> nestedObjects;
//                   Point center;
//                   Scalar color = Scalar(255, 0, 0); // Color for Drawing tool
//                   int radius;

//                   double aspect_ratio = (double)r.width/r.height;
//                   if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
//                   {
//                       center.x = cvRound((r.x + r.width*0.5)*scale);
//                       center.y = cvRound((r.y + r.height*0.5)*scale);
//                       radius = cvRound((r.width + r.height)*0.25*scale);
//                       circle( src, center, radius, color, 3, 8, 0 );
//                   }
//                   else
//                       rectangle(src, cv::Point(round(r.x*scale), round(r.y*scale)),
//                       cv::Point(round((r.x + r.width-1)*scale),
//                       round((r.y + r.height-1)*scale)), color, 3, 8, 0);
//                   if( nestedCascade.empty() )
//                       continue;
//                   smallImgROI = smallImg( r );

//                  // Detection of eyes int the input image

//                    nestedCascade.detectMultiScale(smallImg, nestedObjects, 1.2, 22);

//                    // Draw circles around eyes
//                   for ( size_t j = 0; j < nestedObjects.size(); j++ )
//                   {
//                       Rect nr = nestedObjects[j];
//                       center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
//                       center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
//                       radius = cvRound((nr.width + nr.height)*0.25*scale);
//                       circle( src, center, radius, color, 3, 8, 0 );
//                   }
//               }



//-------------------------------------------------------------------------------------------------
//    cascade.load( "/home/enes/Rtsp_FACE/haarcascades/haarcascade_frontalface_default.xml" );
//    nestedCascade.load( "/home/enes/Rtsp_FACE/haarcascades/haarcascade_eye.xml" );




//------------------------------------------------------------------------------------------
//CascadeClassifier cascade, nestedCascade;
//double scale = 15;


//-----------------------------------------------------------------------------------------------

//cv::Size deltaSize( faces[i].width * 0.3f, faces[i].height * 0.3f ); // 0.1f = 10/100
//cv::Point offset( deltaSize.width/2, deltaSize.height/2);
//faces[i] += deltaSize;
//faces[i] -= offset;
//rectangle(src, faces[i].tl()*scale, faces[i].br()*scale, Scalar(255,0,0), 3);

//------------------------------------------------------------------------------------------------

//Point pt1(faces[i].x, faces[i].y);
//Point pt2((faces[i].x + faces[i].height), (faces[i].y + faces[i].width));
//rectangle(src, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);


//--------------------------------------------------------------------------------------------------

//cvtColor(crop, crop, COLOR_BGR2GRAY); // Convert cropped image to Grayscale


//-------------------------------------------------------------------------------------------------

//                {
//                    cv::resize(crop, crop, Size(src.cols, src.rows), 0, 0, INTER_LINEAR);
//                }

//------------------------------------------------------------------------------------------

//            double scale = 3;
//            Mat gray, smallImg;
//            cvtColor( src, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
//            double fx = 1 / scale;

//            // Resize the Grayscale Image
//            cv::resize( gray, smallImg, cv::Size(), fx, fx, INTER_NEAREST );
//            equalizeHist( smallImg, smallImg );
