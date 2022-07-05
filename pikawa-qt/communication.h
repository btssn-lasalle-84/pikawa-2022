#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author Anthony BRYCKAERT
 * @version 1.1
 */

#include <QObject>
#include <QtBluetooth>
#include "protocole.h"

/**
 * @def PREFIXE_NOM_CAFETIERE
 * @brief L'appareil Bluetooth d'une cafetière doit contenir ce préfixe de nom
 */
#define PREFIXE_NOM_CAFETIERE "PIKAWA-2"
#define ADRESSE_MAC_PIKAWA    "30:AE:A4:23:D6:7E"

/**
 * @def TEST_TRAMES
 * @brief Pour les tests de trames
 */
//#define TEST_TRAMES

/**
 * @class Communication
 * @brief Permet de gérer les échanges Bluetooth
 */
class Communication : public QObject
{
    Q_OBJECT
  private:
    QBluetoothLocalDevice interfaceLocale; //!< l'interface Bluetooth locale

    /*
     * @todo
     * Remplacer le QBluetoothDeviceDiscoveryAgent par le QBluetoothServiceInfo
     */

    QBluetoothDeviceDiscoveryAgent*
      agentDecouvreur; //!< pour découvrir des interfaces Bluetooth
    QBluetoothDeviceInfo
         pikawa;        //!< l'int erface Bluetooth de la cafetière pikawa
    bool connecte;      //!< l'état de connexion de la socket Bluetooth
    bool pikawaDetecte; //!< état de détection de l'interface pikawa
    QBluetoothSocket*
            socketBluetoothPikawa; //!< socket cliente de communication Bluetooth
    QString trameRecue;            //!< la trame reçue

    bool      estBluetoothDisponible() const;
    void      activerBluetooth();
    bool      estTrameValide(QString trame);
    TypeTrame extraireTypeTrame(QString trame);
    bool      traiterTrame(TypeTrame typeTrame, QString trame);

    void effacerLeCacheBluetooth();

  public:
    Communication(QObject* parent = nullptr);
    ~Communication();

    bool estPreteAEtreConnecte() const;
    bool estConnecte() const;
    bool estCafetiereDetectee() const;
    void envoyerTrame(QString trame);
    void envoyerTramePreparation(int nomCafe, int longueur);

    /**
     * @enum ChampEtatCafetiere
     * @brief Les différents champs de la trame EtatCafetiere
     */
    enum ChampEtatCafetiere
    {
        NiveauEau = 2,
        NiveauBac,
        CaspulePresente,
        TassePresente,
        NbChampsEtatCafetiere
    };
    /**
     * @enum ChampEtatMagasin
     * @brief Les différents champs de la trame EtatMagasin
     */
    enum ChampEtatMagasin
    {
        Colombia = 2,
        Indonesia,
        Ethiopia,
        Volluto,
        Capriccio,
        Cosi,
        Scuro,
        Vanilla,
        NbChampsEtatMagasin
    };
    /**
     * @enum ChampEtatPreparation
     * @brief Les différents champs de la trame EtatPreparation
     */
    enum ChampEtatPreparation
    {
        Cafe = 2,
        NbChampsEtatPreparation
    };

  public slots:
    void activerLaDecouverte();
    void desactiverLaDecouverte();
    void decouvrirCafetiere(const QBluetoothDeviceInfo& appareilBluetooth);
    void terminerRecherche();
    void connecter();
    void deconnecter();
    void recupererEtatConnexion();
    void recupererEtatDeconnexion();
    void recevoir();
    void lireEtatSocket();

  signals:
    void cafetiereDetectee(QString nom, QString adresse);
    void cafetiereConnectee(QString nom, QString adresse);
    void cafetiereDeconnectee();
    void rechercheTerminee(bool detecte);
    void etatCafetiere(int  reservoirEau,
                       bool bacPasPlein,
                       bool etatCapsule,
                       bool etatTasse);
    void etatMagasin(QStringList caspulesDisponibles);
    void cafeEnPreparation(int preparationCafe);
    void erreurPreparationCafe(int codeErreur);
};

#endif // COMMUNICATION_H
