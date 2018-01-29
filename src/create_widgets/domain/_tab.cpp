#include "_tab.h"

_Tab::_Tab(QWidget *parent) :
    _QWidget(parent)
{
}

/* public slots */
bool _Tab::closeDataEdit()
{
    bool ret = true;
    if ( !currentStateSaved ) {
        int answer = QMessageBox::question(
                    this,
                    tr("Save Data"),
                    QString(tr("In %1:\nSave last changes?"))
                    .arg(objectName()),
                    QMessageBox::Ok,
                    QMessageBox::Cancel);
        if ( answer==QMessageBox::Ok ) {
            saveData();
        } else {
            revertData();
            ret = false;
        };
    };
    return ret;
}
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
void _Tab::readXMLDesciption(const QString &_xmlDesc)
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
