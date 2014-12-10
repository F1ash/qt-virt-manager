#include "wipe_menu.h"

WipeMenu::WipeMenu(QWidget *parent) :
    QMenu(parent)
{
    setTitle("Wipe Algorithm");
    algorithm_ZERO = new QAction("ZERO", this);
    algorithm_NNSA = new QAction("NNSA", this);
    algorithm_DOD = new QAction("DOD", this);
    algorithm_BSI = new QAction("BSI", this);
    algorithm_GUTMANN = new QAction("GUTMANN", this);
    algorithm_SCHNEIER = new QAction("SCHNEIER", this);
    algorithm_PFITZNER7 = new QAction("PFITZNER7", this);
    algorithm_PFITZNER33 = new QAction("PFITZNER33", this);
    algorithm_RANDOM = new QAction("RANDOM", this);

    addAction(algorithm_ZERO);
    addAction(algorithm_NNSA);
    addAction(algorithm_DOD);
    addAction(algorithm_BSI);
    addAction(algorithm_GUTMANN);
    addAction(algorithm_SCHNEIER);
    addAction(algorithm_PFITZNER7);
    addAction(algorithm_PFITZNER33);
    addAction(algorithm_RANDOM);

    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(emitExecMethod(QAction*)));
    connect(this, SIGNAL(hovered(QAction*)), this, SLOT(showActionToolTip(QAction*)));
}

/* private slots */
void WipeMenu::emitExecMethod(QAction *action)
{
    QStringList parameters;
    if ( action == algorithm_ZERO ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_ZERO);
    } else if ( action == algorithm_NNSA ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_NNSA);
    } else if ( action == algorithm_DOD ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_DOD);
    } else if ( action == algorithm_BSI ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_BSI);
    } else if ( action == algorithm_GUTMANN ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_GUTMANN);
    } else if ( action == algorithm_SCHNEIER ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_SCHNEIER);
    } else if ( action == algorithm_PFITZNER7 ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_PFITZNER7);
    } else if ( action == algorithm_PFITZNER33 ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_PFITZNER33);
    } else if ( action == algorithm_RANDOM ) {
        parameters << "wipeVirtStorageVol" << QString("%1").arg(VIR_STORAGE_VOL_WIPE_ALG_RANDOM);
    } else return;
    emit execMethod(parameters);
}
void WipeMenu::showActionToolTip(QAction *action)
{
    QString toolTip;
    if ( action == algorithm_ZERO ) {
        toolTip = QString("1-pass, all zeroes");
    } else if ( action == algorithm_NNSA ) {
        toolTip = QString("4-pass NNSA Policy Letter NAP-14.1-C (XVI-8)");
    } else if ( action == algorithm_DOD ) {
        toolTip = QString("4-pass DoD 5220.22-M section 8-306 procedure");
    } else if ( action == algorithm_BSI ) {
        toolTip = QString("9-pass method recommended by the German Center\nof Security in Information Technologies");
    } else if ( action == algorithm_GUTMANN ) {
        toolTip = QString("The canonical 35-pass sequence");
    } else if ( action == algorithm_SCHNEIER ) {
        toolTip = QString("7-pass method described by Bruce Schneier\nin \"Applied Cryptography\" (1996)");
    } else if ( action == algorithm_PFITZNER7 ) {
        toolTip = QString("7-pass random");
    } else if ( action == algorithm_PFITZNER33 ) {
        toolTip = QString("33-pass random");
    } else if ( action == algorithm_RANDOM ) {
        toolTip = QString("1-pass random");
    };
    setToolTip(toolTip);
}
