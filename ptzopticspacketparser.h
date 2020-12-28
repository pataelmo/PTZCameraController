#ifndef PTZOPTICSPACKETPARSER_H
#define PTZOPTICSPACKETPARSER_H

#include <QObject>
#include <QByteArray>

class PTZOpticsPacketParser : public QObject
{
    Q_OBJECT
public:
    enum PanTiltDir {
        UP,DOWN,LEFT,RIGHT,UPLEFT,UPRIGHT,DOWNLEFT,DOWNRIGHT,STOP
    };
    enum MemoryType {
        RESET = 0, SET = 1, RECALL = 2
    };
    enum PositionType {
        ZOOM,FOCUS,PANTILT
    };

    explicit PTZOpticsPacketParser(int cameraNumber, QObject *parent = nullptr);
    bool zoomPacket(QByteArray* packet, int zoomSpeed);
    bool panTiltPacket(QByteArray* packet, int panSpeed, int tiltSpeed);
    bool panTiltAbsPacket(QByteArray* packet, unsigned int panSpeed, unsigned int tiltSpeed, int panPosition, int tiltPosition);
    bool memoryPacket(QByteArray* packet, MemoryType type, int memoryLocation);
    bool positionRequestPacket(QByteArray* packet, PositionType type);




signals:

private:
    int m_cameraNumber;
};

#endif // PTZOPTICSPACKETPARSER_H
