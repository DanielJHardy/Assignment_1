#ifndef _LEVEL_H_
#define _LEVEL_H_

class actor //temp
{

};

// Level class for containing and updating all actors in the level
class Level
{
	friend class Game;
public:
	Level();

	bool Startup();
	void Shutdown();

	bool Update(float a_dt);
	void Draw();

public:
	std::vector<actor> m_actors;	//all the actors in the level

private:
	char* m_name;	//level title

};

#endif // !_LEVEL_H_
