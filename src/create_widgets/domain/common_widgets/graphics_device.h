#ifndef GRAPHICS_DEVICE_H
#define GRAPHICS_DEVICE_H

#include "graph_dev_widgets/desktop_graphics.h"
#include "graph_dev_widgets/rdp_graphics.h"
#include "graph_dev_widgets/sdl_graphics.h"
#include "graph_dev_widgets/spice_graphics.h"
#include "graph_dev_widgets/vnc_graphics.h"

class GraphicsDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit GraphicsDevice(QWidget *parent = 0);

private:
    QComboBox       *type;
    _QWidget        *info = NULL;
    QVBoxLayout     *commonLayout;

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;
    QString      getDevDisplay() const;
    QString      getDevXauth() const;
    QString      getDevFullScreen() const;

private slots:
    void typeChanged(int);
};

#endif // GRAPHICS_DEVICE_H
