#include "Actor.h"
#include "StudentWorld.h"

// Actor Implementation

//Actor::Actor(int imageID, int startX, int startY, Direction dir,
//             double size, unsigned int depth):
//GraphObject(imageID, startX, startY, dir, size, depth) {}
//
//

// GraphObject Constructor
//    GraphObject(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
//    : m_imageID(imageID), m_visible(false), m_x(startX), m_y(startY),
//	   m_destX(startX), m_destY(startY), m_brightness(1.0),
//	   m_animationNumber(0), m_direction(dir), m_size(size), m_depth(depth)
//    {
//        if (m_size <= 0)
//            m_size = 1;
//
//        getGraphObjects(m_depth).insert(this);
//    }
//

// Person Implementation



// Iceman Implementation
//Iceman::Iceman(): Actor(IID_PLAYER, 30, 60) {}



//void Actor::makeVisible()  // Make class object visible
//{
//    setVisible(true);
//}


// Ice Implementation

Ice::Ice(int startX, int startY): Actor(IID_ICE, startX, startY, right, 0.25, 3)
{
    setVisible(true);
}
Ice::~Ice()
{
    setVisible(false);
}
void Ice::doSomething() {}


