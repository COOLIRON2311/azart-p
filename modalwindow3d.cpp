#include "modalwindow3d.h"

void ModalWindow3D::setup()
{

    mesh = new Qt3DRender::QMesh();
    mesh->setSource(model);
    mat = new  Qt3DExtras::QTextureMaterial();

    trans = new Qt3DCore::QTransform();
    trans->setScale(0.5f);
    trans->setTranslation(QVector3D(dx, dy, dz));
    trans->setRotationX(rx);
    trans->setRotationY(ry);
    trans->setRotationZ(rz);

    view = new Qt3DExtras::Qt3DWindow();
    view->installEventFilter(this);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    container = QWidget::createWindowContainer(view);
    QSize sz = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(sz);

    widget = new QWidget;
    hLayout = new QHBoxLayout(widget);
    hLayout->addWidget(container, 1);

    widget->setWindowTitle("Загрузка...");
    widget->setWindowIcon(QIcon(":/resources/star.png"));

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    loader = new Qt3DRender::QTextureLoader(rootEntity);
    loader->setSource(texture);
    mat->setTexture(loader);

    mesh_Entity = new Qt3DCore::QEntity(rootEntity);
    mesh_Entity->addComponent(mesh);
    mesh_Entity->addComponent(mat);
    mesh_Entity->addComponent(trans);
    mesh_Entity->setEnabled(true);

    // Camera
    cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    lightEntity = new Qt3DCore::QEntity(rootEntity);
    light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    // Set root object of the scene
    view->setRootEntity(rootEntity);
    widget->resize(1200, 800);
}

ModalWindow3D::ModalWindow3D(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    model = QUrl(QStringLiteral("qrc:/resources/mesh.obj"));
    texture = QUrl(QStringLiteral("qrc:/resources/base.tga"));
    setup();
}

void ModalWindow3D::show()
{
    // TODO: Dirty hack here. Fix memory leak ASAP!
    setup();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(loading()));
    widget->show();
}

bool ModalWindow3D::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // --------
        // Movement
        // --------

        if (keyEvent->key() == Qt::Key_W)
        {
            dy++;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }
        else if (keyEvent->key() == Qt::Key_S)
        {
            dy--;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }
        else if (keyEvent->key() == Qt::Key_A)
        {
            dx--;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }
        else if (keyEvent->key() == Qt::Key_D)
        {
            dx++;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Z)
        {
            dz--;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }
        else if (keyEvent->key() == Qt::Key_X)
        {
            dz++;
            trans->setTranslation(QVector3D(dx, dy, dz));
            return true;
        }

        // --------
        // Rotation
        // --------

        else if (keyEvent->key() == Qt::Key_Up)
        {
            rx = (rx + 1) % 360;
            trans->setRotationX(rx);
            return true;
        }

        else if (keyEvent->key() == Qt::Key_Down)
        {
            rx = (rx - 1) % 360;
            trans->setRotationX(rx);
            return true;
        }

        else if (keyEvent->key() == Qt::Key_Right)
        {
            ry = (ry + 1) % 360;
            trans->setRotationY(ry);
            return true;
        }

        else if (keyEvent->key() == Qt::Key_Left)
        {
            ry = (ry - 1) % 360;
            trans->setRotationY(ry);
            return true;
        }

        else if (keyEvent->key() == Qt::Key_Escape)
        {
            rx = 0, ry = 180, rz = 0;
            dx = 0, dy = -7, dz = 0;
            trans->setTranslation(QVector3D(dx, dy, dz));
            trans->setRotationX(rx);
            trans->setRotationY(ry);
            trans->setRotationZ(rz);

            return true;
        }

        else
            return false;
    }
//    else if (event->type() == QEvent::Hide)
//    {
//       delete view;
//       delete container;
//       delete hLayout;
//       delete vLayout;
//       delete rootEntity;
//       delete cameraEntity;
//       delete lightEntity;
//       delete light;
//       delete lightTransform;
//       delete mesh;
//       delete mesh_Entity;
//       delete mat;
//       delete trans;
//       return true;
//    }
    return false;
}

void ModalWindow3D::loading()
{
    if (loader->status() == Qt3DRender::QTextureLoader::Ready)
    {
        timer->stop();
        widget->setWindowTitle("3D Просмотр");
        disconnect(this, SLOT(loading()));
    }
}

