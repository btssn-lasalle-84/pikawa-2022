#include "ihm.h"
#include "ui_ihm.h"
#include "basededonnees.h"
#include "cafetiere.h"
#include <QDebug>
#include <QIcon>

/**
 * @file ihm.cpp
 *
 * @brief Définition de la classe IHMPikawa
 * @author
 * @version 0.2
 *
 */

/**
 * @brief Constructeur de la classe IHMPikawa
 *
 * @fn IHMPikawa::IHMPikawa
 * @param parent L'adresse de l'objet parent, si nullptr IHMPikawa sera la
 * fenêtre principale de l'application
 */
IHMPikawa::IHMPikawa(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::IHMPikawa), iconeBoutonConnecte(nullptr),
    iconeBoutonDetectee(nullptr), iconeBoutonDeconnecte(nullptr)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;

    ouvrirBaseDeDonnees();

    cafetiere = new Cafetiere(this);

    gererEvenements();

    initialiserIHM();

#ifdef PLEIN_ECRAN
    showFullScreen();
// showMaximized();
#endif
}

/**
 * @brief Destructeur de la classe IHMPikawa
 *
 * @fn IHMPikawa::~IHMPikawa
 * @details Libère les ressources de l'application
 */
IHMPikawa::~IHMPikawa()
{
    delete ui;
    delete iconeBoutonDetectee;
    delete iconeBoutonConnecte;
    delete iconeBoutonDeconnecte;
    BaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO;
}

void IHMPikawa::afficherPage(IHMPikawa::Page page)
{
    // qDebug() << Q_FUNC_INFO << "page" << page;
    ui->ecrans->setCurrentIndex(page);
}

void IHMPikawa::afficherPageAcceuil()
{
    afficherPage(IHMPikawa::Page::Accueil);
}

void IHMPikawa::afficherPageInformations()
{
    afficherPage(IHMPikawa::Page::Information);
}

void IHMPikawa::afficherPageEntretien()
{
    afficherPage(IHMPikawa::Page::Entretien);
}

void IHMPikawa::afficherPageParametres()
{
    afficherPage(IHMPikawa::Page::Parametres);
}

void IHMPikawa::afficherPageSelectionCafe()
{
    afficherPage(IHMPikawa::Page::SelectionCafe);
}

void IHMPikawa::activerBoutonConnexionEtatDetecte(QString nom, QString adresse)
{
    qDebug() << Q_FUNC_INFO << nom << adresse;
    // si une cafetère pikawa a été détectée
    ui->boutonConnexion->setEnabled(true);
    ui->boutonConnexion->setIcon(*iconeBoutonDetectee);
    ui->labelEtatConnexion->setText("Cafetière détectée");
}

/**
 * @todo revoir la gestion de la connexion/déconnexion
 */
void IHMPikawa::activerBoutonConnexionEtatConnecte(QString nom, QString adresse)
{
    qDebug() << Q_FUNC_INFO << nom << adresse;
    // si une cafetère pikawa a été connectée
    ui->boutonConnexion->setEnabled(true);
    ui->boutonConnexion->setIcon(*iconeBoutonConnecte);
    ui->labelEtatConnexion->setText("Cafetière connectée");
    ui->boutonRafraichir->setEnabled(false);
}

void IHMPikawa::activerBoutonConnexionEtatDeconnecte()
{
    qDebug() << Q_FUNC_INFO;
    // si une cafetère pikawa a été déconnectée
    ui->boutonConnexion->setEnabled(true);
    ui->boutonConnexion->setIcon(*iconeBoutonDeconnecte);
    ui->labelEtatConnexion->setText("Cafetière déconnectée");
    ui->boutonRafraichir->setEnabled(true);
}

void IHMPikawa::rafraichirDecouverte()
{
    qDebug() << Q_FUNC_INFO;
    ui->boutonRafraichir->setEnabled(false);
    cafetiere->demarrerDecouverte();
}

void IHMPikawa::terminerDecouverte(bool detecte)
{
    qDebug() << Q_FUNC_INFO << "detecte" << detecte;
    ui->boutonRafraichir->setEnabled(true);
}

void IHMPikawa::gererLongueurPreparation(int longueurPreparation)
{
    qDebug() << Q_FUNC_INFO << "longueurPreparation" << longueurPreparation;
    QStringList labelsLongueurPreparation = cafetiere->getNomLongueurs();
    ui->labelLongueurPreparation->setText(
      labelsLongueurPreparation.at(longueurPreparation));
    cafetiere->setLongueurChoisie(longueurPreparation);
}

void IHMPikawa::gererSelectionCafes()
{
    connect(ui->boutonColombia,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerColombia()));
    connect(ui->boutonIndonesia,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerIndonesia()));
    connect(ui->boutonEthiopia,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerEthiopia()));
    connect(ui->boutonVolluto,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerVolluto()));
    connect(ui->boutonCosi, SIGNAL(clicked()), this, SLOT(selectionnerCosi()));
    connect(ui->boutonScuro,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerScuro()));
    connect(ui->boutonVanilla,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerVanilla()));
    connect(ui->boutonCapriccio,
            SIGNAL(clicked()),
            this,
            SLOT(selectionnerCapriccio()));
    connect(cafetiere, SIGNAL(cafePret()), this, SLOT(afficherCafePret()));
    connect(cafetiere,
            SIGNAL(cafeEnCours()),
            this,
            SLOT(afficherCafeEnCours()));
    connect(cafetiere,
            SIGNAL(erreurPreparation()),
            this,
            SLOT(afficherErreurPreparation()));
}

void IHMPikawa::selectionnerColombia()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleColombia);
    ui->capsuleChoisie->setText("Colombia");
    int idCapsule = cafetiere->getIdCapsule("Colombia");
    qDebug() << Q_FUNC_INFO << "idCapsule Colombia" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerIndonesia()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleIndonesia);
    ui->capsuleChoisie->setText("Indonesia");
    int idCapsule = cafetiere->getIdCapsule("Indonesia");
    qDebug() << Q_FUNC_INFO << "idCapsule Indonesia" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerEthiopia()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleEthiopia);
    ui->capsuleChoisie->setText("Ethiopia");
    int idCapsule = cafetiere->getIdCapsule("Ethiopia");
    qDebug() << Q_FUNC_INFO << "idCapsule Ethiopia" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerVolluto()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleVolluto);
    ui->capsuleChoisie->setText("Volluto");
    int idCapsule = cafetiere->getIdCapsule("Volluto");
    qDebug() << Q_FUNC_INFO << "idCapsule Volluto" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerCosi()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleCosi);
    ui->capsuleChoisie->setText("Cosi");
    int idCapsule = cafetiere->getIdCapsule("Cosi");
    qDebug() << Q_FUNC_INFO << "idCapsule Cosi" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerScuro()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleScuro);
    ui->capsuleChoisie->setText("Scuro");
    int idCapsule = cafetiere->getIdCapsule("Scuro");
    qDebug() << Q_FUNC_INFO << "idCapsule Scuro" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerVanilla()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleVanilla);
    ui->capsuleChoisie->setText("Vanilla");
    int idCapsule = cafetiere->getIdCapsule("Vanilla");
    qDebug() << Q_FUNC_INFO << "idCapsule Vanilla" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::selectionnerCapriccio()
{
    ui->bontonChangerCafe->setIcon(*iconeCapsuleCapriccio);
    ui->capsuleChoisie->setText("Capriccio");
    int idCapsule = cafetiere->getIdCapsule("Capriccio");
    qDebug() << Q_FUNC_INFO << "idCapsule Capriccio" << idCapsule;
    cafetiere->setCapsuleChoisie(idCapsule);
    afficherPageAcceuil();
}

void IHMPikawa::afficherCafePret()
{
    qDebug() << Q_FUNC_INFO;
    afficherMessage("Café prêt", "green");
    cafetiere->estPrete();
}

void IHMPikawa::afficherCafeEnCours()
{
    qDebug() << Q_FUNC_INFO;
    afficherMessage("Café en cours", "orange");
}

void IHMPikawa::afficherErreurPreparation()
{
    qDebug() << Q_FUNC_INFO;
    afficherMessage("Préparation impossible !", "red");
}

void IHMPikawa::mettreAJourEtatCafetiere(int  reservoirEau,
                                         bool bacCapsules,
                                         bool etatCapsule,
                                         bool etatTasse)
{
    int reservoirEauPourcentage = convertirPourcentageEau(reservoirEau);
    ui->niveauEau->setValue(reservoirEauPourcentage);

    if(!bacCapsules)
    {
        ui->etatBac->setPixmap(*iconeBacPlein);
        ui->labelBac->setStyleSheet("font-size: 25px; color: red;");
    }
    else
    {
        ui->etatBac->setPixmap(*iconeBacVide);
        ui->labelBac->setStyleSheet("font-size: 25px; color: black;");
    }

    afficherAvertissement(reservoirEau, bacCapsules, etatCapsule, etatTasse);
}

void IHMPikawa::mettreAJourMagasinIHM(QStringList caspulesDisponibles)
{
    /**
     * @todo Et la capsule choisie ?
     */
    for(int i = 0; i < caspulesDisponibles.size(); ++i)
    {
        if(caspulesDisponibles.at(i) == "1")
        {
            boutonsCafes.at(i)->setEnabled(true);
        }
        else
        {
            boutonsCafes.at(i)->setEnabled(false);
        }
    }
}

void IHMPikawa::afficherCafetierePrete()
{
    ui->boutonLancerPreparation->setEnabled(true);
}

void IHMPikawa::afficherCafetierePasPrete()
{
    ui->boutonLancerPreparation->setEnabled(false);
}

void IHMPikawa::initialiserIcones()
{
    iconeBoutonDetectee   = new QIcon(":/images/cafetiere-rouge.png");
    iconeBoutonConnecte   = new QIcon(":/images/cafetiere-verte.png");
    iconeBoutonDeconnecte = new QIcon(":/images/cafetiere-noire.png");

    iconeCapsuleColombia  = new QIcon(":colombia.png");
    iconeCapsuleIndonesia = new QIcon(":indonesia.png");
    iconeCapsuleEthiopia  = new QIcon(":ethiopia.png");
    iconeCapsuleVolluto   = new QIcon(":volluto.png");
    iconeCapsuleCapriccio = new QIcon(":capriccio.png");
    iconeCapsuleCosi      = new QIcon(":cosi.png");
    iconeCapsuleScuro     = new QIcon(":scuro.png");
    iconeCapsuleVanilla   = new QIcon(":vanilla-eclair.png");
    iconeBacPlein         = new QPixmap(":/images/bacPlein.png");
    iconeBacVide          = new QPixmap(":/images/bacVide.png");
}

void IHMPikawa::initialiserIHM()
{
    ui->statusbar->showMessage(QString::fromUtf8(NOM) + " " +
                               QString::fromUtf8(VERSION));

    chargerBoutonsCafe();

    initialiserIcones();

    activerBoutonConnexionEtatDeconnecte();

    initialiserPreferences();

    afficherPageAcceuil();

    ui->boutonLancerPreparation->setEnabled(false);
}

void IHMPikawa::gererEvenements()
{
    /**
     * @todo A remanier
     */
    connect(ui->selectionLongueurPreparation,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(gererLongueurPreparation(int)));

    connect(ui->boutonInformations,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageInformations()));

    connect(ui->boutonEntretien,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageEntretien()));

    connect(ui->boutonParametres,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageParametres()));

    connect(ui->boutonAcceuilInformation,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageAcceuil()));

    connect(ui->boutonEntretienInformation,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageEntretien()));

    connect(ui->boutonParametresInformation,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageParametres()));

    connect(ui->boutonConnexion,
            SIGNAL(clicked()),
            cafetiere,
            SLOT(gererConnexion()));

    connect(ui->bontonChangerCafe,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageSelectionCafe()));

    connect(ui->boutonAcceuilSelectionCafe,
            SIGNAL(clicked()),
            this,
            SLOT(afficherPageAcceuil()));

    connect(cafetiere,
            SIGNAL(cafetiereDetectee(QString, QString)),
            this,
            SLOT(activerBoutonConnexionEtatDetecte(QString, QString)));
    connect(cafetiere,
            SIGNAL(cafetiereConnectee(QString, QString)),
            this,
            SLOT(activerBoutonConnexionEtatConnecte(QString, QString)));
    connect(cafetiere,
            SIGNAL(cafetiereDeconnectee()),
            this,
            SLOT(activerBoutonConnexionEtatDeconnecte()));

    connect(ui->boutonRafraichir,
            SIGNAL(clicked()),
            this,
            SLOT(rafraichirDecouverte()));
    connect(cafetiere,
            SIGNAL(rechercheTerminee(bool)),
            this,
            SLOT(terminerDecouverte(bool)));

    connect(cafetiere,
            SIGNAL(etatCafetiere(int, bool, bool, bool)),
            this,
            SLOT(mettreAJourEtatCafetiere(int, bool, bool, bool)));

    connect(cafetiere,
            SIGNAL(etatMagasinIHM(QStringList)),
            this,
            SLOT(mettreAJourMagasinIHM(QStringList)));

    connect(cafetiere,
            SIGNAL(cafetierePrete()),
            this,
            SLOT(afficherCafetierePrete()));

    connect(cafetiere,
            SIGNAL(cafetierePasPrete()),
            this,
            SLOT(afficherCafetierePasPrete()));

    connect(ui->boutonLancerPreparation,
            SIGNAL(clicked()),
            cafetiere,
            SLOT(lancerLaPreparationCafe()));

    gererSelectionCafes();
}

void IHMPikawa::initialiserPreferences()
{
    QStringList preferences = cafetiere->getPreferences();

    if(preferences.size() > 0)
    {
        ui->selectionLongueurPreparation->setValue(
          preferences
            .at(Cafetiere::ChampsTablePreferences::
                  COLONNE_PREFERENCES_ID_TYPEBOISSON)
            .toInt() -
          1);
        gererLongueurPreparation(preferences
                                   .at(Cafetiere::ChampsTablePreferences::
                                         COLONNE_PREFERENCES_ID_TYPEBOISSON)
                                   .toInt() -
                                 1);
        switch(
          preferences
            .at(
              Cafetiere::ChampsTablePreferences::COLONNE_PREFERENCES_ID_CAPSULE)
            .toInt() -
          1)
        {
            case Cafetiere::NomCaspule::Colombia:
                selectionnerColombia();
                break;
            case Cafetiere::NomCaspule::Indonesia:
                selectionnerIndonesia();
                break;
            case Cafetiere::NomCaspule::Ethiopia:
                selectionnerEthiopia();
                break;
            case Cafetiere::NomCaspule::Volluto:
                selectionnerVolluto();
                break;
            case Cafetiere::NomCaspule::Capriccio:
                selectionnerCapriccio();
                break;
            case Cafetiere::NomCaspule::Cosi:
                selectionnerCosi();
                break;
            case Cafetiere::NomCaspule::Scuro:
                selectionnerScuro();
                break;
            case Cafetiere::NomCaspule::Vanilla:
                selectionnerVanilla();
                break;
        }
    }
}

int IHMPikawa::convertirPourcentageEau(int reservoirEau)
{
    int reservoirEauPourcentage = (reservoirEau * 100) / TAILLE_RESERVOIR;
    return reservoirEauPourcentage;
}

void IHMPikawa::chargerBoutonsCafe()
{
    boutonsCafes.push_back(ui->boutonColombia);
    boutonsCafes.push_back(ui->boutonIndonesia);
    boutonsCafes.push_back(ui->boutonEthiopia);
    boutonsCafes.push_back(ui->boutonVolluto);
    boutonsCafes.push_back(ui->boutonCapriccio);
    boutonsCafes.push_back(ui->boutonCosi);
    boutonsCafes.push_back(ui->boutonScuro);
    boutonsCafes.push_back(ui->boutonVanilla);
}

void IHMPikawa::ouvrirBaseDeDonnees()
{
    baseDeDonnees = BaseDeDonnees::getInstance();
    baseDeDonnees->ouvrir(NOM_BDD);
}

void IHMPikawa::afficherAvertissement(int  niveauEau,
                                      bool bacPasPlein,
                                      bool capsulePresente,
                                      bool tassePresente)
{
    QString message = "";

    if((cafetiere->getNiveauEau() - cafetiere->getNiveauEauNecessaire()) <= 0)
    {
        message.append("Eau insuffisante ");
    }

    if(!bacPasPlein)
    {
        message.append("Bac plein ");
    }

    if(!capsulePresente)
    {
        message.append("Caspule non présente ");
    }

    if(!tassePresente)
    {
        message.append("Tasse non présente ");
    }

    if(!cafetiere->estCapsuleChoisieDisponible())
    {
        message.append("Caspule choisie indisponible ");
        ui->capsuleChoisie->setStyleSheet("font-size:25px; color:red;");
    }

    afficherMessage(message, "red");
}

void IHMPikawa::afficherMessage(QString message, QString couleur)
{
    qDebug() << Q_FUNC_INFO << "message" << message;
    ui->labelAvertisseur->setText(message);
    ui->labelAvertisseur->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
    ui->labelAvertisseur->setStyleSheet("font-size:25px; color:" + couleur +
                                        ";");
}
