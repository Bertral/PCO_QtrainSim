#include "locothread.h"

QSemaphore LocoThread::mutex(1);
int LocoThread::reservedBy = -1;
int LocoThread::usedBy = -1;
QSemaphore LocoThread::criticalSection(1);

LocoThread::LocoThread(const Locomotive& loco,
                       const QVector<int>& parcours,
//                       const QPair<Contact, Contact>& request,
//                       const QPair<Contact, Contact>& critical,
                       bool priority,
                       QObject *parent) :
    QThread(parent),
    priority(priority),
    parcours(parcours),
    loco(loco)
{
}

void LocoThread::reverse() {
    std::reverse(parcours.begin(), parcours.end());
    loco.inverserSens();
}

void LocoThread::run() {
    loco.allumerPhares();
    loco.demarrer();

    // 2 fois dans chaque sens :
    for(int i = 0; i < NB_TOURS; i++) {
        if(i == NB_TOURS/2) {
            attendre_contact(parcours.at(0));
            reverse();
        }

        // attendre le contact de requête
        attendre_contact(parcours.at(0));

        // si la loco est prioritaire, elle réserve la section critique
        mutex.acquire();
        if(reservedBy < 0 && priority) {
            reservedBy = loco.numero();
            mutex.release();

            criticalSection.acquire();
        } else {
            mutex.release();
        }

        // attendre le contact de section critique
        attendre_contact(parcours.at(1));

        mutex.acquire();
        if(usedBy < 0 && (reservedBy == loco.numero() || reservedBy < 0)) {
            usedBy = loco.numero();

            if(reservedBy == loco.numero()) {
                reservedBy = -1;
                mutex.release();
            } else {
                mutex.release();
                criticalSection.acquire();
            }

        } else {
            mutex.release();

            loco.arreter();
            criticalSection.acquire();

            mutex.acquire();
            usedBy = loco.numero();
            mutex.release();

            loco.demarrer();
        }

        attendre_contact(parcours.at(2));

        mutex.acquire();
        usedBy = -1;
        mutex.release();

        criticalSection.release();

    }

    loco.arreter();
}
