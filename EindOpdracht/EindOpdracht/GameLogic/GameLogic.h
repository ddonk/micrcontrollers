typedef enum {
    CORRECTPERC_INVALID,
    BUFFERSIZE_INVALID,
    CORRECTINPUTVALUE_INVALID,
} GAMELOGIC_ERROR;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
} DIRECTION;

typedef DIRECTION dirs[] = {UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD};

typedef byte LCD_ARROWUP[] = {
  0x00,
  0x04,
  0x0E,
  0x1F,
  0x0E,
  0x0E,
  0x0E,
  0x00
};

typedef byte LCD_ARROWDOWN[] = {
  0x00,
  0x0E,
  0x0E,
  0x0E,
  0x1F,
  0x0E,
  0x04,
  0x00
};

typedef byte LCD_ARROWRIGHT[] = {
  0x00,
  0x04,
  0x1E,
  0x1F,
  0x1E,
  0x04,
  0x00,
  0x00
};

typedef byte LCD_ARROWLEFT[] = {
  0x00,
  0x04,
  0x0F,
  0x1F,
  0x0F,
  0x04,
  0x00,
  0x00
};