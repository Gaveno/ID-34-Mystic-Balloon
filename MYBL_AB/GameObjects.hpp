#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H

#include "LightweightLinkedList.hpp"
#include "HighRect.hpp"

class GameObject {
public:
    int x;
    int y;
    uint8_t objType;
    virtual void draw() = 0; // Pure virtual function for drawing the object.
    virtual void update(HighRect &playerRect, HighRect &playerSuckRect) = 0; // Pure virtual function for updating the object's state.
    GameObject* next;
    GameObject() : GameObject(0, 0) {}
    GameObject(int x, int y) : x(x), y(y), objType(0), next(nullptr) {}
};

class GameObjects {
private:
    static LightweightLinkedList<GameObject> foregroundObjects;
    static LightweightLinkedList<GameObject> backgroundObjects;

public:
    static void addForegroundObject(GameObject* obj) {
        foregroundObjects.add(obj);
    }

    static void addBackgroundObject(GameObject* obj) {
        backgroundObjects.add(obj);
    }

    static void drawAll() {
        // Iterate through the background objects and call draw on each one.
        auto currentNode = backgroundObjects.head;
        while (currentNode) {
            currentNode->draw();
            currentNode = currentNode->next;
        }

        // Iterate through the foreground objects and call draw on each one.
        currentNode = foregroundObjects.head;
        while (currentNode) {
            currentNode->draw();
            currentNode = currentNode->next;
        }
    }

    static void updateAll(HighRect &playerRect, HighRect &playerSuckRect) {
        // Iterate through the background objects and call update on each one.
        auto currentNode = backgroundObjects.head;
        while (currentNode) {
            currentNode->update(playerRect, playerSuckRect);
            currentNode = currentNode->next;
        }

        // Iterate through the foreground objects and call update on each one.
        currentNode = foregroundObjects.head;
        while (currentNode) {
            currentNode->update(playerRect, playerSuckRect);
            currentNode = currentNode->next;
        }
    }

    static void removeForegroundObject(GameObject* objLocation) {
      foregroundObjects.remove(objLocation);
    }

    static void removeBackgroundObject(GameObject* objLocation) {
      backgroundObjects.remove(objLocation);
    }

    static GameObject* getForegroundHead() {
        return foregroundObjects.head;
    }

    static GameObject* getBackgroundHead() {
        return backgroundObjects.head;
    }

    static void clearForeground() {
      foregroundObjects.clear();
    }

    static void clearBackground() {
      backgroundObjects.clear();
    }
};

// Define the static members outside the class
LightweightLinkedList<GameObject> GameObjects::foregroundObjects;
LightweightLinkedList<GameObject> GameObjects::backgroundObjects;

#endif // GAME_OBJECTS_H
