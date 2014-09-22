#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include <QStackedWidget>
#include "graph_dev_widgets/desktop_graphics.h"
#include "graph_dev_widgets/rdp_graphics.h"
#include "graph_dev_widgets/sdl_graphics.h"
#include "graph_dev_widgets/spice_graphics.h"
#include "graph_dev_widgets/vnc_graphics.h"

class GraphicsDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit GraphicsDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QComboBox       *type;
    QStackedWidget  *info;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void setWidgets(int);
};

#endif // GRAPHICS_DEVICE_H
