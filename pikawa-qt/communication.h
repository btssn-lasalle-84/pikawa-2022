#ifndef COMMUNICATION_H
#define COMMUNICATION_H

/**
 * @file communication.h
 *
 * @brief Déclaration de la classe Communication
 * @author
 * @version 1.0
 *
 */

#include <QObject>
#include <QtBluetooth>

/**
 * @def PREFIXE_NOM_CAFETIERE
 * @brief Le nom de l'appareil Bluetooth d'une cafetière doit contenir ce
 * préfixe
 */
#define PREFIXE_NOM_CAFETIERE "PIKAWA"

/**
 * @class Communication
 * @brief
 * @details
 */
class Communication : public QObject
{
    Q_OBJECT
  private:
    QBluetoothLocalDevice interfaceLocale; //!< l'interface Bluetooth locale
    QBluetoothDeviceDiscoveryAgent*
      agentDecouvreur; //!< pour découvrir des interfaces Bluetooth
    QBluetoothDeviceInfo
         pikawa;        //!< l'interface Bluetooth de la cafetière pikawa
    bool pikawaDetecte; //!< état de détection de l'interface pikawa
    QBluetoothSocket*
      socketBluetoothPikawa; //!< La socket cliente de communication Bluetooth

    bool estBluetoothDisponible() const;
    void activerBluetooth();

  public:
    Communication(QObject* parent = nullptr);
    ~Communication();

    bool estConnecte() const;

  public slots:
    void activerLaDecouverte();
    void decouvrirCafetiere(const QBluetoothDeviceInfo& appareilBluetooth);
    void deconnecter();

  signals:
    void cafetiereDetectee(QString nom, QString adresse);
};

#endif // COMMUNICATION_H
