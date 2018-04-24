#pragma once
#include "Game.h"
#include "collidable.h"
#include "SVGParser.h"
#include <vector>

class map :public sf::Drawable{
public:

	marker *pStart = nullptr;
	marker *pCur = nullptr;
	vector<marker *> checkpoints;
	sf::RectangleShape * startBox = nullptr;

	// constructor

	map(string filePath) {
		SVGParser svgp(filePath);
		svgp.parse();
		for (G_Layer layer : svgp.glayers) {
			for (SVGData block : layer.svgs) {
				if (layer.id == "marker\">") {
					marker * temp = new marker(sf::Vector2f(block.width, block.height), sf::Vector2f(block.x, block.y), block.rotation, sf::Color::Transparent, false);
					temp->setOutlineColor(sf::Color::Magenta);
					temp->setOutlineThickness(5.f);
					collidables.push_back(temp);

					if (pStart == nullptr) {
						pStart = temp;
					}
					else {
						pCur->pNextMarker = temp;
					}
					pCur = temp;
					checkpoints.push_back(pCur);
				}
				else if (layer.id == "solid\">") {
					collidables.push_back(new collidable(sf::Vector2f(block.width, block.height), sf::Vector2f(block.x, block.y), block.rotation, sf::Color::Cyan, true));
				}
				else if (layer.id == "start\">") {
					startBox = new sf::RectangleShape(sf::Vector2f(block.width, block.height));
					startBox->setPosition(sf::Vector2f(block.x, block.y));
				}
				else {
					cout << "Nope" << endl;
				}
			}
		}

		pCur->pNextMarker = pStart; // circular list

		for (collidable * it : collidables) {
			drawables.push_back(it);
		}
	}
	vector<collidable *> collidables;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const {
		for (Drawable * it : drawables)
			target.draw(*it, states);
	}
	~map() {
		for (collidable * it : collidables)
			it = nullptr;
		for (Drawable * it : drawables)
			delete it;
	}
private :
	vector<sf::Drawable *> drawables;

};