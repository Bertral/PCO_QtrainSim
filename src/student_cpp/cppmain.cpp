#include "ctrain_handler.h"
#include "locomotive.h"

#include <QList>

//Creation d'une locomotive
static Locomotive locomotive;

//Arret d'urgence
void emergency_stop()
{
    locomotive.arreter();
    afficher_message("\nSTOP!");
}

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    //Initialisation d'un parcours
    QList<int> parcours;
    parcours << 5 << 7 << 6 << 8 << 26 << 10 << 13 << 9;

    //Initialisation des aiguillages
    diriger_aiguillage(1, DEVIE, 0);
    diriger_aiguillage(2, DEVIE, 0);
    diriger_aiguillage(3, DEVIE, 0);
    diriger_aiguillage(4, DEVIE, 0);
    diriger_aiguillage(5, TOUT_DROIT, 0);
    diriger_aiguillage(6, TOUT_DROIT, 0);
    diriger_aiguillage(7, DEVIE, 0);
    diriger_aiguillage(8, DEVIE, 0);
    diriger_aiguillage(9, DEVIE, 0);
    diriger_aiguillage(10, TOUT_DROIT, 0);
    diriger_aiguillage(11, TOUT_DROIT, 0);
    diriger_aiguillage(12, DEVIE, 0);
    diriger_aiguillage(13, DEVIE, 0);
    diriger_aiguillage(14, DEVIE, 0);
    diriger_aiguillage(15, TOUT_DROIT, 0);
    diriger_aiguillage(16, TOUT_DROIT, 0);
    diriger_aiguillage(17, TOUT_DROIT, 0);
//    diriger_aiguillage(18, TOUT_DROIT,  0);
//    diriger_aiguillage(19, TOUT_DROIT,  0);

    //Initialisation de la locomotive
    locomotive.fixerNumero(1);
    locomotive.fixerVitesse(12);
    locomotive.fixerPosition(15, 10);
    locomotive.allumerPhares();
    locomotive.demarrer();
    locomotive.afficherMessage("Ready!");

    //Attente du passage sur les contacts
    for (int i = 1; i < parcours.size(); i++) {
        attendre_contact(parcours.at(i));
        afficher_message(qPrintable(QString("The engine no. %1 has reached contact no. %2.")
                                    .arg(locomotive.numero()).arg(parcours.at(i))));
        locomotive.afficherMessage(QString("I've reached contact no. %1.").arg(parcours.at(i)));
    }

    //Arreter la locomotive
    locomotive.arreter();
    locomotive.afficherMessage("Yeah, piece of cake!");

    //Fin de la simulation
    mettre_maquette_hors_service();

    //Exemple de commande
    afficher_message("Enter a command in the input field at the top of the window.");
    QString commande = getCommand();
    afficher_message(qPrintable(QString("Your command is: ") + commande));

    return EXIT_SUCCESS;
}
