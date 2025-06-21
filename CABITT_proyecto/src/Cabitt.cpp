#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Personaje.hpp"
#include "Zanahorias.hpp"
#include "Calcetin.hpp"

double velocidad = 2.8;

enum class EstadoJuego { Inicio, Jugando, GameOver };

int main()
{
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Cabbit SFML");
    ventana.setFramerateLimit(60);

    // Fondo
    sf::Texture texturaFondo;
    if (!texturaFondo.loadFromFile("assets/images/fondo.png")) 
    {
        std::cerr << "No se pudo cargar la imagen de fondo\n";
        return -1;
    }
    sf::Sprite spriteFondo(texturaFondo);
    sf::Vector2u tamTextura = texturaFondo.getSize();
    sf::Vector2u tamVentana = ventana.getSize();
    spriteFondo.setScale(
        float(tamVentana.x) / tamTextura.x,
        float(tamVentana.y) / tamTextura.y );

    sf::Font fuente;
    if (!fuente.loadFromFile("assets/fonts/Minecraft.ttf")) {
        std::cerr << "No se pudo cargar la fuente\n";   
    }
    
    srand(static_cast<unsigned>(time(0)));

    std::vector<Zanahorias> zanahoriasVec;
    std::vector<Calcetin> calcetinesVec;

    // Estado del juego y puntaje
    EstadoJuego estado = EstadoJuego::Inicio;
    int puntaje = 0;
    Personaje conejo(sf::Vector2f(400, 300));

    // Inicializa zanahorias y calcetines
    auto reiniciarObjetos = [&]() {
        zanahoriasVec.clear();
        calcetinesVec.clear();
        for (int i = 0; i < 5; ++i) {
            float x = rand() % (ventana.getSize().x - 50);
            float y = rand() % (ventana.getSize().y - 50);
            zanahoriasVec.emplace_back(sf::Vector2f(x, y));    
        }
        for (int i = 0; i < 5; ++i) {
            float x = rand() % (ventana.getSize().x - 50);
            float y = rand() % (ventana.getSize().y - 50);
            calcetinesVec.emplace_back(sf::Vector2f(x, y));    
        }
    };

    reiniciarObjetos();

    // Bucle principal
    while (ventana.isOpen())
    {
        sf::Event evento;
        while (ventana.pollEvent(evento))
        {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear();

        if (estado == EstadoJuego::Inicio) {
            // Pantalla de instrucciones
            sf::Text instrucciones;
            instrucciones.setFont(fuente);
            instrucciones.setString(
                "INSTRUCCIONES\n\n"
                "Mueve al conejo con las flechas\n"
                "Toca zanahorias para sumar puntos\n"
                "Evita los calcetines (pierdes vida)\n\n"
                "Presiona ESPACIO para empezar"
            );
            instrucciones.setCharacterSize(28);
            instrucciones.setFillColor(sf::Color::White);
            instrucciones.setPosition(60, 100);
            ventana.draw(instrucciones);

            // Espera a que el usuario presione ESPACIO para empezar
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                puntaje = 0;
                conejo = Personaje(sf::Vector2f(400, 300));
                reiniciarObjetos();
                estado = EstadoJuego::Jugando;
                sf::sleep(sf::milliseconds(200));
            }
        }
        else if (estado == EstadoJuego::Jugando) {
            ventana.draw(spriteFondo);

     bool moviendo = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    conejo.mirarIzquierda();
    conejo.mover(-velocidad, 0);
    moviendo = true;
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    conejo.mirarDerecha();
    conejo.mover(velocidad, 0);
    moviendo = true;
}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        conejo.mover(0, -velocidad);
        moviendo = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        conejo.mover(0, velocidad);
        moviendo = true;
    }
    conejo.actualizar(moviendo);
    conejo.dibujar(ventana);

            // Zanahorias
            for (auto& z : zanahoriasVec) {
                z.dibujar(ventana);
                z.mover(0.0f, 2.0f);

                if (conejo.getBounds().intersects(z.getBounds())) {
                    puntaje += 1;
                    float nuevaX = rand() % (ventana.getSize().x - 50);
                    z.setPosition(sf::Vector2f(nuevaX, -50));
                }
                sf::Vector2f pos = z.getPosition();
                if (pos.y > ventana.getSize().y) {
                    float nuevaX = rand() % (ventana.getSize().x - 50);
                    z.setPosition(sf::Vector2f(nuevaX, -50));
                }
            }

            // Calcetines
            for (auto& c : calcetinesVec) {
                c.dibujar(ventana);
                c.mover(0.0f, 2.0f);

                if (conejo.getBounds().intersects(c.getBounds())) {
                    conejo.restarVida();
                    float nuevaX = rand() % (ventana.getSize().x - 50);
                    c.obtenerposicion(sf::Vector2f(nuevaX, -50));
                }
                sf::Vector2f pos = c.getPosition();
                if (pos.y > ventana.getSize().y) {
                    float nuevaX = rand() % (ventana.getSize().x - 50);
                    c.obtenerposicion(sf::Vector2f(nuevaX, -50));
                }
            }

            // Textos de vida y puntaje
            sf::Text vidaTexto;
            vidaTexto.setFont(fuente);
            vidaTexto.setString("Vida: " + std::to_string(conejo.obtenerVida()));
            vidaTexto.setCharacterSize(30);
            vidaTexto.setFillColor(sf::Color::Cyan);
            vidaTexto.setPosition(10, 10);
            ventana.draw(vidaTexto);

            sf::Text puntajeTexto;
            puntajeTexto.setFont(fuente);
            puntajeTexto.setString("Puntaje: " + std::to_string(puntaje));
            puntajeTexto.setCharacterSize(30);
            puntajeTexto.setFillColor(sf::Color::Yellow);
            puntajeTexto.setPosition(10, 50);
            ventana.draw(puntajeTexto);

            // Si la vida llega a 0, cambia el estado
            if (conejo.obtenerVida() <= 0) {
                estado = EstadoJuego::GameOver;
                sf::sleep(sf::milliseconds(200));
            }
        }
        else if (estado == EstadoJuego::GameOver) {
            // Pantalla de Game Over
            sf::Text gameOver;
            gameOver.setFont(fuente);
            gameOver.setString("GAME OVER\n\nPuntaje: " + std::to_string(puntaje) + "\n\nPresiona R para reiniciar");
            gameOver.setCharacterSize(36);
            gameOver.setFillColor(sf::Color::Red);
            gameOver.setPosition(120, 180);
            ventana.draw(gameOver);

            // Espera a que el usuario presione R para reiniciar
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                estado = EstadoJuego::Inicio;
                sf::sleep(sf::milliseconds(200));
            }
        }

        ventana.display();
    }

    return 0;
}