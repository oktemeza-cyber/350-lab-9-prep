#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}

    // TODO: all of these:

    sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) { 
        const sf::Vector2f offset = pWindow - mWindowSize / 2.f;
        return {mViewCenterWorld.x + offset.x * mWorldUnitsPerPixel, mViewCenterWorld.y - offset.y * mWorldUnitsPerPixel};
    };

    sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) { 
        const sf::Vector2f pWindowOffset = pWorld - mViewCenterWorld;
        sf::Vector2f offsetPixel = {pWindowOffset.x/mWorldUnitsPerPixel, -pWindowOffset.y/mWorldUnitsPerPixel};
        return offsetPixel + mWindowSize / 2.f;
    };

    sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) { 
        sf::Vector2f conversion = {mWindowSize.x * pWindowPercent.x, mWindowSize.y * pWindowPercent.y};
        return windowToWorld(conversion);
     };

    sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) { 
        sf::Vector2f conversion = worldToWindow(pWorld);
        return {conversion.x/mWindowSize.x, conversion.y/mWindowSize.y};
    };

    void translateWorld(const sf::Vector2f& deltaWorld) {
       mViewCenterWorld = deltaWorld + mViewCenterWorld;
    };

    void translateWindow(const sf::Vector2f& deltaScreen) {
        sf::Vector2f offsetPixel = {deltaScreen.x*mWorldUnitsPerPixel, -deltaScreen.y*mWorldUnitsPerPixel};
        mViewCenterWorld = offsetPixel + mViewCenterWorld;
    };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        sf::Vector2f conversion = {mWindowSize.x * deltaScreenPercent.x, mWindowSize.y * deltaScreenPercent.y};
        translateWindow(conversion);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        sf::Vector2f changed = windowToWorld(pWindow);
        translateWorld(pWorld - changed);
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        sf::Vector2f conversion = {mWindowSize.x * pWindowPercent.x, mWindowSize.y * pWindowPercent.y};
        setCamera(pWorld, conversion);
    };

    void setCameraCenter(const sf::Vector2f& pWorld) { mViewCenterWorld = pWorld;};

    sf::Vector2f getCameraCenter() const { return mViewCenterWorld; };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
