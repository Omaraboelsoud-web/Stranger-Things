#include "gameview.h"//includes the gameview class
#include <QKeyEvent>//handles keyboard input
#include <QBrush>//to color objects
#include <QPen>// for drawing outlines
#include <QGraphicsRectItem>//rectangle graphics items
#include <QPixmap>//for handling images
#include <QPainter>// to draw on pixmaps
#include <QScreen> // Used to get the user's screen size
#include <QApplication> // Used to access the main application and screen data

// Creates a solid-color fallback pixmap when an image file is missing
static QPixmap colorPixmap(QColor c, int w, int h) {
    QPixmap px(w, h); // Create a pixmap with the given width and height
    px.fill(c);       // Fill the entire pixmap with the chosen color
    return px;        // Return the generated solid-color pixmap
}
// Removes white background from an image by making near-white pixels transparent
static QPixmap removeWhiteBackground(const QPixmap& src, int threshold = 30) {
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    // Convert pixmap to image and ensure it supports transparency (ARGB32)
    for (int y = 0; y < img.height(); y++) { // Loop through each row of pixels
        for (int x = 0; x < img.width(); x++) { // Loop through each column of pixels
            QColor c = img.pixelColor(x, y); // Get the color of the current pixel

            // Check if the pixel is close to white based on RGB threshold
            // If it is, we treat it as background and make it transparent
            if (c.red()   > 255 - threshold &&
                c.green() > 255 - threshold &&
                c.blue()  > 255 - threshold) {

                img.setPixelColor(x, y, Qt::transparent); // Make the pixel fully transparent
            }
        }
    }
    return QPixmap::fromImage(img); // Convert the modified image back to a pixmap and return it
}
//initializes the game view
GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),// base class constructor
    scene(new QGraphicsScene(this)),// create graphics scene
    level(nullptr), hud(nullptr),// no level or HUD yet
    playerSprite(nullptr),// player sprite not created yet
    spritesLoaded(false) // images not loaded yet
{
    setScene(scene);// connect scene to view
    setWindowTitle("Stranger Things: The Dungeon");// window title

    // Get full screen size
    QScreen* screen = QApplication::primaryScreen(); // Gets the player's monitor screen
    QRect screenRect = screen->availableGeometry(); // Gets the available screen dimensions
    screenW = screenRect.width(); // Stores the width of the screen
    screenH = screenRect.height(); // Stores the height of the screen

    // Fill entire screen
    setFixedSize(screenW, screenH); // Makes the game window take the full screen size
    showFullScreen(); // Displays the game in fullscreen mode

    setFocusPolicy(Qt::StrongFocus);// allows keyboard input
    setBackgroundBrush(QColor(10, 10, 15));// dark background color

    // Default tile size will be recalculated in initLevel()
    tileSize = TILE; // Sets default tile size before recalculating
}

GameView::~GameView() {}// destructor

// Recalculate tile size so the map fills the screen
void GameView::recalcTileSize() {
    if (!level || !level->getMap()) return; // Stops if there is no level or map loaded

    int mapW = level->getMap()->getWidth(); // Gets map width
    int mapH = level->getMap()->getHeight(); // Gets map height

    // Reserve 110px at the bottom for the HUD
    int availH = screenH - 110; // Leaves space for the HUD under the map
    int availW = screenW; // Uses full screen width

    // Pick the largest tile size that fits both dimensions
    int tileByW = availW / mapW; // Calculates tile size based on width
    int tileByH = availH / mapH; // Calculates tile size based on height
    tileSize = qMin(tileByW, tileByH); // Uses the smaller value so everything fits on screen

    // Clamp so tiles don't get absurdly large on small maps
    tileSize = qMin(tileSize, 80); // Maximum tile size is 80
    tileSize = qMax(tileSize, 20); // never smaller than 20px
}

void GameView::loadPixmaps() {// loads all image assets
    // function to load image or color

        auto load = [this](const QString& path, QColor fallback) { // Helper function (lambda) to load an image or fall back to a colored tile if loading fails
            QPixmap px(path); // Try to load the image from file path

            if (px.isNull())  // If the image fails to load, return a simple colored placeholder tile instead
                return colorPixmap(fallback, tileSize, tileSize);
            px = removeWhiteBackground(px); // Remove white background from the image for cleaner transparency

            // Create a properly sized pixmap for the game tile
            QPixmap result(tileSize, tileSize);
            result.fill(Qt::transparent); // Start with a transparent background
            QPainter painter(&result);
            painter.setRenderHint(QPainter::SmoothPixmapTransform); // Enable smooth scaling for better visual quality
            painter.drawPixmap(result.rect(), px.scaled(tileSize, tileSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)); // Scale and draw the image into the tile area
            painter.end(); // Finish painting
            return result; // Return the final processed pixmap
        };

    //load all tile images
    pxFloor      = load(":/assets/floor.png", QColor(50,  50,  60));
    pxWall       = load(":/assets/wall.png", QColor(30,  30,  40));
    pxTrap       = load(":/assets/trap.png", QColor(180, 60,  0));
    pxDoorLocked = load(":/assets/door_locked.png", QColor(100, 70,  0));
    pxDoorOpen   = load(":/assets/door_open.png", QColor(160, 120, 40));
    pxTreasure   = load(":/assets/treasure.png", QColor(255, 210, 0));
    pxEleven     = load(":/assets/eleven.png", QColor(60,  120, 255));
    pxPapa       = load(":/assets/papa.png", QColor(200, 40,  40));
    pxDemogorgon = load(":/assets/demogorgon.png", QColor(200, 40,  40));
    pxDemoBats   = load(":/assets/demobat.png", QColor(200, 40,  40));
    pxMindFlayer = load(":/assets/mindflayer.png", QColor(200, 40,  40));

    spritesLoaded = true;//shows that images have loaded
}

// initializes level and creates sprites and HUD
void GameView::initLevel(Level* lvl) {
    level = lvl;// store level
    scene->clear(); // clear previous scene
    enemySprites.clear(); // Clears all old enemy sprites
    spritesLoaded = false; // force reload at new tile size

    // Recalculate tile size for this level's map dimensions
    recalcTileSize(); // Recalculates tile size depending on map size

    // Reload pixmaps at the new tile size
    loadPixmaps(); // Reloads images with updated tile size

    // Update window title
    setWindowTitle(
        QString("Stranger Things: The Dungeon -- Level %1").arg(level->getLevelNum())
        ); // Shows current level number in the title

    // Resize the scene to fit the map + HUD
    int mapPixW = level->getMap()->getWidth()  * tileSize; // Calculates map width in pixels
    int mapPixH = level->getMap()->getHeight() * tileSize; // Calculates map height in pixels
    scene->setSceneRect(0, 0, mapPixW, mapPixH + 110); // Resizes the scene area including HUD space

    // Center the scene in the fullscreen view
    setAlignment(Qt::AlignCenter); // Centers the map on screen

    // create player sprite
    playerSprite = scene->addPixmap(pxEleven);
    playerSprite->setZValue(2);// draw above tiles

    // One sprite per enemy
    // Loop through all enemies in the current level
    for (int i = 0; i < level->getEnemies().size(); i++) {
        Enemy* e = level->getEnemies()[i]; // Get the current enemy from the list
        QPixmap spriteToUse;               // This will store the correct sprite for each enemy

        // Choose the correct image based on the enemy type (name)
        if (e->getName() == "MindFlayer")
            spriteToUse = pxMindFlayer;   // Assign MindFlayer sprite
        else if (e->getName() == "DemoBat")
            spriteToUse = pxDemoBats;     // Assign DemoBat sprite
        else if (e->getName() == "Papa")
            spriteToUse = pxPapa;         // Assign Papa sprite
        else
            spriteToUse = pxDemogorgon;   // Default sprite for Demogorgon or unknown enemies
        QGraphicsPixmapItem* sp = scene->addPixmap(spriteToUse); // Add the enemy sprite to the scene
        sp->setZValue(2); // Set rendering order so enemies appear above background elements correctly
        enemySprites.append(sp); // Store the sprite so it can be updated or removed later if needed
    }

    // HUD sits just below the map
    hud = new HUD(scene, mapPixH); // create HUD under the map
}

// draws map tiles
void GameView::drawMap() {
    GridMap* map  = level->getMap();// get map data
    Door*    door = level->getDoor(); // get door state like whether locked or unlocked

    // loop through all tiles
    for (int y = 0; y < map->getHeight(); y++) {
        for (int x = 0; x < map->getWidth(); x++) {
            Tile t = map->getTile(Position(x, y));// get tile
            QPixmap* px = nullptr; // Will be assigned in the switch below based on tile type

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
            item->setPos(x * tileSize, y * tileSize); // the position on grid
            item->setZValue(0);// draw background
        }
    }
}

// draws objects
void GameView::drawObjects() {

    if (level->getTreasure()->isCollected()) {// if treasure is collected it darkens the tile
        auto* dim = scene->addRect(
            level->getTreasure()->getPosition().x * tileSize,
            level->getTreasure()->getPosition().y * tileSize,
            tileSize, tileSize,
            QPen(Qt::NoPen),
            QBrush(QColor(0, 0, 0, 160)));//sort of transparent layer on top
        dim->setZValue(1);//draws this dark layer on top of the tiles but under the player and enemy
    }
}

void GameView::drawCharacters() {// draws and updates player and enemy positions
    Player* p = level->getPlayer();// get player

    if (p->isAlive()) {// update player
        playerSprite->setPos(p->getPosition().x * tileSize,
                             p->getPosition().y * tileSize);
        playerSprite->setVisible(true);
    } else {// hide if dead
        playerSprite->setVisible(false);
    }

    const QVector<Enemy*>& enemies = level->getEnemies(); // Gets all enemies from the level
    for (int i = 0; i < enemies.size() && i < enemySprites.size(); i++) {
        Enemy* e = enemies[i]; // Gets current enemy
        if (e->isAlive()) { // update enemy
            enemySprites[i]->setPos(e->getPosition().x * tileSize,
                                    e->getPosition().y * tileSize);
            enemySprites[i]->setVisible(true);
        } else {// hide if dead
            enemySprites[i]->setVisible(false);
        }
    }
}

void GameView::redraw(bool isPlayerTurn) {// redraws the entire scene
    const QList<QGraphicsItem*> all = scene->items(); // Gets a snapshot of all scene items to selectively remove non-character ones
    for (auto* item : std::as_const(all)) {
        if (item->zValue() < 2) {//if  item is not a character then it removes it
            scene->removeItem(item);
            delete item;
        }
    }

    drawMap();// draw tiles
    drawObjects();// draw objects
    drawCharacters();// update character

    if (hud) {
        hud->update(level->getPlayer(), level->getEnemy(),
                    isPlayerTurn, level->getLevelNum(),
                    level->getEnemies()); // Updates the HUD with player, enemy, level, and turn information
    }
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
    case Qt::Key_Q:  emit psychicAttackRequested(); break;// enables special ability
    case Qt::Key_Return:
    case Qt::Key_Space: emit endTurnRequested(); break;// end turn
    case Qt::Key_F5: emit saveRequested(); break; // Saves the game
    case Qt::Key_F9: emit loadRequested(); break; // Loads saved game
    case Qt::Key_Escape: showNormal(); break; // ESC exits fullscreen
    case Qt::Key_1: emit levelSelectRequested(1); break;
    case Qt::Key_2: emit levelSelectRequested(2); break;
    case Qt::Key_3: emit levelSelectRequested(3); break;
    case Qt::Key_4: emit levelSelectRequested(4); break;
    case Qt::Key_5: emit levelSelectRequested(5); break;
    default: break;// ignore other keys

    }
}