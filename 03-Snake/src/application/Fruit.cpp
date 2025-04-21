#include "../domain/IElement.h"
#include "../env"
#include <stdlib.h> // srand, rand

class Fruit : public IElement
{

public:
    Fruit()
    {
        Initialize();
    }

    void Move()
    {
        Initialize();
    }

private:
    void Initialize()
    {
        position.x = (rand() % BOARD_WIDTH);
        position.y = (rand() % BOARD_HEIGHT);
    }
};