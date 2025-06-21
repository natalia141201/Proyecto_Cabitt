#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Zanahorias {
public:
    Zanahorias(sf::Vector2f position)
    {
        if (!textureCargada) {
            if (!texture.loadFromFile("assets/images/Zanahoria.png")) {
                std::cerr << "No se pudo cargar la imagen de la zanahoria\n";
            }
            textureCargada = true;
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(0.1f, 0.1f);
    }

    void mover(float offsetX, float offsetY)
    {
        sprite.move(offsetX, offsetY);
    }

    void dibujar(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

private:
    sf::Sprite sprite;
    static sf::Texture texture;
    static bool textureCargada;
};

// Definición de los miembros estáticos FUERA de la clase:
sf::Texture Zanahorias::texture;
bool Zanahorias::textureCargada = false;