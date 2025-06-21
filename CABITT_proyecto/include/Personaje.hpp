#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Personaje
{
public:
    Personaje(sf::Vector2f position)
    {
        if (!texturaDerecha.loadFromFile("./assets/images/conejo.png"))
            std::cerr << "No se pudo cargar la imagen conejo_derecha\n";
        if (!texturaIzquierda.loadFromFile("./assets/images/conejo2.png"))
            std::cerr << "No se pudo cargar la imagen conejo_izquierda\n";
        sprite.setTexture(texturaDerecha);
        sprite.setPosition(position);
        sprite.setTextureRect(sf::IntRect(0, 24, frameWidth, frameHeight));
        mirandoDerecha = true;
        vida = 5;
        anchoTotalIzquierda = texturaIzquierda.getSize().x;
    }

    int obtenerVida() const  
    {
        return vida;
    }

    void restarVida(int cantidad = 1) {
        vida -= cantidad;
    }

    void mover(float offsetX, float offsetY)
    {
        sprite.move(offsetX, offsetY);
    }

    void dibujar(sf::RenderWindow &window)
    { 
        window.draw(sprite);
    }

    void actualizar(bool mover)
    {
        if (mover && clock.getElapsedTime().asSeconds() >= frameTime)
        {
            currentFrame = (currentFrame + 1) % numFrames;
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 24, frameWidth, frameHeight));
            clock.restart();
        }
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    void mirarDerecha()
    {
        if (!mirandoDerecha) {
            sprite.setTexture(texturaDerecha);
            mirandoDerecha = true;
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 25, frameWidth, frameHeight));
        }
    }


    
    void mirarIzquierda()
    {
        if (mirandoDerecha) {
            sprite.setTexture(texturaIzquierda);
            mirandoDerecha = false;
            int separacion = 0; 
            int x = anchoTotalIzquierda - (frameWidthI + currentFrame);
            sprite.setTextureRect(sf::IntRect(7, 40, frameWidthI, frameHeightI));
        }
    }

private:
    sf::Sprite sprite;
    sf::Texture texturaDerecha;
    sf::Texture texturaIzquierda;
    sf::Clock clock;
    float frameTime = 0.1f;
    int currentFrame = 0;
    int numFrames = 5;
    int frameWidth = 104;
    int frameHeight = 80;

    int frameWidthI = 95;
    int frameHeightI =80;
    int anchoTotalIzquierda = 600;
    int vida;
    bool mirandoDerecha = true;
};