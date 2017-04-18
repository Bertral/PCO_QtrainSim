#include "ctrain_handler.h"
#include "locomotive.h"
#include "locothread.h"

#include <QList>

//Creation d'une locomotive
static Locomotive loco1;
static Locomotive loco2;

//Arret d'urgence
void emergency_stop()
{
    loco1.arreter();
    loco2.arreter();
    afficher_message("\nSTOP!");
}

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QVector<int> parcours1{7, 8, 26, 13};

    QVector<int> parcours2{1, 25, 20, 19};

    //Initialisation des aiguillages
    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, DEVIE, 0);
    diriger_aiguillage(5, DEVIE, 0);
    diriger_aiguillage(6, DEVIE, 0);
    diriger_aiguillage(9, DEVIE, 0);
    diriger_aiguillage(10, DEVIE, 0);
    diriger_aiguillage(13, DEVIE, 0);
    diriger_aiguillage(14, DEVIE, 0);
    diriger_aiguillage(17, DEVIE, 0);

    //Initialisation de la locomotive
    loco1.fixerNumero(1);
    loco1.fixerVitesse(5);
    loco1.fixerPosition(7, 13);
    loco1.afficherMessage("Ready!");

    loco2.fixerNumero(2);
    loco2.fixerVitesse(6);
    loco2.fixerPosition(1, 19);
    loco2.afficherMessage("Ready!");

    //Attente du passage sur les contacts
//    for (int i = 0; i < parcours.size(); i++) {
//        attendre_contact(parcours.at(i));
//        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
//                                    .arg(loco1.numero()).arg(parcours.at(i))));
//        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
//                                    .arg(loco2.numero()).arg(parcours.at(i))));
//        loco1.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
//        loco2.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
//    }

    LocoThread thread1(loco1, parcours1, true);
    LocoThread thread2(loco2, parcours2, false);
    thread1.start();
    thread2.start();


    //Arreter la locomotive
    thread1.wait();
    loco1.afficherMessage("Yeah, piece of cake!");

    thread2.wait();
    loco2.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
