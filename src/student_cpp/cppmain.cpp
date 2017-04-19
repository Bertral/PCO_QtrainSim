/*
 * UTILISATION :
 * Selection de la maquette dans cmain().
 * Initialiser la position des aiguillages dans la fonction cmain().
 * Idem pour les locomotives (2 loco max, pas testé avec plus que 2).
 * Idem pour les parcours.
 * Idem pour les priorités.
 * Pour choisir entre programme 1 et 2 (sans ou avec priorité), changer
 * la valeur de la constante "ENABLE_PRIORITY".
 * Pour changer le nombre de tours à effectuer, changer la constante
 * NB_TOURS dans le fichier locothread.h.
 *
 * IMPLEMENTATION :
 * La classe Locomotive n'a pas été modifiée.
 * La classe LocoThread permet de contrôler une locomotive de sorte à ce
 * qu'elle fasse le parcours sans entrer en collision avec une autre loco.
 *
 * Le tronçon critique est protégé par le sémaphore criticalSegment.
 * Pour savoir s'il est libre (tryAcquire est interdit), on utilise un
 * un entier usedBy. Pour savoir s'il est réservé par une loco prioritaire,
 * on utilise l'entier reservedBy.
 * Ces deux variables sont protégées pas le sémaphore "mutex".
 *
 * Résumé du déroulement d'un tour :
 * 1.   Attend le premier contact (celui de requête).
 * 2.   Si la loco est prioritaire, elle réserve la section critique
 *      et si, en plus, la section critique est libre,
 *      elle empêche d'autres loco d'y entrer.
 * 4.   Attend le deuxième contact (début de section critique).
 * 5.   Si la section critique n'est pas utilisée ou réservée, la loco
 *      ne s'arrête pas et empêche d'autres loco d'y entrer (si ce n'était
 *      pas déjà fait).
 *      Sinon, elle s'arrête et attend la libération de la section critique.
 * 6.   Attend le troisième contact (sortie de section critique).
 * 7.   Réinitialise la réservation/utilisation de la section critique.
 * 8.   Libère la section critique, pour autoriser d'autres loco à y passer.
 *
 *
 *
 * TESTS :
 * 1.   vitesse loco1 = 5, vitesse loco2 = 6, priorité activée (pour loco 1) :
 *      La loco 2 s'arrête n'entre pas dans la section critique quand la loco 1
 *      la réserve.
 *
 * 2.   vitesse loco1 = 5, vitesse loco2 = 6, priorité désactivée :
 *      La première loco qui entre en section critique ne s'arrête pas.
 *      La seconde s'arrête à l'entrée de la section critique puis repart
 *      quand elle se libère.
 *
 * 3.   vitesse loco1 = 2, vitesse loco2 = 6, priorité activée,
 *      vitesse fixée à 1 lors de l'entrée en section critique :
 *      La loco2 entre dans la section critique et ralentit. La loco1
 *      (prioritaire) réserve la section critique, mais s'arrête correctement à
 *      l'entrée de la section critique, puis repart quand elle se libère.
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
    thread1.quit();
    thread2.quit();
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
