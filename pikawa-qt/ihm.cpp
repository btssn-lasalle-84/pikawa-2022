#include "ihm.h"
#include "ui_ihm.h"
#include <QDebug>

/**
 * @file ihm.cpp
 *
 * @brief Définition de la classe IHMPikawa
 * @author
 * @version 1.0
 *
 */

IHMPikawa::IHMPikawa(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::IHMPikawa)
{
    ui->setupUi(this);
    qDebug() << Q_FUNC_INFO;

    gererEvenements();

    initialiserIHM();

#ifdef PLEIN_ECRAN
    showFullScreen();
// showMaximized();
#endif
}

IHMPikawa::~IHMPikawa()
{
    delete ui;
    qDebug() << Q_FUNC_INFO;
}

void IHMPikawa::initialiserIHM()
{
    /**
     * @todo Mettre des constantes pour le NOM et la VERSION
     */
    ui->statusbar->showMessage(QString::fromUtf8("Pikawa 2022"));

    ui->selectionLongueurPreparation->setValue(LongueurCafe::Court);
    afficherLongueurPreparation(LongueurCafe::Court);
}

void IHMPikawa::gererEvenements()
{
    connect(ui->selectionLongueurPreparation,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(afficherLongueurPreparation(int)));
}

void IHMPikawa::afficherLongueurPreparation(int longueurPreparation)
{
    QStringList labelsLongueurPreparation;
    labelsLongueurPreparation << "Court"
                              << "Moyen"
                              << "Long";

    qDebug() << Q_FUNC_INFO << "longueurPreparation" << longueurPreparation;
    ui->labelLongueurPreparation->setText(
      labelsLongueurPreparation.at(longueurPreparation));
}
