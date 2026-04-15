#include "gameview.h"
#include <QKeyEvent>
#include <QBrush>
#include <QPen>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QPainter>

// Fallback colored rectangle when image file is missing
static QPixmap colorPixmap(QColor c, int w = TILE, int h = TILE) {
    QPixmap px(w, h);
    px.fill(c);
    return px;
}

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)),
    level(nullptr), hud(nullptr),
    playerSprite(nullptr), enemySprite(nullptr),
    spritesLoaded(false)
{
    setScene(scene);
    setWindowTitle("Stranger Things: The Dungeon — Level 1");
    setFixedSize(12 * TILE + 2, 9 * TILE + 102);   // map + HUD
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::StrongFocus);

    // Dark background
    setBackgroundBrush(QColor(10, 10, 15));
}

GameView::~GameView() {}

void GameView::loadPixmaps() {
    // Try to load real assets; fall back to colored blocks
    auto load = [](const QString& path, QColor fallback) {
        QPixmap px(path);
        return px.isNull() ? colorPixmap(fallback) : px.scaled(TILE, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    };

    pxFloor      = load(":/assets/floor.png",       QColor(50,  50,  60));
    pxWall       = load(":/assets/wall.png",        QColor(30,  30,  40));
    pxTrap       = load(":/assets/trap.png",        QColor(180, 60,  0));
    pxDoorLocked = load(":/assets/door_locked.png", QColor(100, 70,  0));
    pxDoorOpen   = load(":/assets/door_open.png",   QColor(160, 120, 40));
    pxTreasure   = load(":/assets/treasure.png",    QColor(255, 210, 0));
    pxEleven     = load(":/assets/eleven.png",      QColor(60,  120, 255));
    pxPapa       = load(":/assets/papa.png",        QColor(200, 40,  40));

    spritesLoaded = true;
}

void GameView::initLevel(Level* lvl) {
    level = lvl;
    scene->clear();

    if (!spritesLoaded) loadPixmaps();

    // Persistent character sprites
    playerSprite = scene->addPixmap(pxEleven);
    playerSprite->setZValue(2);

    enemySprite = scene->addPixmap(pxPapa);
    enemySprite->setZValue(2);

    hud = new HUD(scene, 9 * TILE);
}

void GameView::drawMap() {
    GridMap* map = level->getMap();
    Door*    door = level->getDoor();

    for (int y = 0; y < map->getHeight(); y++) {
        for (int x = 0; x < map->getWidth(); x++) {
            Tile t = map->getTile(Position(x, y));
            QPixmap* px = &pxFloor;

            switch (t.getType()) {
            case Wall:         px = &pxWall;       break;
            case TrapTile:     px = &pxTrap;       break;
            case TreasureTile: px = &pxTreasure;   break;
            case DoorTile:
                px = door->isOpen() ? &pxDoorOpen : &pxDoorLocked;
                break;
            default:           px = &pxFloor;      break;
            }

            auto* item = scene->addPixmap(*px);
            item->setPos(x * TILE, y * TILE);
            item->setZValue(0);
        }
    }
}

void GameView::drawObjects() {
    // Trap already shown via tile; nothing extra needed for now.
    // Treasure already shown via tile.
    // Collected treasure: dim it
    if (level->getTreasure()->isCollected()) {
        auto* dim = scene->addRect(
            level->getTreasure()->getPosition().x * TILE,
            level->getTreasure()->getPosition().y * TILE,
            TILE, TILE,
            QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 160)));
        dim->setZValue(1);
    }
}

void GameView::drawCharacters() {
    Player* p = level->getPlayer();
    Enemy*  e = level->getEnemy();

    if (p->isAlive()) {
        playerSprite->setPos(p->getPosition().x * TILE, p->getPosition().y * TILE);
        playerSprite->setVisible(true);
    } else {
        playerSprite->setVisible(false);
    }

    if (e->isAlive()) {
        enemySprite->setPos(e->getPosition().x * TILE, e->getPosition().y * TILE);
        enemySprite->setVisible(true);
    } else {
        enemySprite->setVisible(false);
    }
}

void GameView::redraw(bool isPlayerTurn) {
    QList<QGraphicsItem*> all = scene->items();
    for (auto* item : all) {
        if (item->zValue() < 2) {   // only remove map tiles/overlays
            scene->removeItem(item);
            delete item;
        }
    }
    drawMap();
    drawObjects();
    drawCharacters();
    if (hud) hud->update(level->getPlayer(), level->getEnemy(), isPlayerTurn);
}

void GameView::showMessage(const QString& msg) {
    if (hud) hud->showMessage(msg);
}

void GameView::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Up:    case Qt::Key_W: emit moveRequested( 0, -1); break;
    case Qt::Key_Down:  case Qt::Key_S: emit moveRequested( 0,  1); break;
    case Qt::Key_Left:  case Qt::Key_A: emit moveRequested(-1,  0); break;
    case Qt::Key_Right: case Qt::Key_D: emit moveRequested( 1,  0); break;
    case Qt::Key_Q:                     emit psychicAttackRequested(); break;
    case Qt::Key_Return:
    case Qt::Key_Space:                 emit endTurnRequested(); break;
    default: break;
    }
}