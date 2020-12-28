#include "ptzopticspacketparser.h"
#include <QByteArray>

PTZOpticsPacketParser::PTZOpticsPacketParser(int cameraNumber, QObject *parent) : QObject(parent)
{
    m_cameraNumber = cameraNumber;
}

bool PTZOpticsPacketParser::zoomPacket(QByteArray *packet, int zoomSpeed)
{
    char zoomCmd = 0x00;
    if ((packet != 0) && (zoomSpeed < 10) && (zoomSpeed > -10))
    {
        packet->clear();
        packet->append(0x80 + m_cameraNumber);
        packet->append("\x01\x04\x07");
        if (abs(zoomSpeed) == 9)
        {
            if (zoomSpeed > 0)
            {
                zoomCmd = 0x02;
            }
            else
            {
                zoomCmd = 0x03;
            }
        }
        else if (zoomSpeed == 0) {
            zoomCmd = 0x00;
        }
        else
        {
            if (zoomSpeed > 0)
            {
                zoomCmd = (0x20 + zoomSpeed - 1);
            }
            else
            {
                zoomCmd = (0x30 + abs(zoomSpeed) - 1);
            }
        }
        packet->append(zoomCmd);
        packet->append(0xff);
        return true;
    }
    return false;
}

bool PTZOpticsPacketParser::panTiltPacket(QByteArray *packet, int panSpeed, int tiltSpeed)
{
    char panCommand = 0x03;
    char tiltCommand = 0x03;
    if ((packet != 0) && (abs(panSpeed) < 0x1A) && (abs(tiltSpeed)< 0x16))
    {
        if (abs(panSpeed) > 0) {
            if (panSpeed > 0) {
                panCommand = 0x02;
            } else {
                panCommand = 0x01;
            }
            panSpeed = abs(panSpeed)-1;
        }
        if (abs(tiltSpeed) > 0) {
            if (tiltSpeed > 0) {
                tiltCommand = 0x01;
            } else {
                tiltCommand = 0x02;
            }
            tiltSpeed = abs(tiltSpeed)-1;
        }
        packet->clear();
        packet->append(0x80 + m_cameraNumber);
        packet->append("\x01\x06\01");
        packet->append(panSpeed);
        packet->append(tiltSpeed);
        packet->append(panCommand);
        packet->append(tiltCommand);
        packet->append(0xff);
        return true;
    }
    return false;
}

bool PTZOpticsPacketParser::panTiltAbsPacket(QByteArray *packet, unsigned int panSpeed, unsigned int tiltSpeed, int panPosition, int tiltPosition)
{
    if ((packet != 0) && (panSpeed < 0x19) && (tiltSpeed < 0x15))
    {
        packet->clear();
        packet->append(0x80 + m_cameraNumber);
        packet->append("\x01\x06\x02");
        packet->append(panSpeed);
        packet->append(tiltSpeed);
        packet->append((panPosition >> 12) & 0x0f);
        packet->append((panPosition >> 8) & 0x0f);
        packet->append((panPosition >> 4) & 0x0f);
        packet->append((panPosition >> 0) & 0x0f);
        packet->append((tiltPosition >> 12) & 0x0f);
        packet->append((tiltPosition >> 8) & 0x0f);
        packet->append((tiltPosition >> 4) & 0x0f);
        packet->append((tiltPosition >> 0) & 0x0f);
        packet->append(0xff);
        return true;
    }
    return false;
}

bool PTZOpticsPacketParser::memoryPacket(QByteArray *packet, PTZOpticsPacketParser::MemoryType type, int memoryLocation)
{
    if ((packet != 0) && (memoryLocation < 255))
    {
        packet->clear();
        packet->append(0x80 + m_cameraNumber);
        packet->append("\x01\x04\x3f");
        packet->append(type);
        packet->append(memoryLocation);
        packet->append(0xff);
        return true;
    }
    return false;
}

bool PTZOpticsPacketParser::positionRequestPacket(QByteArray *packet, PTZOpticsPacketParser::PositionType type)
{
    if (packet != 0)
    {
        packet->clear();
        packet->append(0x80 + m_cameraNumber);
        packet->append(0x09);
        if ((type == ZOOM) || (type == FOCUS))
        {
            packet->append(0x04);
            if (type == ZOOM)
            {
                packet->append(0x47);
            }
            else if (type == FOCUS)
            {
                packet->append(0x48);
            }
        }
        else
        {
            packet->append("\x06\x12");
        }
        packet->append(0xff);
        return true;
    }
    return false;
}
