#ifndef CREATE_VIRT_NETWORK_ASS_H
#define CREATE_VIRT_NETWORK_ASS_H

#include <QWizard>
#include <QDir>
#include <QTemporaryFile>

class CreateVirtNetwork_Ass : public QWizard
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork_Ass(QWidget *parent = nullptr);
    enum {
        Page_Intro,
        Page_Bridge,
        Page_NATed,
        Page_Routed,
        Page_Isolated,
        Page_Conclusion,
        // next is additional page enumerates
        Page_Forward,
        Page_IP,
        Page_NoGateway
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
