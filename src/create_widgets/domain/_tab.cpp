#include "_tab.h"

_Tab::_Tab(QWidget *parent) :
    _QWidget(parent)
{
}

/* public slots */
QString _Tab::closeDataEdit()
{
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    QString("Save Data"),
                    QString("In %1:\nSave last changes?")
                    .arg(objectName()),
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok )
            saveData();
        else
            revertData();
    };
    return QString();
}

/* private slots */
void _Tab::stateChanged()
{
    if ( currentStateSaved ) {
        currentStateSaved = false;
    };
    emit dataChanged();
}
void _Tab::readXMLDesciption()
{

}
void _Tab::readXMLDesciption(QString &_xmlDesc)
{

}
void _Tab::resetData()
{
    readXMLDesciption();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void _Tab::revertData()
{
    readXMLDesciption(currentDeviceXMLDesc);
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
void _Tab::saveData()
{
    QDomDocument doc;
    QDomElement _el;
    doc = getDataDocument();
    _el = doc.firstChildElement("data");
    _el.setTagName("domain");
    currentDeviceXMLDesc = doc.toString();
    currentStateSaved = true;
    restorePanel->stateChanged(false);
}
