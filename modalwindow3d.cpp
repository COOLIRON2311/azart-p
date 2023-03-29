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
    this->setCentralWidget(QWidget::createWindowContainer(view));
    this->centralWidget()->setMinimumSize(QSize(500, 800));
    this->centralWidget()->setMaximumSize(sz);

    this->setWindowTitle("Загрузка...");
    this->setWindowIcon(QIcon(":/resources/star.png"));

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
    this->resize(500, 800);
}

ModalWindow3D::ModalWindow3D() : QMainWindow()
{
    timer = new QTimer(this);
    model = QUrl(QStringLiteral("qrc:/resources/mesh.obj"));
    texture = QUrl(QStringLiteral("qrc:/resources/base.tga"));
}

void ModalWindow3D::show()
{
    setup();
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(loading()));
    QMainWindow::show();
}

void ModalWindow3D::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    this->hide();
}

bool ModalWindow3D::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    bool ret = false;
//    if (event->type() == QEvent::Resize)
//    {
//        qDebug() << size();
//    }
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // --------
        // Movement
        // --------

        if (keyEvent->key() == Qt::Key_W)
        {
            bW = true;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_S)
        {
            bS = true;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_A)
        {
            bA = true;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_D)
        {;
            bD = true;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_Z)
        {
            bZ = true;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_X)
        {
            bX = true;
            ret = true;
        }

        // --------
        // Rotation
        // --------

        else if (keyEvent->key() == Qt::Key_Up)
        {

            bUp = true;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Down)
        {
            bDo = true;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Right)
        {
            bRi = true;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Left)
        {
            bLe = true;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Escape)
        {
            rx = 0, ry = 180, rz = 0;
            dx = 0, dy = -7, dz = 0;
            bLe = false, bRi = false, bUp = false, bDo = false;
            bA = false, bD = false, bW = false, bS = false, bZ = false, bX = false;
            trans->setTranslation(QVector3D(dx, dy, dz));
            trans->setRotationX(rx);
            trans->setRotationY(ry);
            trans->setRotationZ(rz);

            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_F11 || keyEvent->key() == Qt::Key_F)
        {
            if (isMaximized())
                showNormal();
            else
                showMaximized();
            ret = true;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        // --------
        // Movement
        // --------

        if (keyEvent->key() == Qt::Key_W)
        {
            bW = false;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_S)
        {
            bS = false;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_A)
        {
            bA = false;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_D)
        {
            bD = false;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_Z)
        {
            bZ = false;
            ret = true;
        }
        else if (keyEvent->key() == Qt::Key_X)
        {
            bX = false;
            ret = true;
        }

        // --------
        // Rotation
        // --------

        else if (keyEvent->key() == Qt::Key_Up)
        {
            bUp = false;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Down)
        {
            bDo = false;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Right)
        {
            bRi = false;
            ret = true;
        }

        else if (keyEvent->key() == Qt::Key_Left)
        {
            bLe = false;
            ret = true;
        }
    }

    if (bA || bD || bS || bW || bX || bZ)
    {
        if (bA)
            dx--;
        if (bD)
            dx++;
        if (bS)
            dy--;
        if (bW)
            dy++;
        if (bZ)
            dz--;
        if (bX)
            dz++;
        trans->setTranslation(QVector3D(dx, dy, dz));
    }
    if (bLe || bRi || bDo || bUp)
    {
        if (bLe)
            ry = (ry - 1) % 360;
        if (bRi)
            ry = (ry + 1) % 360;
        if (bDo)
            rx = (rx - 1) % 360;
        if (bUp)
            rx = (rx + 1) % 360;

        trans->setRotationX(rx);
        trans->setRotationY(ry);
        trans->setRotationZ(rz);
    }
    return ret;
}

void ModalWindow3D::loading()
{
    if (loader->status() == Qt3DRender::QTextureLoader::Ready)
    {
        timer->stop();
        this->setWindowTitle("3D Просмотр");
        disconnect(this, SLOT(loading()));
    }
}

