#include "gameController.h"

#include "cards/abstractcard.h"
#include "graphicsItems/label.h"
#include "graphicsItems/newcardwidget.h"
#include "players/dealer.h"
#include "players/player.h"
#include "scene/scene.h"

#include <QDebug>
#include <QGraphicsItem>
#include <random>

namespace Constants
{
const auto distanceBetweenCards = 40;
const QPointF betLabelPos = { 40, 265 };
const QPointF playerLabelPos = { 400, 435 };
const QPointF dealerLabelPos = { 400, 195 };
const QPointF newCardWidgetPos = { 1000, 230 };
} // namespace Constants

GameController::GameController(Scene *scene)
    : QObject(nullptr),
      _scene(scene),
      _player(new Player()),
      _dealer(new Dealer()),
      _betLabel(new Label("bet: 5.00")),
      _playerLabel(new Label("player: ")),
      _dealerLabel(new Label("dealer: ")),
      _newCardWidget(new NewCardWidget()),
      _cardStack(_newCardWidget->initCardStack())
{
    addLabelsOnScene();
    _scene->addItem(_newCardWidget);
    _newCardWidget->setPos(Constants::newCardWidgetPos);
    connect(this, &GameController::playerReceivedCards, this, &GameController::addNewCardToDealer);
    prepareGameTable();
}

GameController::~GameController()
{
    delete _scene;
    delete _player;
    delete _dealer;
    delete _betLabel;
    delete _dealerLabel;
    delete _playerLabel;
    delete _newCardWidget;
}

void GameController::onClickedNewCardWidget()
{
    const auto newCard = getNewCardFromStack();

    newCard->setCardVisible(_cardVisible);

    newCard->setPos(_defaultPlayerCardPos);
    _scene->addItem(newCard);

    _player->addCard(newCard);
    _playerLabel->setPlainText(QStringLiteral("player: %1").arg(QString::number(_player->score())));

    _defaultPlayerCardPos.setX(_defaultPlayerCardPos.x() + Constants::distanceBetweenCards);

    emit playerReceivedCards();
}

void GameController::onClickOnVisibilityToggleWidget()
{
    _cardVisible = !_cardVisible;
    for (const auto playerCard : _player->cards())
    {
        playerCard->setCardVisible(_cardVisible);
    }
}

void GameController::addNewCardToDealer()
{
    if (_dealer->score() >= 17)
        return;

    auto newCard = getNewCardFromStack();

    newCard->setPos(_defaultDealerCardPos);
    _scene->addItem(newCard);

    _dealer->addCard(newCard);
    _dealerLabel->setPlainText(QStringLiteral("dealer: %1").arg(QString::number(_dealer->score())));

    _defaultDealerCardPos.setX(_defaultDealerCardPos.x() + Constants::distanceBetweenCards);
}

AbstractCard *GameController::getNewCardFromStack()
{
    if (_cardStack.empty())
        return nullptr;

    const auto newCard = (_cardStack.front());
    _cardStack.erase(_cardStack.begin());

    return newCard;
}

void GameController::prepareGameTable()
{
    for (int i = 0; i < 2; ++i)
    {
        auto newCard = getNewCardFromStack();
        newCard->setPos(_defaultDealerCardPos);
        _dealer->addCard(newCard);
        _scene->addItem(newCard);

        _defaultDealerCardPos.setX(_defaultDealerCardPos.x() + Constants::distanceBetweenCards);
    }

    _dealer->cards().front()->setCardVisible(false);

    for (int i = 0; i < 2; ++i)
    {
        auto newCard = getNewCardFromStack();

        newCard->setPos(_defaultPlayerCardPos);

        _player->addCard(newCard);
        _scene->addItem(newCard);

        _defaultPlayerCardPos.setX(_defaultPlayerCardPos.x() + Constants::distanceBetweenCards);
    }
    _dealerLabel->setPlainText(QStringLiteral("dealer: %1").arg(QString::number(_dealer->score())));
    _playerLabel->setPlainText(QStringLiteral("player: %1").arg(QString::number(_player->score())));
}

void GameController::addLabelsOnScene()
{
    _scene->addItem(_betLabel);
    _betLabel->setPos(Constants::betLabelPos);
    _scene->addItem(_playerLabel);
    _playerLabel->setPos(Constants::playerLabelPos);
    _scene->addItem(_dealerLabel);
    _dealerLabel->setPos(Constants::dealerLabelPos);
}
