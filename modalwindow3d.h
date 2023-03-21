#ifndef MODALWINDOW3D_H
#define MODALWINDOW3D_H

#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>


#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/QTextureMaterial>

#include <QMainWindow>
#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <QScreen>
#include <QMesh>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QTimer>
#include <QMainWindow>

class ModalWindow3D : public QMainWindow
{
    Q_OBJECT
    Qt3DExtras::Qt3DWindow *view;
    QWidget *container;
    QHBoxLayout *hLayout;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QPointLight *light;
    Qt3DCore::QTransform *lightTransform;
    Qt3DRender::QMesh *mesh = 0;
    Qt3DCore::QEntity *mesh_Entity;
    Qt3DRender::QTextureLoader *loader;
    // TODO: use QMetalRoughMaterial
    Qt3DExtras::QTextureMaterial *mat;
    QTimer *timer;

    Qt3DCore::QTransform *trans;
    int rx = 0, ry = 180, rz = 0;
    float dx = 0, dy = -7, dz = 0;
    QUrl model;
    QUrl texture;
    void setup();

public:
    explicit ModalWindow3D();
    void show();
    void closeEvent(QEvent* event);

public slots:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void loading();
signals:

};

#endif // MODALWINDOW3D_H
