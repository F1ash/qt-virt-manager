#ifndef HOST_BOOT_H
#define HOST_BOOT_H

#include "create_widgets/domain/_qwidget.h"
#include "path_to_file.h"

class Host_Boot : public _QWidget
{
    Q_OBJECT
public:
    explicit Host_Boot(QWidget *parent = nullptr);
    Path_To_File    *loader;
    QLineEdit       *args;

signals:
    void             domainType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QLabel          *loaderLabel, *argsLabel;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // HOST_BOOT_H
