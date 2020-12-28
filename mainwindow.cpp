#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QAbstractSocket>
#include <QTcpSocket>
#include "ptzopticspacketparser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_connected = false;
    m_tiltControl = 0;
    m_panControl = 0;
    m_tiltSpeed = 0;
    m_panSpeed = 0;
    m_zoomSpeed = 0;
    ui->setupUi(this);
    m_parser = new PTZOpticsPacketParser(1);
    connect(&tcpClient, &QAbstractSocket::connected, this, &MainWindow::clientConnected);
    tcpClient.connectToHost(QHostAddress("192.168.2.247"),5678);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::panTiltMoveCommand(int panCommand, int tiltCommand)
{
    QByteArray packet;
    m_parser->panTiltPacket(&packet, panCommand, tiltCommand);
    if (m_connected) {
        tcpClient.write(packet);
    }
    qDebug() << "Pan/Tilt Command: " << panCommand << "/" << tiltCommand;
}

void MainWindow::zoomCommand(int zoomCommand)
{
    QByteArray packet;
    m_parser->zoomPacket(&packet,zoomCommand);
    if (m_connected) {
        tcpClient.write(packet);
    }
}

void MainWindow::clientConnected()
{
    // Show something
    qDebug() << "Connected";
    m_connected = true;
}

void MainWindow::clientBytesRecieved(qint64 bytes)
{
    // Parse data
    QByteArray packet = tcpClient.readAll();
    // Show for now
    qDebug() << packet;
}

void MainWindow::clientHandleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Client socket error: " << socketError;
    m_connected = false;
    return;
}


void MainWindow::on_sliderTiltControl_sliderMoved(int position)
{
    m_tiltControl = position;
    panTiltMoveCommand(m_panControl,m_tiltControl);
}

void MainWindow::on_sliderPanControl_sliderMoved(int position)
{
    m_panControl = position;
    panTiltMoveCommand(m_panControl,m_tiltControl);
}

void MainWindow::on_sliderTiltControl_sliderReleased()
{
    ui->sliderTiltControl->setValue(0);
    m_tiltControl = 0;
    panTiltMoveCommand(m_panControl,m_tiltControl);
}

void MainWindow::on_sliderPanControl_sliderReleased()
{
    ui->sliderPanControl->setValue(0);
    m_panControl = 0;
    panTiltMoveCommand(m_panControl,m_tiltControl);
}

void MainWindow::on_sliderPanSpeed_sliderMoved(int position)
{
    m_panSpeed = position;
    m_tiltSpeed = position;
}



void MainWindow::on_sliderZoomSpeed_sliderMoved(int position)
{
    m_zoomSpeed = position;
}

void MainWindow::on_buttonUp_pressed()
{
    panTiltMoveCommand(0,m_tiltSpeed + 1);
}

void MainWindow::on_buttonUp_released()
{
    panTiltMoveCommand(0,0);
}

void MainWindow::on_buttonRight_pressed()
{
    panTiltMoveCommand(m_panSpeed + 1,0);
}

void MainWindow::on_buttonRight_released()
{
    panTiltMoveCommand(0,0);
}

void MainWindow::on_buttonDown_pressed()
{
    panTiltMoveCommand(0, -1 - m_tiltSpeed);
}

void MainWindow::on_buttonDown_released()
{
    panTiltMoveCommand(0,0);
}

void MainWindow::on_buttonLeft_pressed()
{
    panTiltMoveCommand(-1 - m_panSpeed, 0);
}

void MainWindow::on_buttonLeft_released()
{
    panTiltMoveCommand(0,0);
}

void MainWindow::on_buttonZoomIn_pressed()
{
    zoomCommand(m_zoomSpeed+1);
}

void MainWindow::on_buttonZoomIn_released()
{
    zoomCommand(0);
}

void MainWindow::on_buttonZoomOut_pressed()
{
    zoomCommand(-1 - m_zoomSpeed);
}

void MainWindow::on_buttonZoomOut_released()
{
    zoomCommand(0);
}

