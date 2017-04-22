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

/**
 * @brief Classe LocoThread, contrôle une locomotive
 */
class LocoThread : public QThread
{
    Q_OBJECT
private:

    /**
     * @brief mutex protège l'accès à reservedBy et usedBy
     */
    static QSemaphore mutex;

    /**
     * @brief reservedBy
     * indique le numéro de la loco prioritaire
     * qui a réservé le tronçon critique (-1 sinon)
     */
    static int reservedBy;

    /**
     * @brief usedBy
     * indique le numéro de la loco
     * qui est sur le tronçon critique (-1 sinon)
     */
    static int usedBy;

    /**
     * @brief criticalSegment protège l'accès au tronçon critique
     */
    static QSemaphore criticalSegment;

    /**
     * @brief priority est vrai si la loco peut réserver le tronçon critique,
     * faux sinon
     */
    bool priority;

    /**
     * @brief loco la locomotive contrôlée
     */
    Locomotive loco;

    /**
     * @brief parcours
     * Défini, dans lordre, par
     * le contact de requête,
     * entrée de section critique,
     * sortie de section critique,
     * puis requête (en sens inverse)
     */
    QVector<int> parcours;

    /**
     * @brief run la boucle lancée lors du "start()"
     */
    void run();

    /**
     * @brief reverse
     * change le sens de marche de la loco,
     * ainsi que celui du parcours
     */
    void reverse();

    /**
     * @brief demarrer la locomotive uniquement si l'arret d'urgence
     *        n'est pas enclanché
     */
    void demarrer();

public:
    /**
     * @brief LocoThread constructeur
     * @param loco la locomotive à contrôler
     * @param parcours le parcours à effectuer
     * @param priority détermine si la loco est prioritaire
     * @param parent l'objet QObject qui créé le thread
     */
    LocoThread(const Locomotive& loco,
                        const QVector<int>& parcours,
                        bool priority,
                        QObject *parent = 0);
    /**
     * @brief LocoThread constructeur d'un Locothread non initialisé
     */
    LocoThread();

signals:

public slots:
};

#endif // LOCOTHREAD_H
