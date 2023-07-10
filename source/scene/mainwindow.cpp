#include "mainwindow.h"
#include "gameController/gameController.h"
#include "graphicsItems/gamewidget.h"
#include "scene.h"
#include "sceneview.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _scene(new Scene(this)),
      _gameController(new GameController(_scene)),
      _sceneView(new SceneView(this)),
      _gameWidget(new GameWidget(this))
{
    setFixedSize(1200, 800);
    setupUI();
    initConnections();
}

void MainWindow::setupUI()
{
    _sceneView->setGeometry(0, 0, width(), height() * 0.8);
    _sceneView->setScene(_scene);

    _gameWidget->setGeometry(0, height() * 0.8, width(), height() * 0.2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(_sceneView);
    layout->addWidget(_gameWidget);
}

void MainWindow::initConnections()
{
    connect(_gameController, &GameController::isMatchTie, _gameWidget, &GameWidget::foo);
    connect(_gameController, &GameController::dealerIsWinner, _gameWidget, &GameWidget::foo);
    connect(_gameController, &GameController::playerIsWinner, _gameWidget, &GameWidget::foo);
}
