// firework.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream> 
#include <math.h>
#include <time.h>

#include <SFML/Graphics.hpp>

#define TAILLE_ECRAN_X 1000
#define TAILLE_ECRAN_Y 1000



class Particle
{
public:
    Particle(sf::Vector2i position, sf::Vector2i direction, int dureeVie, int generatrice, int nombreGeneration, 
            int delaisGeneration, sf::Color couleur, sf::Color couleurCible):
        m_position(position),
        m_direction(direction),
        m_vitesse(sqrt((m_direction.x* m_direction.x) + (m_direction.y * m_direction.y))),
        m_dureeVie(dureeVie),
        m_dureeVieInitiale(dureeVie),
        m_generatrice(generatrice),
        m_nombreGeneration(nombreGeneration),
        m_nombreGenerationInitial(nombreGeneration),
        m_delaisGeneration(delaisGeneration),
        m_compteurDelaisGeneration(0),
        m_couleur(couleur),
        m_couleurCible(couleurCible)
    {
        m_shape = sf::CircleShape(2.f);
        m_shape.setFillColor(m_couleur);
        m_shape.setPosition(sf::Vector2f(m_position));
        

    }
    ~Particle()
    {

    }

    int deplacement()
    {
        m_position.x += m_direction.x;
        m_position.y += m_direction.y;
        m_shape.setPosition(sf::Vector2f(m_position));
        m_compteurDelaisGeneration++;

        m_couleur.r -= (m_couleur.r - m_couleurCible.r) / m_dureeVie; 
        m_couleur.g -= (m_couleur.g - m_couleurCible.g) / m_dureeVie; 
        m_couleur.b -= (m_couleur.b - m_couleurCible.b) / m_dureeVie;

        m_shape.setFillColor(m_couleur);


        if (m_generatrice > 0 && m_compteurDelaisGeneration >= m_delaisGeneration)
        {
            m_compteurDelaisGeneration = 0;
            if (m_nombreGeneration >= 0)
            {
                for (int i = 0; i <  m_nombreGeneration; i++)
                {
                    //std::cout << "generation " << std::endl;
                    sf::Vector2i direction{ rand() % m_direction.x - (m_direction.x / 2), rand() % m_direction.y - (m_direction.y / 2) };

                    if (direction.x == 0) { direction.x = 1; }
                    if (direction.y == 0) { direction.y = 1; }

                    int nombreGenerations =  m_nombreGenerationInitial-1; 
                    int delaisGeneration =  m_delaisGeneration;
                    //std::cout << nombreGenerations << " " << delaisGeneration << std::endl;


                    m_listeParticles.push_back(std::make_shared<Particle>(Particle(m_position, direction,
                        m_dureeVieInitiale-1,
                        m_generatrice - 1,
                        nombreGenerations,
                        delaisGeneration,
                        sf::Color(255, 255, 255), m_couleurCible)));
                    
                }
                //std::cout << m_listeParticles.size() << std::endl;
                m_nombreGeneration--;
            }
        }
        m_dureeVie--;
        return m_dureeVie; 
    }

    static sf::Uint8 modifCouleur(int initial, int intervale)
    {
        int res = initial + (rand() % intervale - (intervale/2));
        if (res > 255)
        {
            res = 255; 
        }
        else if (res < 0)
        {
            res = 0;
        } 
        return res; 
    }

    sf::CircleShape getShape() { return m_shape;  }


    static void ajoutParticle(std::shared_ptr<Particle> particle) { m_listeParticles.push_back(particle); }
    static void deplacementParticles()
    {
        for (long i = 0; i < m_listeParticles.size(); i++)
        {
            if (m_listeParticles[i]->deplacement() == 0) // duree Vie à 0
            {
                m_listeParticles.erase(m_listeParticles.begin() + i);
                i--;
            }
        }
    }
    static void afficherParticles(sf::RenderWindow* render)
    {
        for (long i = 0; i < m_listeParticles.size(); i++)
        {
            //std::cout << m_listeParticles.size() << std::endl;
            render->draw(m_listeParticles[i]->getShape());
        }

    }
protected:
    sf::Vector2i m_position;
    sf::Vector2i m_direction;
    int m_vitesse; // utilisé dans le calcul de la vitesse de déplacement
    int m_dureeVie;
    int m_dureeVieInitiale;

    int m_generatrice;
    int m_nombreGeneration;
    int m_nombreGenerationInitial;
    int m_delaisGeneration;
    int m_compteurDelaisGeneration;

    sf::Color m_couleur;
    sf::Color m_couleurCible;
    

    sf::CircleShape m_shape;

    static std::vector<std::shared_ptr<Particle>> m_listeParticles;
};


std::vector < std::shared_ptr< Particle >> Particle::m_listeParticles;

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(TAILLE_ECRAN_X , TAILLE_ECRAN_Y), "Firework!");

    Particle::ajoutParticle(
        std::make_shared<Particle>(Particle(   sf::Vector2i(TAILLE_ECRAN_X / 2, TAILLE_ECRAN_Y / 2),
                    sf::Vector2i(rand()%40-20,rand()%40-20),
                    20,
                    3,
                    4,
                    2,
                    sf::Color(255, 255, 255),
                    sf::Color(255, 0,   255)
        )));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                int dirX = rand() % 40 - 20; if (dirX == 0) { dirX = 1; }
                int dirY = rand() % 40 - 20; if (dirY == 0) { dirY = 1; }

                std::cout << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl; 
                Particle::ajoutParticle(
                    std::make_shared<Particle>(Particle(sf::Mouse::getPosition(window),
                        sf::Vector2i(dirX,dirY),
                        20,
                        4,
                        3,
                        2,
                        sf::Color(255, 255, 255),
                        sf::Color(255, 0, 255)
                    )));

                

                
            }
        }
        window.clear();
        sf::sleep(sf::milliseconds(16)); 
        Particle::deplacementParticles();
        Particle::afficherParticles(&window);
        window.display();
    }

    return 0;
}