#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaPlayer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "ptzopticspacketparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void clientConnected(void);
    void clientBytesRecieved(qint64 bytes);
    void clientHandleError(QAbstractSocket::SocketError socketError);
private slots:
    void on_sliderTiltControl_sliderMoved(int position);

    void on_sliderPanControl_sliderMoved(int position);

    void on_sliderTiltControl_sliderReleased();

    void on_sliderPanControl_sliderReleased();

    void on_sliderPanSpeed_sliderMoved(int position);

    void on_sliderZoomSpeed_sliderMoved(int position);

    void on_buttonUp_pressed();

    void on_buttonUp_released();

    void on_buttonRight_pressed();

    void on_buttonRight_released();

    void on_buttonDown_pressed();

    void on_buttonDown_released();

    void on_buttonLeft_pressed();

    void on_buttonLeft_released();

    void on_buttonZoomIn_pressed();

    void on_buttonZoomIn_released();

    void on_buttonZoomOut_pressed();

    void on_buttonZoomOut_released();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *m_player;
    QTcpSocket tcpClient;
    PTZOpticsPacketParser *m_parser;
    bool m_connected;
    int m_tiltControl;
    int m_panControl;
    int m_tiltSpeed;
    int m_panSpeed;
    int m_zoomSpeed;

    void zoomCommand(int zoomCommand);
    void panTiltMoveCommand(int panCommand, int tiltCommand);

};
#endif // MAINWINDOW_H
