#include "cafetiere.h"
#include "ihm.h"
#include "communication.h"
#include "preparation.h"
#include <QDebug>

/**
 * @file cafetiere.cpp
 *
 * @brief Définitionde la classe Cafetiere
 * @author
 * @version 1.0
 *
 */

Cafetiere::Cafetiere(IHMPikawa* ihm) :
    QObject(ihm), ihm(ihm), communication(nullptr), preparation(nullptr),
    nomCapsules(0), nomBoissons(0), capsuleChoisie(0), boissonChoisie(0),
    niveauEau(0), connectee(false), activee(false), capsulePresente(false),
    tassePresente(false)
{
    qDebug() << Q_FUNC_INFO;
   Communication* communication = new Communication(this);
   Preparation* preparation = new Preparation(this);
}

Cafetiere::~Cafetiere()
{
    qDebug() << Q_FUNC_INFO;
}

void Cafetiere::connecter()
{
    qDebug() << Q_FUNC_INFO;
}
