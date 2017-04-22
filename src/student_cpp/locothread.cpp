#include "locothread.h"

QSemaphore LocoThread::mutex(1);
int LocoThread::reservedBy = -1;
int LocoThread::usedBy = -1;
QSemaphore LocoThread::criticalSegment(1);

LocoThread::LocoThread(const Locomotive& loco,
                       const QVector<int>& parcours,
                       bool priority,
                       QObject *parent) :
    QThread(parent),
    priority(priority),
    parcours(parcours),
    loco(loco)
{
}

LocoThread::LocoThread() {}


void LocoThread::reverse() {
    // inverse le parcours
    std::reverse(parcours.begin(), parcours.end());
    // change le sens de marche
    loco.inverserSens();
}

void LocoThread::demarrer() {
    if(isInterruptionRequested()) {
        loco.arreter();
        exit(0);
    } else {
        loco.demarrer();
    }
}

void LocoThread::run() {

    loco.allumerPhares();
    demarrer();

    // 2 fois dans chaque sens :
    for(int i = 0; i < NB_TOURS; i++) {
        if(i == NB_TOURS/2) {
            attendre_contact(parcours.at(0));
            reverse();
        }

        // attendre le contact de requête
        attendre_contact(parcours.at(0));

        mutex.acquire();
        if(reservedBy < 0 && priority) {
            // si la loco est prioritaire, elle réserve la section critique
            reservedBy = loco.numero();

            if(usedBy < 0) {
                mutex.release();
                // si section critique n'est pas occupée, on acquire()
                criticalSegment.acquire();
            } else {
                mutex.release();
            }
        } else {
            mutex.release();
        }

        // attendre le contact de section critique
        attendre_contact(parcours.at(1));

        // quitte le thread si nécessaire avant le blocage suivant
        if(isInterruptionRequested()) {
            loco.arreter();
            return;
        }

        mutex.acquire();
        if((reservedBy < 0 || reservedBy == loco.numero()) && usedBy < 0) {
            // si le segment n'est pas réservé ni utilisé par une autre loco,
            // on le déclare comme utilisé par nous-même
            usedBy = loco.numero();

            if(reservedBy == loco.numero()) {
               // si on l'avait déjà réservé, on ne l'acquire pas une 2eme fois
                mutex.release();
            } else {
                mutex.release();
                criticalSegment.acquire();
            }
        } else {
            mutex.release();

            // si le segment est utilisé ou réservé par une autre loco,
            // on s'arrête
            loco.arreter();

            // attend la libération de la section critique
            criticalSegment.acquire();

            // déclare la section comme utilisée par nous même
            mutex.acquire();
            usedBy = loco.numero();
            mutex.release();

            // relance la marche
            demarrer();
        }

        // attendre le contact de sortie de section critique
        attendre_contact(parcours.at(2));

        // libère la résevation et l'utilisation de la section critique
        mutex.acquire();
        reservedBy = -1;
        usedBy = -1;
        mutex.release();

        // release la section critique
        criticalSegment.release();
    }

    loco.arreter();
}
