#ifndef _LEVEL_H_
#define _LEVEL_H_

class actor //temp
{

};

class Level
{
	friend class Game;
public:
	Level();

	bool Update(float a_dt);
	void Draw();

public:
	std::vector<actor> m_actors;

private:
	char* name;

};

#endif // !_LEVEL_H_
