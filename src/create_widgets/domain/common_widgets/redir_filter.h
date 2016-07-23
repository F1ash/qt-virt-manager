#ifndef REDIR_FILTER_H
#define REDIR_FILTER_H

#include "create_widgets/common/_list_widget.h"

class RedirFilter : public _List_Widget
{
    Q_OBJECT
public:
    explicit RedirFilter(
            QWidget *parent = nullptr,
            QString  tag    = "");
    void             clearList();

private:
    QLineEdit       *_class, *_product,
                    *_vendor, *_version;
    QPushButton     *add;
    QPushButton     *del;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;
    QVBoxLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QStringList      getFiltersList() const;
    void             setFiltersList(QString&, bool);

private slots:
    void             addItem();
    void             delItem();
};

#endif // REDIR_FILTER_H
