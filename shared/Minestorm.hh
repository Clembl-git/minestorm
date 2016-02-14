#ifndef MINESTORM_HH
# define MINESTORM_HH

# include <QDebug>

# define DEBUG(x,y) do { \
  if (y) { qDebug() << x; } \
} while (0)

/* GLOBALS */
# define SCREEN_WIDTH   1300
# define SCREEN_HEIGHT  650
# define SERVER_PORT    4242

/* CORE */
# define CYCLE_PER_S    25
# define MAX_CLIENTS    50
# define MAX_PLAYERS    4
# define SCORE_MINE     100

/* DISPLAY */
# define EVENT_PER_S    15

/* ENTITIES */
# define SHOT_PER_S     4
# define SHIP_SIZE      32
# define MINE_MIN_DELAY 4
# define MINE_MAX_DELAY 20
# define SHIELD_REPOP   20

#endif // MINESTORM_HH

