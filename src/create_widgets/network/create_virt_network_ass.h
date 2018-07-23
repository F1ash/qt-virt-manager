#ifndef CREATE_VIRT_NETWORK_ASS_H
#define CREATE_VIRT_NETWORK_ASS_H

#include <QWizard>
#include <QDir>
#include <QTemporaryFile>

class CreateVirtNetwork_Ass : public QWizard
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork_Ass(QWidget *parent = Q_NULLPTR);
    enum {
        Page_Intro,
        Page_Bridge,
        Page_Forward,
        Page_IP,
        Page_NoGateway,
        Page_Conclusion
    };
    QString         getXMLDescFileName() const;

signals:
    void            accepted(bool);

private:
    QTemporaryFile *xml;

private slots:
    int             nextId() const;
    void            buildXMLDescription();
    void            set_Result(int);
};

#endif // CREATE_VIRT_NETWORK_ASS_H
