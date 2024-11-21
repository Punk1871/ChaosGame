// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>

#include <SFML/Graphics/ConvexShape.hpp>
#include <random>

// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main()
{
    // constant variables
    const string name_of_the_game = "Chaos Game";
    const string font_name = "arial.ttf";

    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, name_of_the_game, Style::Default);

    vector<Vector2f> vertices;
    vector<Vector2f> points;

    // For handling a right click, and creating shape once
    bool create = false;
    bool outShape = false;
    // For restricting the point chooser algortihm to not use the same point twice in a row later, without it no fractals on higher poly
    int last_vertex = 0;

    // Loading font
    Font font;
    if (!font.loadFromFile(font_name))
    {
        cout << "Error loading " << font_name << endl; // error...
    }

    // Setting text element
    Text text;
    text.setFont(font);
    text.setString("left-click 3+ points to form a shape");
    text.setCharacterSize(100);
    text.setColor(sf::Color::White);

    // Setting shape cosmetic parameters
    ConvexShape shape;
    shape.setOutlineColor(Color::White);
    shape.setOutlineThickness(2);
    shape.setFillColor(Color::Transparent);


    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::cout << "the left button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;

                    if (!create) //path shuts off after right right click
                    {
                        vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    else if (points.size() == 0) //accepts only initial point click
                    {
                        ///push back to points vector
                        points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    if (vertices.size() >= 3) // set right-click activated after 3+ points
                    {
                        create = true;
                    }
                    if (outShape && vertices.size() > 1) // reset all variables
                    {
                        text.setString("left-click 3+ points to form a shape");
                        vertices.clear();
                        points.clear();
                        create = false;
                        outShape = false;
                    }
                }
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        /*
        ****************************************
        Update
        ****************************************
        */
        int numVert = vertices.size();
        int numPoints = points.size();
        if (vertices.size() >= 3 && !outShape) text.setString("add more points or right-click to set shape");
        else if (outShape && points.size() == 0) text.setString("left-click inside the shape to start the Chaos");
        else if (outShape) text.setString("  right-click to reset");

        //create shape
        if (create && !outShape)
        {
            shape.setPointCount(vertices.size()); // set number of points
            for (int i = 0; i < numVert; i++) // apply all point coodinates
            {
                shape.setPoint(i, vertices[i]);
            }
            outShape = true; //confirms shape is made so is not remade every loop
        }

        if (points.size() > 0) //active first click after right click
        {
            for (int i = 0; i < 10; i++) // Generate points 10 times a frame for speed
            {
                ///generate more point(s)
                ///select random vertex
                ///calculate midpoint between random vertex and the last point in the vector
                ///push back the newly generated coord.

                int random_vertex = rand() % vertices.size(); // picks a random vertex

                if (vertices.size() > 3) // for shapes with 3 or more points add a restriction on vertex selection
                {
                    while (random_vertex == last_vertex) // next vertex cannot be the same as the last vertex
                    {
                        random_vertex = rand() % vertices.size();
                    }
                    last_vertex = random_vertex;
                }

                // mid point algorithm
                int midpointX = (vertices[random_vertex].x + points.back().x) / 2;
                int midpointY = (vertices[random_vertex].y + points.back().y) / 2;
                points.push_back(Vector2f(midpointX, midpointY));
            }
        }

        /*
        ****************************************
        Draw
        ****************************************
        */
        window.clear();
        srand(0); // seed is set so rainbow dots are the same, without seed shimmers,
        for (int i = 0; i < numPoints; i++) //draw all dots
        {
            RectangleShape rect(Vector2f(1, 1));
            rect.setPosition(Vector2f(points[i].x, points[i].y));
            rect.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255)); // rainbow dots
            window.draw(rect);
        }

        for (int i = 0; i < numVert; i++) //draw verticies
        {
            RectangleShape rect(Vector2f(5, 5));
            rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
            rect.setFillColor(Color::Red);
            window.draw(rect);
        }

        window.draw(text);
        if (outShape) window.draw(shape); //only draw the shape if it is created
        window.display();
    }
}
