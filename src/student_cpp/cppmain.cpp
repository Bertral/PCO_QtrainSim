/*
 * Conditions de test :
 * 1. vitesse loco1 = 5, vitesse loco2 = 6, priorité activée (pour loco 1)
 * 2. vitesse loco1 = 5, vitesse loco2 = 6, priorité désactivée
 * 3. vitesse loco1 = 2, vitesse loco2 = 6, priorité activée, vitesse fixée à 1 lors
 *    de l'entrée en section critique.
 *
 *
 *
 *
 */


#include "ctrain_handler.h"
#include "locomotive.h"
#include "locothread.h"

#include <QList>

const bool ENABLE_PRIORITY = true;

//Creation d'une locomotive
static Locomotive loco1;
static Locomotive loco2;
static LocoThread thread1;
static LocoThread thread2;

//Arret d'urgence
void emergency_stop()
{
    loco1.arreter();
    loco2.arreter();
    thread1.quit();
    thread2.quit();
    afficher_message("\nSTOP!");
}

//Fonction principale
int cmain()
{
    afficher_message("Hit play to start the simulation...");

    //Choix de la maquette
    selection_maquette(MAQUETTE_B);

    /* IMPORTANT :
     * Les vecteurs de parcours sont composés de 4 éléments comme suit :
     * 0 : capteur de requête
     * 1 : capteur de début de section critique
     * 2 : capteur de fin de section critique
     * 3 : capteur de requête lors du parcours en sens inverse
     */
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

    //Initialisation des locomotives
    loco1.fixerNumero(1);
    loco1.fixerVitesse(5);
    loco1.fixerPosition(7, 13);
    loco1.afficherMessage("Ready!");

    loco2.fixerNumero(2);
    loco2.fixerVitesse(6);
    loco2.fixerPosition(1, 19);
    loco2.afficherMessage("Ready!");

    LocoThread thread1(loco1, parcours1, ENABLE_PRIORITY);
    LocoThread thread2(loco2, parcours2, false);

    // lancement des threads
    thread1.start();
    thread2.start();


    //Arreter les locomotive
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
