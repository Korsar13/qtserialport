/*
    License...
*/

#ifndef SERIALPORTENGINE_P_UNIX_H
#define SERIALPORTENGINE_P_UNIX_H

#include "serialport.h"
#include "serialportengine_p.h"

#include <termios.h>
//#  undef CMSPAR

QT_BEGIN_NAMESPACE

class QSocketNotifier;

class UnixSerialPortEngine : public QObject, public SerialPortEngine
{
    Q_OBJECT
public:
    UnixSerialPortEngine(SerialPortPrivate *parent);
    virtual ~UnixSerialPortEngine();

    virtual bool nativeOpen(const QString &location, QIODevice::OpenMode mode);
    virtual void nativeClose(const QString &location);

    virtual SerialPort::Lines nativeLines() const;

    virtual bool setNativeDtr(bool set);
    virtual bool setNativeRts(bool set);

    virtual bool nativeFlush();
    virtual bool nativeReset();

    virtual bool nativeSendBreak(int duration);
    virtual bool nativeSetBreak(bool set);

    virtual qint64 nativeBytesAvailable() const;
    virtual qint64 nativeBytesToWrite() const;

    virtual qint64 nativeRead(char *data, qint64 len);
    virtual qint64 nativeWrite(const char *data, qint64 len);
    virtual bool nativeSelect(int timeout,
                              bool checkRead, bool checkWrite,
                              bool *selectForRead, bool *selectForWrite);

    virtual QString nativeToSystemLocation(const QString &port) const;
    virtual QString nativeFromSystemLocation(const QString &location) const;

    virtual bool setNativeRate(qint32 rate, SerialPort::Directions dir);
    virtual bool setNativeDataBits(SerialPort::DataBits dataBits);
    virtual bool setNativeParity(SerialPort::Parity parity);
    virtual bool setNativeStopBits(SerialPort::StopBits stopBits);
    virtual bool setNativeFlowControl(SerialPort::FlowControl flowControl);

    virtual bool setNativeDataInterval(int usecs);
    virtual bool setNativeReadTimeout(int msecs);

    virtual bool setNativeDataErrorPolicy(SerialPort::DataErrorPolicy policy);

    virtual bool isReadNotificationEnabled() const;
    virtual void setReadNotificationEnabled(bool enable);
    virtual bool isWriteNotificationEnabled() const;
    virtual void setWriteNotificationEnabled(bool enable);

protected:
    virtual void detectDefaultSettings();
    virtual bool saveOldsettings();
    virtual bool restoreOldsettings();
    virtual void prepareOtherOptions();

    virtual bool eventFilter(QObject *obj, QEvent *e);

private:
    struct termios m_currTermios;
    struct termios m_oldTermios;
    int m_descriptor;

    QSocketNotifier *m_readNotifier;
    QSocketNotifier *m_writeNotifier;
    QSocketNotifier *m_exceptionNotifier;

    void prepareTimeouts(int msecs);
    bool updateTermious();
    bool setStandartRate(SerialPort::Directions dir, speed_t rate);
    bool setCustomRate(qint32 rate);

    bool isRestrictedAreaSettings(SerialPort::DataBits dataBits,
                                  SerialPort::StopBits stopBits) const;
#if !defined (CMSPAR)
    qint64 writePerChar(const char *data, qint64 maxSize);
    qint64 readPerChar(char *data, qint64 maxSize);
#endif
};

QT_END_NAMESPACE

#endif // SERIALPORTENGINE_P_UNIX_H
