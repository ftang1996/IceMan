#include "Actor.h"
#include "StudentWorld.h"

//////////////////////////////////////////////////////////////
// Actor Implementation                                     //
//////////////////////////////////////////////////////////////
//Actor::Actor(StudentWorld* world, int imageID, int sound,
//             int startX, int startY, Direction dir,
//             double size, unsigned int depth):
//GraphObject(imageID, startX, startY, dir, size, depth)
//{
//    m_world = world;
//    m_sound = sound;
//}
//
//Actor::~Actor()
//{
//    setVisible(false);
//}
//
//StudentWorld* Actor::getWorld()
//{
//    return m_world;
//}
//
//void Actor::playSound()
//{
//    getWorld()->playSound(m_sound);
//    
//}

//////////////////////////////////////////////////////////////
// Person Implementation                                    //
//////////////////////////////////////////////////////////////


// Iceman Implementation
//Iceman::Iceman(): Actor(IID_PLAYER, 30, 60) {}



//void Actor::makeVisible()  // Make class object visible
//{
//    setVisible(true);
//}

//////////////////////////////////////////////////////////////
// Ice Implementation                                       //
//////////////////////////////////////////////////////////////

Ice::Ice(StudentWorld* world, int startX, int startY):
Actor(world, IID_ICE, startX, startY, right, 0.25, 3)
{
    setVisible(true);
}

Ice::~Ice()
{
    setVisible(false);
}

void Ice::doSomething() {}


