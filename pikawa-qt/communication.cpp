#include "communication.h"
#include "protocole.h"
#include <QDebug>

/**
 * @file communication.cpp
 *
 * @brief Définition de la classe Communication
 * @author Anthony BRYCKAERT
 * @version 1.1
 */

Communication::Communication(QObject* parent) :
    QObject(parent), agentDecouvreur(nullptr), connecte(false),
    pikawaDetecte(false), socketBluetoothPikawa(nullptr)
{
    qDebug() << Q_FUNC_INFO << "Bluetooth" << interfaceLocale.isValid();
    activerBluetooth();
    activerLaDecouverte();

#ifdef TEST_TRAMES
    recevoir();
#endif
}

Communication::~Communication()
{
    deconnecter();
    qDebug() << Q_FUNC_INFO;
}

bool Communication::estBluetoothDisponible() const
{
    return interfaceLocale.isValid();
}

void Communication::activerBluetooth()
{
    if(estBluetoothDisponible())
    {
        qDebug() << Q_FUNC_INFO << interfaceLocale.name();
        interfaceLocale.powerOn();
    }
    else
        qDebug() << Q_FUNC_INFO << "Pas de bluetooh !";
}

/**
 * @fn Communication::estTrameValide(QString trame)
 * @brief Vérifie si la trame est valide
 * @details La trame est valide si elle commence avec "$" et fini avec "/r/n"
 */
bool Communication::estTrameValide(QString trame)
{
    qDebug() << Q_FUNC_INFO << trame
             << (trame.startsWith(ENTETE) && trame.endsWith(FIN_TRAME));
    return (trame.startsWith(ENTETE) && trame.endsWith(FIN_TRAME));
}

/**
 * @fn Communication::extraireTypeTrame(QString trame)
 * @brief Extrait le type de trame (champ à la position 1 dans la trame)
 */
TypeTrame Communication::extraireTypeTrame(QString trame)
{
    QStringList champs =
      trame.split(QLatin1Char(DELIMITEUR), QString::SkipEmptyParts);
    QString type = champs[CHAMP_TYPE_TRAME];
    qDebug() << Q_FUNC_INFO << type;

    if(type == ETAT_CAFETIERE)
        return TypeTrame::EtatCafetiere;
    else if(type == ETAT_MAGASIN)
        return TypeTrame::EtatMagasin;
    else if(type == ETAT_PREPARATION)
        return TypeTrame::EtatPreparation;
    else if(type == TRAME_ERREUR)
        return TypeTrame::ErreurPreparation;
    else
        return TypeTrame::Inconnue;

    return TypeTrame::Inconnue;
}

/**
 * @fn Communication::traiterTrame(TypeTrame typeTrame, QString trame)
 * @brief Envoie un signal suivant le type de trame
 * @details
 */
bool Communication::traiterTrame(TypeTrame typeTrame, QString trame)
{
    if(typeTrame == TypeTrame::Inconnue)
        return false;
    QStringList champs =
      trame.split(QLatin1Char(DELIMITEUR), QString::SkipEmptyParts);
    qDebug() << Q_FUNC_INFO << typeTrame << champs;
    int     niveauEau;
    bool    caspulePresente, tassePresente, bacPasPlein;
    QString colombiaPresent, indonesiaPresent, ethiopiaPresent, vollutoPresent,
      capriccioPresent, cosiPresent, scuroPresent, vanillaPresent;
    int preparationCafe, codeErreur;

    QStringList caspulesDisponibles;

    switch(typeTrame)
    {
        case TypeTrame::Inconnue:
            qDebug() << Q_FUNC_INFO << "type de trame inconnue !";
            break;
        case TypeTrame::EtatCafetiere:
            niveauEau   = champs[ChampEtatCafetiere::NiveauEau].toInt();
            bacPasPlein = (champs[ChampEtatCafetiere::NiveauBac] == '1');
            caspulePresente =
              (champs[ChampEtatCafetiere::CaspulePresente] == '1');
            tassePresente = (champs[ChampEtatCafetiere::TassePresente] == '1');
            qDebug() << Q_FUNC_INFO << " niveauEau " << niveauEau
                     << " bacPasPlein " << bacPasPlein << " caspulePresente "
                     << caspulePresente << " tassePresente " << tassePresente;
            emit etatCafetiere(niveauEau,
                               bacPasPlein,
                               caspulePresente,
                               tassePresente);
            return true;
        case TypeTrame::EtatMagasin:
            for(int i = ChampEtatMagasin::Colombia;
                i < ChampEtatMagasin::NbChampsEtatMagasin;
                ++i)
            {
                caspulesDisponibles << champs.at(i);
            }
            qDebug() << Q_FUNC_INFO << " capulesDisponibles "
                     << caspulesDisponibles;
            emit etatMagasin(caspulesDisponibles);
            return true;
        case TypeTrame::EtatPreparation:
            preparationCafe = champs[ChampEtatPreparation::Cafe].toInt();
            qDebug() << Q_FUNC_INFO << " preparationCafe " << preparationCafe;
            emit cafeEnPreparation(preparationCafe);

            return true;
        case TypeTrame::ErreurPreparation:
            codeErreur = champs[ChampEtatPreparation::Cafe].toInt();
            qDebug() << Q_FUNC_INFO << " codeErreur " << codeErreur;
            emit erreurPreparationCafe(codeErreur);
            return true;
    }
    return false;
}

/**
 * @fn Communication::estPreteAEtreConnecte() const
 * @brief Verifie si la cafetière est connectée
 * @details Verifie si le bluetooth est disponible et si la socket est ouverte
 */

bool Communication::estPreteAEtreConnecte() const
{
    if(!estBluetoothDisponible() || socketBluetoothPikawa == nullptr ||
       !socketBluetoothPikawa->isOpen())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Communication::estCafetiereDetectee() const
{
    return pikawaDetecte;
}

/**
 * @fn Communication::envoyerTrame(QString trame)
 * @brief Envoie la trame passée en paramètre
 * @details Ecrit sur la socket la trame
 */
void Communication::envoyerTrame(QString trame)
{
    qDebug() << Q_FUNC_INFO << trame;
    if(!estConnecte())
    {
        return;
    }
    if(!trame.endsWith(FIN_TRAME))
        trame += QString(FIN_TRAME);
    socketBluetoothPikawa->write(trame.toLatin1());
}

/**
 * @fn Communication::activerLaDecouverte()
 * @brief Scanne les appareils bluetooth à proximité
 */
void Communication::activerLaDecouverte()
{
    if(estBluetoothDisponible())
    {
        qDebug() << Q_FUNC_INFO;
        agentDecouvreur = new QBluetoothDeviceDiscoveryAgent(this);
        if(agentDecouvreur != nullptr)
        {
            effacerLeCacheBluetooth();
            connect(agentDecouvreur,
                    SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                    this,
                    SLOT(decouvrirCafetiere(QBluetoothDeviceInfo)));
            connect(agentDecouvreur,
                    SIGNAL(finished()),
                    this,
                    SLOT(terminerRecherche()));
            pikawaDetecte = false;
            agentDecouvreur->start();
        }
    }
}

void Communication::desactiverLaDecouverte()
{
    qDebug() << Q_FUNC_INFO;
    if(agentDecouvreur != nullptr)
    {
        agentDecouvreur->start();
    }
}

void Communication::effacerLeCacheBluetooth()
{
    qDebug() << Q_FUNC_INFO;
    QProcess* process  = new QProcess;
    QString   commande = "bluetoothctl remove " + QString(ADRESSE_MAC_PIKAWA);
    process->start(commande);
    process->waitForFinished();
    QString resulat = process->readAllStandardOutput();
    qDebug() << Q_FUNC_INFO << resulat;
}

/**
 * @fn Communication::decouvrirCafetiere(
  const QBluetoothDeviceInfo& appareilBluetooth)
 * @brief Stockage des caractéristiques du bluetooth de la cafetiere si elle
 est découverte
 * @details Envoie de signal si la recherche est terminée
 */
void Communication::decouvrirCafetiere(
  const QBluetoothDeviceInfo& appareilBluetooth)
{
    qDebug() << Q_FUNC_INFO << "appareil Bluetooth" << appareilBluetooth.name()
             << '[' << appareilBluetooth.address().toString() << ']' << "rssi"
             << appareilBluetooth.rssi();

    if(appareilBluetooth.name().contains(PREFIXE_NOM_CAFETIERE))
    {
        qDebug() << Q_FUNC_INFO << "machine à café pikawa détectée"
                 << appareilBluetooth.name() << '['
                 << appareilBluetooth.address().toString() << ']';
        agentDecouvreur->stop();
        pikawa        = appareilBluetooth;
        pikawaDetecte = true;
        emit rechercheTerminee(pikawaDetecte);
        emit cafetiereDetectee(appareilBluetooth.name(),
                               appareilBluetooth.address().toString());
        connecter();
    }
}

void Communication::terminerRecherche()
{
    qDebug() << Q_FUNC_INFO << "pikawaDetecte" << pikawaDetecte;
    emit rechercheTerminee(pikawaDetecte);
}

/**
 * @fn Communication::connecter()
 * @brief Slot de connexion
 * @details connexion des "signals" de l'objet socketBluetoothPikawa au
 * "slots"
 */
void Communication::connecter()
{
    qDebug() << Q_FUNC_INFO;

    if(!estPreteAEtreConnecte())
    {
        if(pikawaDetecte)
        {
            qDebug() << Q_FUNC_INFO << "connexion" << pikawa.name() << '['
                     << pikawa.address().toString() << ']';
            socketBluetoothPikawa =
              new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

            if(socketBluetoothPikawa != nullptr)
            {
                connect(socketBluetoothPikawa,
                        SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
                        this,
                        SLOT(lireEtatSocket()));
                connect(socketBluetoothPikawa,
                        SIGNAL(connected()),
                        this,
                        SLOT(recupererEtatConnexion()));
                connect(socketBluetoothPikawa,
                        SIGNAL(disconnected()),
                        this,
                        SLOT(recupererEtatDeconnexion()));
                connect(socketBluetoothPikawa,
                        SIGNAL(readyRead()),
                        this,
                        SLOT(recevoir()));

                QBluetoothAddress adresse = QBluetoothAddress(pikawa.address());
                QBluetoothUuid    uuid =
                  QBluetoothUuid(QBluetoothUuid::SerialPort);

                socketBluetoothPikawa->connectToService(adresse, uuid);
                socketBluetoothPikawa->open(QIODevice::ReadWrite);
            }
            else
                qDebug() << Q_FUNC_INFO << "erreur création socket";
        }
        else
            qDebug() << Q_FUNC_INFO << "erreur pikawa non détecté";
    }
    qDebug() << Q_FUNC_INFO << estPreteAEtreConnecte();
}

/**
 * @fn Communication::deconnecter()
 * @brief Slot de déconnexion
 * @details Ferme la socket
 */

void Communication::deconnecter()
{
    if(!estBluetoothDisponible())
        return;
    if(socketBluetoothPikawa == nullptr)
        return;
    if(socketBluetoothPikawa->isOpen())
    {
        qDebug() << Q_FUNC_INFO;
        socketBluetoothPikawa->close();
        connecte = false;
    }
}

void Communication::recupererEtatConnexion()
{
    qDebug() << Q_FUNC_INFO << socketBluetoothPikawa->peerName()
             << socketBluetoothPikawa->peerAddress().toString();
    connecte = true;
    emit cafetiereConnectee(socketBluetoothPikawa->peerName(),
                            socketBluetoothPikawa->peerAddress().toString());
}

void Communication::recupererEtatDeconnexion()
{
    qDebug() << Q_FUNC_INFO;
    connecte = false;
    emit cafetiereDeconnectee();
}

/**
 * @fn Communication::recevoir()
 * @brief Receptionne les trames
 * @details Slot appelé à chaque fois que l'objet socketBluetoothPikawa
 * envoie le signal readyRead()
 */
void Communication::recevoir()
{
#ifndef TEST_TRAMES
    QByteArray donnees;
    donnees = socketBluetoothPikawa->readAll();
    qDebug() << Q_FUNC_INFO << "donnees" << donnees;
    trameRecue += QString(donnees.data());
#else
    // on peut simuler des trames reçues
    // trameRecue = "$PIKAWA;C;50;60;1;1;\r\n";
    trameRecue = "$PIKAWA;M;0;1;1;0;1;0;1;0;\r\n";
    // trameRecue = "$PIKAWA;P;1;\r\n";
    // trameRecue = "$PIKAWA;W;50;60;1;1;\r\n"; // trame invalide
#endif
    qDebug() << Q_FUNC_INFO << "trameRecue" << trameRecue;
    if(estTrameValide(trameRecue))
    {
        TypeTrame typeTrame = extraireTypeTrame(trameRecue);
        if(traiterTrame(typeTrame, trameRecue))
        {
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "erreur traitement";
        }
        trameRecue.clear();
    }
}

void Communication::lireEtatSocket()
{
    qDebug() << Q_FUNC_INFO << socketBluetoothPikawa->state();
}

/**
 * @fn Communication::envoyerTramePreparation(int nomCafe, int longueur)
 * @brief Envoie d'une trame de préparation
 * @details Exemple PIKAWA;7;2;\r\n
 */
void Communication::envoyerTramePreparation(int nomCafe, int longueur)
{
    qDebug() << Q_FUNC_INFO << nomCafe << longueur;
    envoyerTrame(QString(DEBUT_TRAME_PREPARATION) + QString::number(nomCafe) +
                 QString(DELIMITEUR) + QString::number(longueur + 1) +
                 QString(DELIMITEUR) + QString(FIN_TRAME));
}

bool Communication::estConnecte() const
{
    return connecte;
}
