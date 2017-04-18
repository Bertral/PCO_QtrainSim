#ifndef LOCOTHREAD_H
#define LOCOTHREAD_H

#include <QThread>
#include <QSemaphore>
#include <QPair>
#include <map>
#include <QMutex>
#include <algorithm>
#include "contact.h"
#include "locomotive.h"
#include "ctrain_handler.h"

const int NB_TOURS = 4;

using namespace std;
class LocoThread : public QThread
{
    Q_OBJECT
private:
    static QSemaphore mutex;
    static int reservedBy;
    static int usedBy;
    static QSemaphore criticalSegment;

    bool priority; // 1=first, 0=second
    Locomotive loco;

    /**
     * @brief parcours
     * Défini, dans lordre, par le contact de requête, entrée de section critique,
     * sortie de section critique, puis requête (en sens inverse)
     */
    QVector<int> parcours;

    void run();
    void reverse();

public:
    LocoThread(const Locomotive& loco,
                        const QVector<int>& parcours,
                        bool priority,
                        QObject *parent = 0);
    LocoThread();

signals:

public slots:
};

#endif // LOCOTHREAD_H
