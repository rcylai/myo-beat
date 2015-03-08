#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#include <myo/myo.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include "Note.h"

class GameEngine
{
private:
	sf::RenderWindow* window;
	std::queue <Note> map;
	std::vector<Note> onScreen;
	long time;
	std::string title;
	std::string songfile;
	sf::Texture ot[4];
public:
	GameEngine(sf::RenderWindow *window)
	{
		//window->clear(sf::Color(255, 255, 255));

		this->window = window;
		
		/*ot[0].loadFromFile("Images/fist.png");
		system("pause");*/

		if (!ot[0].loadFromFile("Images/fist.png")) {
			std::cerr << "Could not load fist" << std::endl;
		}
		
		if (!ot[1].loadFromFile("Images/spread.png")) {
			std::cerr << "Could not load spread" << std::endl;
		}
		if (!ot[2].loadFromFile("Images/in.png")) {
			std::cerr << "Could not load in" << std::endl;
		}
		if (!ot[3].loadFromFile("Images/out.png")) {
			std::cerr << "Could not load out" << std::endl;
		}
	}
	void setTime(long time)
	{
		this->time = time;
	}
	void drawNotes()
	{
		if (onScreen.size() > 0)
		{
			for (int i = 0; i <= onScreen.size() - 1; i++)
			{
				std::cout << onScreen[i].getPosition().x << " | " << onScreen[i].getPosition().y << std::endl;
				window->draw(onScreen[i]);
			}
		}
	}
	void killNotes()
	{
		if (onScreen.size() > 0 && time - 304 > onScreen[onScreen.size() - 1].getTime())
		{
			onScreen.pop_back();
		}
		//system("pause");
	}
	void loadNotes()
	{
		if (map.empty() == false && time + 2000 >= map.front().getTime())
		{
			auto it = onScreen.begin();
			it = onScreen.insert(it, map.front());
			map.pop();
			//std::cout << onScreen.size();
		}
	}

	void setNotePositions()
	{
		if (onScreen.size() > 0)
		{
			for (int i = 0; i <= onScreen.size() - 1; i++)
			{
				onScreen[i].setPosition(sf::Vector2f(173 + onScreen[i].getColumn() * 340, 667 - (onScreen[i].getTime() - time) / 3));
			}
		}
	}

	void update(long t)
	{
		setTime(t);
		killNotes();
		loadNotes();
		setNotePositions();
	}
	
	void loadMap(std::string fileName)
	{

		std::ifstream mapReader(fileName);
		if (!mapReader) {
			std::cerr << "Could not load map" << std::endl;
		}
		getline(mapReader, title);
		getline(mapReader, songfile);
		long t;
		int c, p;
		myo::Pose pp;

		mapReader >> t >> c >> p;
		while (!mapReader.fail())
		{		
			if (p == 0) // fist, spread, in, out
			{
				pp = myo::Pose::fist;
			}
			else if (p == 1)
			{
				pp = myo::Pose::fingersSpread;
			}
			else if (p == 2)
			{
				pp = myo::Pose::waveIn;
			}
			else if (p == 3)
			{
				pp = myo::Pose::waveOut;
			}
			else
			{
				//die
			}
			Note newNote(t, c, pp, ot[p]);
			map.push(newNote);
			mapReader >> t >> c >> p;
		}
		mapReader.close();
	}
	std::string getTitle()
	{
		return title;
	}

	std::string getSongFile()
	{
		return songfile;
	}

	void spitOutMap()
	{
		while (!map.empty())
		{
			std::cout << map.front().getTime() << std::endl;
			map.pop();
		}
	}
};
#endif