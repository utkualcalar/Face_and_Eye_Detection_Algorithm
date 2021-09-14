#include "login_face.h"
#include "ui_login_face.h"
#include <qstring.h>
#include <stdio.h>
#include <QString>
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btn_play_clicked()
{
    close();
}

QString Login::getRtspURL()
{
    return ui->lineEdit->text();;
}

void Login::on_btn_cancel_clicked()
{
    exit(0);
}
