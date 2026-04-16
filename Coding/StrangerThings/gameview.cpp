#include "gameview.h"//includes the gameview class
#include <QKeyEvent>//handles keyboard input
#include <QBrush>//to color objects
#include <QPen>// for drawing outlines
#include <QGraphicsRectItem>//rectangle graphics items
#include <QPixmap>//for handling images
#include <QPainter>// to draw on pixmaps

//creates a colored square pixmap if image doesnt load
static QPixmap colorPixmap(QColor c, int w = TILE, int h = TILE) {
    QPixmap px(w, h); //create pixmap
    px.fill(c);//fill it with a color
    return px;//return the pixmap
}
//initializes the game view
GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),// base class constructor
    scene(new QGraphicsScene(this)),// create graphics scene
    level(nullptr), hud(nullptr),// no level or HUD yet
    playerSprite(nullptr), enemySprite(nullptr),// sprites not created yet
    spritesLoaded(false) // images not loaded yet
{
    setScene(scene);// connect scene to view
    setWindowTitle("Stranger Things: The Dungeon — Level 1");// window title
    setFixedSize(12 * TILE + 2, 9 * TILE + 102);   // size of window
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// disable scroll
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// disable scroll
    setFocusPolicy(Qt::StrongFocus);// allows keyboard input

    setBackgroundBrush(QColor(10, 10, 15));// dark background color
}

GameView::~GameView() {}// destructor

void GameView::loadPixmaps() {// loads all image assets
    // function to load image or color
    auto load = [](const QString& path, QColor fallback) {
        QPixmap px(path);// try loading image
        return px.isNull() ? colorPixmap(fallback) : px.scaled(TILE, TILE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// use color if failed and resize image
    };
    //load all tile images
    pxFloor      = load(":/assets/floor.png",       QColor(50,  50,  60));
    pxWall       = load(":/assets/wall.png",        QColor(30,  30,  40));
    pxTrap       = load(":/assets/trap.png",        QColor(180, 60,  0));
    pxDoorLocked = load(":/assets/door_locked.png", QColor(100, 70,  0));
    pxDoorOpen   = load(":/assets/door_open.png",   QColor(160, 120, 40));
    pxTreasure   = load(":/assets/treasure.png",    QColor(255, 210, 0));
    pxEleven     = load(":/assets/eleven.png",      QColor(60,  120, 255));
    pxPapa       = load(":/assets/papa.png",        QColor(200, 40,  40));

    spritesLoaded = true;//shows that images have loaded
}
// initializes level and creates sprites and HUD
void GameView::initLevel(Level* lvl) {
    level = lvl;// store level
    scene->clear(); // clear previous scene

    if (!spritesLoaded) loadPixmaps();// load images

    // create player sprite
    playerSprite = scene->addPixmap(pxEleven);
    playerSprite->setZValue(2);// draw above tiles
    // create enemy sprite
    enemySprite = scene->addPixmap(pxPapa);
    enemySprite->setZValue(2);// draw above tiles

    hud = new HUD(scene, 9 * TILE);// create HUD under the map
}
// draws map tiles
void GameView::drawMap() {
    GridMap* map = level->getMap();// get map data
    Door*    door = level->getDoor(); // get door state like whether locked or unlocked

    // loop through all tiles
    for (int y = 0; y < map->getHeight(); y++) {
        for (int x = 0; x < map->getWidth(); x++) {
            Tile t = map->getTile(Position(x, y));// get tile
            QPixmap* px = &pxFloor;//shows that the default is the floor

            switch (t.getType()) {// choose correct image based on tile type
            case Wall:         px = &pxWall;       break;
            case TrapTile:     px = &pxTrap;       break;
            case TreasureTile: px = &pxTreasure;   break;
            case DoorTile:
                px = door->isOpen() ? &pxDoorOpen : &pxDoorLocked;// open/closed door
                break;
            default:           px = &pxFloor;      break;
            }

            auto* item = scene->addPixmap(*px);// draw tile on scene
            item->setPos(x * TILE, y * TILE); // the position on grid
            item->setZValue(0);// draw background
        }
    }
}
// draws objects
void GameView::drawObjects() {

    if (level->getTreasure()->isCollected()) {// if treasure is collected it darkens the tile
        auto* dim = scene->addRect(
            level->getTreasure()->getPosition().x * TILE,
            level->getTreasure()->getPosition().y * TILE,
            TILE, TILE,
            QPen(Qt::NoPen), QBrush(QColor(0, 0, 0, 160)));//sort of transparent layer on top
        dim->setZValue(1);//draws this dark layer on top of the tiles but under the player and enemy
    }
}

void GameView::drawCharacters() {// draws and updates player and enemy positions
    Player* p = level->getPlayer();// get player
    Enemy*  e = level->getEnemy();// get enemy

    if (p->isAlive()) {// update player
        playerSprite->setPos(p->getPosition().x * TILE, p->getPosition().y * TILE);
        playerSprite->setVisible(true);
    } else {// hide if dead
        playerSprite->setVisible(false);
    }

    if (e->isAlive()) { // update enemy
        enemySprite->setPos(e->getPosition().x * TILE, e->getPosition().y * TILE);
        enemySprite->setVisible(true);
    } else {// hide if dead
        enemySprite->setVisible(false);
    }
}

void GameView::redraw(bool isPlayerTurn) {// redraws the entire scene
    QList<QGraphicsItem*> all = scene->items();// removes all old tiles and objects but keeps characters
    for (auto* item : all) {
        if (item->zValue() < 2) {//if  item is not a character then it removes it
            scene->removeItem(item);
            delete item;
        }
    }
    drawMap();// draw tiles
    drawObjects();// draw objects
    drawCharacters();// update character
    if (hud) hud->update(level->getPlayer(), level->getEnemy(), isPlayerTurn);// it updates HUD
}

void GameView::showMessage(const QString& msg) {// displays message on HUD
    if (hud) hud->showMessage(msg);
}

void GameView::keyPressEvent(QKeyEvent* event) {// handles keyboard input

    switch (event->key()) { // movement controls so the arrow keys and WASD
    case Qt::Key_Up:    case Qt::Key_W: emit moveRequested( 0, -1); break;
    case Qt::Key_Down:  case Qt::Key_S: emit moveRequested( 0,  1); break;
    case Qt::Key_Left:  case Qt::Key_A: emit moveRequested(-1,  0); break;
    case Qt::Key_Right: case Qt::Key_D: emit moveRequested( 1,  0); break;
    case Qt::Key_Q:                     emit psychicAttackRequested(); break;// enables special ability
    case Qt::Key_Return:
    case Qt::Key_Space:                 emit endTurnRequested(); break;// end turn
    default: break;// ignore other keys
    }
}