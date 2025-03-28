#include "lib/IElement.hpp"
#include <stdlib.h>   // srand, rand
#include "etc/env"

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