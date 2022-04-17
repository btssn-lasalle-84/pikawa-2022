#include "cafetiere.h"
#include "ihm.h"
#include "communication.h"
#include "preparation.h"
#include "basededonnees.h"
#include <QDebug>

/**
 * @file cafetiere.cpp
 *
 * @brief Définitionde la classe Cafetiere
 * @author
 * @version 0.2
 *
 */

Cafetiere::Cafetiere(IHMPikawa* ihm) :
    QObject(ihm), ihm(ihm), communication(new Communication(this)),
    preparation(new Preparation(this)), nomCapsules(0), nomLongueurs(0),

    capsuleChoisie(0), longueurChoisie(0), niveauEau(0), niveauEauNecessaire(0), connectee(false),
    activee(false), capsulePresente(false), tassePresente(false)
{
    qDebug() << Q_FUNC_INFO;
    baseDeDonneesPikawa = BaseDeDonnees::getInstance();
    baseDeDonneesPikawa->ouvrir(NOM_BDD);

    connect(communication,
            SIGNAL(cafetiereDetectee(QString, QString)),
            this,
            SIGNAL(cafetiereDetectee(QString, QString)));

    connect(communication,
            SIGNAL(cafetiereConnectee(QString, QString)),
            this,
            SIGNAL(cafetiereConnectee(QString, QString)));

    connect(communication,
            SIGNAL(cafetiereDeconnectee()),
            this,
            SIGNAL(cafetiereDeconnectee()));

    connect(communication,
            SIGNAL(rechercheTerminee(bool)),
            this,
            SIGNAL(rechercheTerminee(bool)));

    connect(communication,
            SIGNAL(etatMagasin(bool, bool, bool, bool, bool, bool, bool, bool)),
            this,
            SLOT(mettreAJourMagasin(bool, bool, bool, bool, bool, bool, bool, bool)));


    /**
     * @todo Gérer l'utilisateur connecté (identifiant ou badge) à cette
     * cafetière
     */
    chargerPreferences(IDENTIFIANT_UTILISATEUR);
    initialiserNomCapsules();
    initiatiserNomLongueurs();
}

Cafetiere::~Cafetiere()
{
    BaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO;
}

void Cafetiere::initialiserNomCapsules()
{
    this->nomCapsules = preparation->getNomCapsules();
}

void Cafetiere::initiatiserNomLongueurs()
{
    this->nomLongueurs = preparation->getNomLongueurs();
}

void Cafetiere::chargerPreferences(QString identifiantUtilisateur)
{
    qDebug() << Q_FUNC_INFO << identifiantUtilisateur;
    QString requete =
      "SELECT Preferences.idPreferences, Utilisateur.idUtilisateur, "
      "Utilisateur.nom, Utilisateur.prenom, Capsule.idCapsule, "
      "Capsule.designation, Capsule.libelle, TypeBoisson.idTypeBoisson, "
      "TypeBoisson.type FROM Preferences INNER JOIN Utilisateur ON "
      "Utilisateur.idUtilisateur=Preferences.idUtilisateur INNER JOIN Capsule "
      "ON Capsule.idCapsule=Preferences.capsuleActuelle INNER JOIN TypeBoisson "
      "ON TypeBoisson.idTypeBoisson=Preferences.typeBoissonActuelle WHERE "
      "Utilisateur.identifiant='" +
      identifiantUtilisateur + "';";
    baseDeDonneesPikawa->recuperer(requete, preferences);
    qDebug() << Q_FUNC_INFO << preferences;
}

QStringList Cafetiere::getNomcapsules() const
{
    return nomCapsules;
}

QStringList Cafetiere::getNomLongueurs() const
{
    return nomLongueurs;
}

int Cafetiere::getCaspuleChoisie() const
{
    return capsuleChoisie;
}

int Cafetiere::getLongueurChoisie() const
{
    return longueurChoisie;
}

int Cafetiere::getNiveauEau() const
{
    return niveauEau;
}

int Cafetiere::getniveauEauNecessaire() const
{
    return niveauEauNecessaire;
}

bool Cafetiere::getConnectee() const
{
    return connectee;
}

bool Cafetiere::getActivee() const
{
    return activee;
}

bool Cafetiere::getCapsulePresente() const
{
    return capsulePresente;
}

bool Cafetiere::getTassePresente() const
{
    return tassePresente;
}

int Cafetiere::getIdCapsule(QString nomCapsule) const
{
    if(nomCapsules.isEmpty() || nomCapsule.isEmpty())
        return -1;
    return nomCapsules.indexOf(nomCapsule);
}

QStringList Cafetiere::getPreferences() const
{
    return preferences;
}

QString Cafetiere::getCapsulePreferee() const
{
    if(!preferences.isEmpty())
    {
        qDebug() << Q_FUNC_INFO
                 << preferences.at(Cafetiere::ChampsTablePreferences::
                                     COLONNE_PREFERENCES_DESIGNATION_CAPSULE);
        return preferences.at(Cafetiere::ChampsTablePreferences::
                                COLONNE_PREFERENCES_DESIGNATION_CAPSULE);
    }
    return QString();
}

QString Cafetiere::getLongueurPreferee() const
{
    if(!preferences.isEmpty())
    {
        qDebug() << Q_FUNC_INFO
                 << preferences.at(Cafetiere::ChampsTablePreferences::
                                     COLONNE_PREFERENCES_TYPE_BOISSON);
        return preferences.at(
          Cafetiere::ChampsTablePreferences::COLONNE_PREFERENCES_TYPE_BOISSON);
    }
    return QString();
}

void Cafetiere::setCapsuleChoisie(const int& capsuleChoisie)
{
    if(this->capsuleChoisie != capsuleChoisie)
    {
        this->capsuleChoisie = capsuleChoisie;
        QString requete      = "UPDATE Preferences SET capsuleActuelle='" +
                          QString::number(capsuleChoisie + 1) +
                          "' WHERE Preferences.idUtilisateur='" +
                          IDENTIFIANT_UTILISATEUR_ID + "'";
        baseDeDonneesPikawa->executer(requete);
    }
}

void Cafetiere::setLongueurChoisie(const int& longueurChoisie)
{
    this->longueurChoisie = longueurChoisie;
    if(this->longueurChoisie != capsuleChoisie)
    {
        this->longueurChoisie = longueurChoisie;
        QString requete       = "UPDATE Preferences SET typeBoissonActuelle='" +
                          QString::number(longueurChoisie + 1) +
                          "' WHERE Preferences.idUtilisateur='" +
                          IDENTIFIANT_UTILISATEUR_ID + "'";
        baseDeDonneesPikawa->executer(requete);
    }
}

void Cafetiere::setNiveauEau(const int& niveauEau)
{
    this->niveauEau = niveauEau;
}

void Cafetiere::setNiveauEauNecessaire(const int& niveauEauNecessaire)
{
    this->niveauEauNecessaire = niveauEauNecessaire;
}

void Cafetiere::demarrerDecouverte()
{
    qDebug() << Q_FUNC_INFO;
    communication->activerLaDecouverte();
}

void Cafetiere::arreterDecouverte()
{
    qDebug() << Q_FUNC_INFO;
    communication->desactiverLaDecouverte();
}

void Cafetiere::rafraichirDecouverte()
{
    qDebug() << Q_FUNC_INFO;
    communication->desactiverLaDecouverte();
    communication->activerLaDecouverte();
}

void Cafetiere::gererConnexion()
{
    qDebug() << Q_FUNC_INFO;
    if(communication->estConnecte())
        communication->deconnecter();
    else
        communication->connecter();
}

bool Cafetiere::estPret()
{
    if(preparation->estPreparationPrete() && communication->estConnecte())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Cafetiere::mettreAJourMagasin(QString colombiaPresent, QString indonesiaPresent, QString ethiopiaPresent,
                            QString volutoPresent, QString capriccioPresent, QString cosiPresent, QString scuroPresent,
                            QString vanillaPresent)
{
    QString requette = "UPDATE Capsule SET quantite = " + colombiaPresent + "WHERE rangee = 1";
    QString requette = "UPDATE Capsule SET quantite = " + indonesiaPresent + "WHERE rangee = 2";
    QString requette = "UPDATE Capsule SET quantite = " + ethiopiaPresent + "WHERE rangee = 3";
    QString requette = "UPDATE Capsule SET quantite = " + volutoPresent + "WHERE rangee = 4";
    QString requette = "UPDATE Capsule SET quantite = " + capriccioPresent + "WHERE rangee = 5";
    QString requette = "UPDATE Capsule SET quantite = " + cosiPresent + "WHERE rangee = 6";
    QString requette = "UPDATE Capsule SET quantite = " + scuroPresent + "WHERE rangee = 7";
    QString requette = "UPDATE Capsule SET quantite = " + vanillaPresent + "WHERE rangee = 8";

    baseDeDonneesPikawa->executer(requette);
}


