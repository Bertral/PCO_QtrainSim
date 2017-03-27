#ifndef LOCOTHREAD_H
#define LOCOTHREAD_H

#include <QThread>

class LocoThread : public QThread
{
    Q_OBJECT
private:
    void run();
public:
    explicit LocoThread(QObject *parent = 0, int id, int vitesse, int pos1, int pos2);

signals:

public slots:
};

#endif // LOCOTHREAD_H
